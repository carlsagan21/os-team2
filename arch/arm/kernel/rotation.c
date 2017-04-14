#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/list.h>
#include <linux/slab.h> /* for kmalloc(), GFP_KERNEL. linux/gfp 따로 include 안해도 됨. */
#include <linux/spinlock.h>

rotation_t rotation;

LIST_HEAD(pending_lh);
LIST_HEAD(wait_read_lh);
LIST_HEAD(wait_write_lh);
LIST_HEAD(acquired_lh);

DEFINE_SPINLOCK(list_iteration_spin_lock);
unsigned long flags;

int __is_unlock_match(rotlock_t *lock1, int type, int degree, int range, int pid)
{
	if (
		(lock1->type == type) &&
		(lock1->degree == degree) &&
		(lock1->range == range) &&
		(lock1->pid == pid)
	) {
		return 1;
	} else {
		return 0;
	}
}

int __is_range_contains_rotation(int degree, int range, int rotation)
{
	int a = degree - range;
	int b = degree + range;

	int rot_minus = rotation - 360;
	int rot_plus = rotation + 360;

	if (
		(a < rotation && rotation < b) ||
		(a < rot_minus && rot_minus < b) ||
		(a < rot_plus && rot_plus < b)
	)
		return 1;
	else
		return 0;
}

int __is_not_overlapped(int deg1, int rang1, int deg2, int rang2)
{
	int a = (deg1 - rang1) < 0 ? (deg1 - rang1) + 360 : (deg1 - rang1);
	int b = (deg1 + rang1) % 360;
	int c = (deg2 - rang2) < 0 ? (deg2 - rang2) + 360 : (deg2 - rang2);
	int d = (deg2 + rang2) % 360;

	return (
		(a < b) &&
		(
			(c < d && ((c < a && d < a) || (c > a && d > a))) ||
			(c > d && (c > b && d < a))
		)
	) || (
		(a > b) &&
		(c < d && (c > b && d < a))
	);
}

int __is_waiting_lock_acquirable_compare_to_acquired_lock(rotlock_t *p_waiting_lock, rotlock_t *p_acquired_lock)
{
	/*
	 * Check whether degree/range of waiting_lock_node overlaps with that of acquired_lock
	 * if not overlapped -> 1 , else -> 0
	 * Regarding Reader Case, reader should look at acquired_lock_list if there is writer lock which has same degree/range
	 * Writer Case You don't need to
	 */

	//Reader Case
	if (p_waiting_lock->type == 0) {
		if (p_acquired_lock->type == 0) {
			return 1; // 둘다 read 일 경우
		} else { // waiting read, aquired write
			return __is_not_overlapped(
				p_waiting_lock->degree, p_waiting_lock->range,
				p_acquired_lock->degree, p_acquired_lock->range
			);
		}
	} else { //Writer Case
		return __is_not_overlapped(
			p_waiting_lock->degree, p_waiting_lock->range,
			p_acquired_lock->degree, p_acquired_lock->range
		);
	}
};

// for iteration
rotlock_t *p_lock;
rotlock_t *p_temp_lock;

int list_add_pending(rotlock_t *p_pending_lock)
{
	spin_lock_irqsave(&list_iteration_spin_lock, flags);

	list_add_tail(&(p_pending_lock->list_node), &pending_lh);

	spin_unlock_irqrestore(&list_iteration_spin_lock, flags);
	return 0;
}

// reconstruct pending_lh, wait_read_lh, wait_write_lh
int refresh_pending_waiting_lists(void)
{
	spin_lock_irqsave(&list_iteration_spin_lock, flags);

	list_for_each_entry_safe(p_lock, p_temp_lock, &pending_lh, list_node) {
		// range 가 포함하면, read / write
		if (__is_range_contains_rotation(p_lock->degree, p_lock->range, rotation.degree)) {
			if (p_lock->type == READ_LOCK) {
				list_move_tail(&(p_lock->list_node), &wait_read_lh);
				p_lock->status = WAIT_READ;
			} else if (p_lock->type == WRITE_LOCK) {
				list_move_tail(&(p_lock->list_node), &wait_write_lh);
				p_lock->status = WAIT_WRITE;
			} else {
				pr_debug("[soo] refresh_pending_waiting_lists: invalid type");
			}
		}
	}
	list_for_each_entry_safe(p_lock, p_temp_lock, &wait_read_lh, list_node) {
		if (!__is_range_contains_rotation(p_lock->degree, p_lock->range, rotation.degree)) {
			list_move_tail(&(p_lock->list_node), &pending_lh);
			p_lock->status = PENDING;
		}
	}
	list_for_each_entry_safe(p_lock, p_temp_lock, &wait_write_lh, list_node) {
		if (!__is_range_contains_rotation(p_lock->degree, p_lock->range, rotation.degree)) {
			list_move_tail(&(p_lock->list_node), &pending_lh);
			p_lock->status = PENDING;
		}
	}

	spin_unlock_irqrestore(&list_iteration_spin_lock, flags);
	return 0;
};

rotlock_t *p_waiting_lock;
rotlock_t *p_waiting_temp_lock;
rotlock_t *p_acquired_lock;
rotlock_t *p_acquired_safe_lock;

int wait_write_to_acquire(void)
{
	spin_lock_irqsave(&list_iteration_spin_lock, flags);

	list_for_each_entry_safe(p_waiting_lock, p_waiting_temp_lock, &wait_write_lh, list_node) {
		// 어콰이어 리스트를 다 돌고도 문제가 없으면 넣어줘야. 하나하나와 비교하는거 아님. flag방식.
		int is_acquirable = 1;

		list_for_each_entry_safe(p_acquired_lock, p_acquired_safe_lock, &acquired_lh, list_node) {
			if (!__is_waiting_lock_acquirable_compare_to_acquired_lock(p_waiting_lock, p_acquired_lock)) {
				is_acquirable = 0;
				break;
			}
		}
		if (is_acquirable) {
			list_move_tail(&(p_waiting_lock->list_node), &acquired_lh);
			p_waiting_lock->status = ACQUIRED;
			break; // FIXME 필요한가? wait_write_lh 에서 여러개가 acquired 될 경우는 없나?
		}
	}

	spin_unlock_irqrestore(&list_iteration_spin_lock, flags);
	return 0;
};

int wait_read_to_acquire(void)
{
	spin_lock_irqsave(&list_iteration_spin_lock, flags);

	if (list_empty(&wait_write_lh)) {
		list_for_each_entry_safe(p_waiting_lock, p_waiting_temp_lock, &wait_read_lh, list_node) {
			// 어콰이어 리스트를 다 돌고도 문제가 없으면 넣어줘야. 하나하나와 비교하는거 아님. flag방식.
			int is_acquirable = 1;

			list_for_each_entry_safe(p_acquired_lock, p_acquired_safe_lock, &acquired_lh, list_node) {
				if (!__is_waiting_lock_acquirable_compare_to_acquired_lock(p_waiting_lock, p_acquired_lock)) {
					is_acquirable = 0;
					break;
				}
			}
			if (is_acquirable) {
				list_move_tail(&(p_waiting_lock->list_node), &acquired_lh);
				p_waiting_lock->status = ACQUIRED;
				// break; // FIXME 필요한가? wait_write_lh 에서 여러개가 acquired 될 경우는 없나?
			}
		}
	}

	spin_unlock_irqrestore(&list_iteration_spin_lock, flags);
	return 0;
};

int delete_lock(int type, int degree, int range, int pid)
{
	int is_deleted;

	spin_lock_irqsave(&list_iteration_spin_lock, flags);

	is_deleted = 0;

	if (!is_deleted) {
		list_for_each_entry_safe_reverse(p_lock, p_temp_lock, &pending_lh, list_node) {
			if (__is_unlock_match(p_lock, type, degree, range, pid)) {
				list_del(&(p_lock->list_node)); // 끝내야. flag 를 쓰던가.
				kfree(p_lock);
				is_deleted = 1;
			}
		}
	}

	if (!is_deleted) {
		list_for_each_entry_safe_reverse(p_lock, p_temp_lock, &wait_read_lh, list_node) {
			if (__is_unlock_match(p_lock, type, degree, range, pid)) {
				list_del(&(p_lock->list_node));
				kfree(p_lock);
				is_deleted = 1;
			}
		}
	}

	if (!is_deleted) {
		list_for_each_entry_safe_reverse(p_lock, p_temp_lock, &wait_write_lh, list_node) {
			if (__is_unlock_match(p_lock, type, degree, range, pid)) {
				list_del(&(p_lock->list_node));
				kfree(p_lock);
				is_deleted = 1;
			}
		}
	}

	if (!is_deleted) {
		list_for_each_entry_safe_reverse(p_lock, p_temp_lock, &acquired_lh, list_node) {
			if (__is_unlock_match(p_lock, type, degree, range, pid)) {
				list_del(&(p_lock->list_node));
				kfree(p_lock);
				is_deleted = 1;
			}
		}
	}

	spin_unlock_irqrestore(&list_iteration_spin_lock, flags);
	return 0;
}

void __print_all_lists(void)
{
	spin_lock_irqsave(&list_iteration_spin_lock, flags);

	list_for_each_entry_safe(p_lock, p_temp_lock, &pending_lh, list_node) {
		pr_debug("[soo] pending_lh: %d, %d, %d, %d, %d, %p, %p, %p\n",
		p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, p_lock->status,
		&(p_lock->list_node), p_lock->list_node.next, p_lock->list_node.prev);
	}

	list_for_each_entry_safe(p_lock, p_temp_lock, &wait_read_lh, list_node) {
		pr_debug("[soo] wait_read_lh: %d, %d, %d, %d, %d, %p, %p, %p\n",
		p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, p_lock->status,
		&(p_lock->list_node), p_lock->list_node.next, p_lock->list_node.prev);
	}

	list_for_each_entry_safe(p_lock, p_temp_lock, &wait_write_lh, list_node) {
		pr_debug("[soo] wait_write_lh: %d, %d, %d, %d, %d, %p, %p, %p\n",
		p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, p_lock->status,
		&(p_lock->list_node), p_lock->list_node.next, p_lock->list_node.prev);
	}

	list_for_each_entry_safe(p_lock, p_temp_lock, &acquired_lh, list_node) {
		pr_debug("[soo] acquired_lh: %d, %d, %d, %d, %d, %p, %p, %p\n",
		p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, p_lock->status,
		&(p_lock->list_node), p_lock->list_node.next, p_lock->list_node.prev);
	}

	spin_unlock_irqrestore(&list_iteration_spin_lock, flags);
}

// int exit_rotlock(pid_t pid) {
// return 0;
// }; // kernel/exit.c 의 do_exit() 안에 insert.
