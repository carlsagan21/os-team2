#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/list.h>
#include <linux/slab.h> /* for kmalloc(), GFP_KERNEL. linux/gfp 따로 include 안해도 됨. */
#include <linux/spinlock.h>
#include <linux/rwlock_types.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/sched.h>

/**
 * extern rotation impl.
 */
rotation_t rotation;

/**
 * extern list heads impl.
 * 이니셜라이즈까지 매크로로 됩니다.
 */
LIST_HEAD(pending_lh);
LIST_HEAD(wait_read_lh);
LIST_HEAD(wait_write_lh);
LIST_HEAD(acquired_lh);

/**
 * extern spinlock impl.
 * 리스트 이터레이션 및 write 일 때 사용됩니다.
 * list_add_pending, refresh_pending_waiting_lists, wait_write_to_acquire, wait_read_to_acquire 에서 사용합니다.
 */
DEFINE_SPINLOCK(list_iteration_spin_lock);

/**
 * a flag for spin_unlock_irqsave/restore
 */
unsigned long spin_lock_flags;

/**
 * extern mutex impl.
 * 리스트 이터레이션 함수들을 묶어주기 위해 시스템콜에서 직접 호출됩니다.
 * 구조적으로
 * mutex.lock()
 *   spinlock.lock()
 *   spinlock.unlock()
 *   spinlock.lock()
 *   spinlock.unlock()
 *   ...
 * mutex.unlock()
 * 이 됩니다.
 */
DEFINE_MUTEX(rotlock_mutex);

/**
 * extern rwlock impl.
 * 리스트에 접근할떄 사용되는 락입니다.
 * 리스트를 read 만 하는 경우에 있어서 readlock 이 사용되고 있습니다.
 * list_iteration_spin_lock을 써도 되지만, read에 한정되는 경우들이기도 하고,
 * 실제적올 deprecated 를 제외하면 디버깅용 함수에만 쓰이고 있어서 공부도 할 겸 집어넣었습니다.
 */
DEFINE_RWLOCK(list_iteration_rwlock);

/**
 * rwlock flags
 */
unsigned long rwlock_flags;

/**
 * extern wait queue impl.
 */
DECLARE_WAIT_QUEUE_HEAD(wq_rotlock);

/**
 * for each entry 로 이터레이션을 할때 사용되는 임시값들입니다.
 */
rotlock_t *p_lock;
rotlock_t *p_temp_lock;
rotlock_t *p_waiting_lock;
rotlock_t *p_waiting_temp_lock;
rotlock_t *p_acquired_lock;
rotlock_t *p_acquired_safe_lock;

/**
 * unlock 호출 시, 호출에서 사용된 type, degree, range, pid 가 저장되어있는 락과 일치하는지 확인합니다.
 * 일치하면 1을, 아니면 0을 리턴합니다.
 */
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

/**
 * 특정 degree, range의 조합이 rotation 을 포함하는지 판단합니다. pending -> wait 판단에 사용됩니다.
 */
int __is_range_contains_rotation(int degree, int range, int rotation)
{
	int a = degree - range;
	int b = degree + range;

	int rot_minus = rotation - 360;
	int rot_plus = rotation + 360;

	if (
		(a <= rotation && rotation <= b) ||
		(a <= rot_minus && rot_minus <= b) ||
		(a <= rot_plus && rot_plus <= b)
	)
		return 1;
	else
		return 0;
}

/**
 * 두 범위가 겹치지 않는지 여부를 판단합니다. wait -> acquire 판단에 사용됩니다.
 */
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

/**
 * wait 리스트에 있는 특정 락이, 현재 어콰이어 되어있는 락들에 대비하여 어콰이어가 가능한지를 판단합니다.
 * read/write 락의 경우들을 모두 포괄합니다.
 * Check whether degree/range of waiting_lock_node overlaps with that of acquired_lock
 * if not overlapped -> 1 , else -> 0
 * Regarding Reader Case, reader should look at acquired_lock_list if there is writer lock which has same degree/range
 * Writer Case You don't need to
 */
int __is_waiting_lock_acquirable_compare_to_acquired_lock(rotlock_t *p_waiting_lock, rotlock_t *p_acquired_lock)
{
	// Reader Case
	if (p_waiting_lock->type == READ_LOCK) {
		if (p_acquired_lock->type == READ_LOCK) {
			return 1; // 둘다 read 일 경우
		} else { // waiting read, aquired write
			return __is_not_overlapped(
				p_waiting_lock->degree, p_waiting_lock->range,
				p_acquired_lock->degree, p_acquired_lock->range
			);
		}
	} else { // Writer Case
		return __is_not_overlapped(
			p_waiting_lock->degree, p_waiting_lock->range,
			p_acquired_lock->degree, p_acquired_lock->range
		);
	}
};

int is_acquirable(rotlock_t *p_lock)
{
	int acquirable;

	if (!__is_range_contains_rotation(p_lock->degree, p_lock->range, rotation.degree))
		return 0;

	// refresh list 과정이 전체적으로 atomic 하다는 가정 위에서 작동. wait_write_lh 의 상태가 atomic 해야. mutex 필수가 됨.
	if (!list_empty(&wait_write_lh) && p_lock->type == READ_LOCK)
		return 0;

	read_lock_irqsave(&list_iteration_rwlock, rwlock_flags);
	// 어콰이어 리스트를 다 돌고도 문제가 없으면 넣어줘야. 하나하나와 비교하는거 아님. flag방식.
	acquirable = 1;

	list_for_each_entry_safe(p_acquired_lock, p_acquired_safe_lock, &acquired_lh, list_node) {
		if (!__is_waiting_lock_acquirable_compare_to_acquired_lock(p_lock, p_acquired_lock)) {
			acquirable = 0;
			break;
			// 바로 리턴하는 방식이 아니라 flag 를 쓰는 것은, lock 을 보다 명확하게 풀어주기 위함.
		}
	}

	read_unlock_irqrestore(&list_iteration_rwlock, rwlock_flags);
	return acquirable;
}

int list_add_pending(rotlock_t *p_pending_lock)
{
	spin_lock_irqsave(&list_iteration_spin_lock, spin_lock_flags);

	list_add_tail(&(p_pending_lock->list_node), &pending_lh);

	spin_unlock_irqrestore(&list_iteration_spin_lock, spin_lock_flags);
	return 0;
}

// reconstruct pending_lh, wait_read_lh, wait_write_lh
int refresh_pending_waiting_lists(void)
{
	spin_lock_irqsave(&list_iteration_spin_lock, spin_lock_flags);

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

	spin_unlock_irqrestore(&list_iteration_spin_lock, spin_lock_flags);
	return 0;
};

int wait_write_to_acquire(void)
{
	spin_lock_irqsave(&list_iteration_spin_lock, spin_lock_flags);

	list_for_each_entry_safe(p_waiting_lock, p_waiting_temp_lock, &wait_write_lh, list_node) {
		if (is_acquirable(p_waiting_lock)) {
			list_move_tail(&(p_waiting_lock->list_node), &acquired_lh);
			p_waiting_lock->status = ACQUIRED;
			break; // 하나만
		}
	}

	spin_unlock_irqrestore(&list_iteration_spin_lock, spin_lock_flags);
	return 0;
};

int wait_read_to_acquire(void)
{
	spin_lock_irqsave(&list_iteration_spin_lock, spin_lock_flags);

	if (list_empty(&wait_write_lh)) {
		list_for_each_entry_safe(p_waiting_lock, p_waiting_temp_lock, &wait_read_lh, list_node) {
			if (is_acquirable(p_waiting_lock)) {
				list_move_tail(&(p_waiting_lock->list_node), &acquired_lh);
				p_waiting_lock->status = ACQUIRED;
				// break; 여럿가능
			}
		}
	}

	spin_unlock_irqrestore(&list_iteration_spin_lock, spin_lock_flags);
	return 0;
};

int delete_lock(int type, int degree, int range, int pid)
{
	int is_deleted;

	spin_lock_irqsave(&list_iteration_spin_lock, spin_lock_flags);

	is_deleted = 0;

	if (!is_deleted) {
		list_for_each_entry_safe_reverse(p_lock, p_temp_lock, &pending_lh, list_node) {
			if (__is_unlock_match(p_lock, type, degree, range, pid)) {
				list_del(&(p_lock->list_node));
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

	spin_unlock_irqrestore(&list_iteration_spin_lock, spin_lock_flags);
	return is_deleted;
}

void __print_all_lists(void)
{
	read_lock_irqsave(&list_iteration_rwlock, rwlock_flags);

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

	read_unlock_irqrestore(&list_iteration_rwlock, rwlock_flags);
}

rotlock_t *__find_rotlock_by_pid(pid_t pid)
{
	int is_rotlock_found;
	rotlock_t *p_found_lock = NULL;
	spin_lock_irqsave(&list_iteration_spin_lock, spin_lock_flags);

	// flag for finding process with same pid, 0 => false 1 => true
	is_rotlock_found = 0;

	if (is_rotlock_found != 1) {
		list_for_each_entry_safe(p_lock, p_temp_lock, &pending_lh, list_node) {
			if (p_lock->pid == pid) {
				is_rotlock_found = 1;
				p_found_lock = p_lock;
				break;
			}
		}
	}

	
	if (is_rotlock_found != 1) {
		list_for_each_entry_safe(p_lock, p_temp_lock, &wait_read_lh, list_node) {
			if(p_lock->pid == pid) {
				is_rotlock_found = 1;
				p_found_lock = p_lock;
				break;
			}
		}
	}
	
	if (is_rotlock_found != 1) {
		list_for_each_entry_safe(p_lock, p_temp_lock, &wait_write_lh, list_node) {
			if(p_lock->pid == pid) {
				is_rotlock_found = 1;
				p_found_lock = p_lock;
				break;
			}
		}
	}

	if (is_rotlock_found != 1) {
		list_for_each_entry_safe(p_lock, p_temp_lock, &acquired_lh, list_node) {
			if(p_lock->pid == pid) {
				is_rotlock_found = 1;
				p_found_lock = p_lock;
				break;
			}
		}
	}

	spin_unlock_irqrestore(&list_iteration_spin_lock, spin_lock_flags);
	return p_found_lock;
}

wait_queue_t *__find_wait_queue(wait_queue_head_t *q, pid_t pid)
{
	wait_queue_t *p_wq;
	wait_queue_t *p_temp_wq;
	int is_wq_found = 0;
	wait_queue_t *p_found_wq = NULL;
	unsigned long flags;
	
	spin_lock_irqsave(&q->lock, flags);

	list_for_each_entry_safe(p_wq, p_temp_wq, &(q->task_list), task_list) {
		struct task_struct *p_task = (struct task_struct *)p_wq->private;
		if (p_task->pid == pid) {
			is_wq_found = 1;
			p_found_wq = p_wq;
			break;
		}
	}

	spin_unlock_irqrestore(&q->lock, flags);
	// if not found(No possiblity), return NULL
	// actually higly possible
	return p_found_wq;
}

int __sync_remove_thread_from_waiting_queue_and_delete_lock(rotlock_t *target) {
	// TODO remove_wait_queue, rotunlock 순서
	wait_queue_t *p_wait_queue_to_be_removed = NULL;
	mutex_lock(&rotlock_mutex);

	delete_lock(target->type, target->degree, target->range, target->pid);
	// refresh_pending_waiting_lists();
	// wait_write_to_acquire();
	// wait_read_to_acquire();

	__print_all_lists();
	mutex_unlock(&rotlock_mutex);

	p_wait_queue_to_be_removed = __find_wait_queue(&wq_rotlock, target->pid);
	// 여기에서 현재 삭제되려는 프로세스가 이미 어콰이어되어서 없을 가능성이 있을 수 있을 것 같은데.
	// do_exit() 이 인터럽트 방지가 되어있나?
	remove_wait_queue(&wq_rotlock, p_wait_queue_to_be_removed);
	
	/**
	 * write starvation 방지 상황이었으면, delete_lock으로 인해 어콰이어 하는 리더가 생깁니다.
	 * 리스트에서도 삭제되었고, 웨이트 큐에서도 빠졌으므로 웨이팅 하고 있는 것들을 다 깨워줍니다.
	 */
	wake_up_interruptible_all(&wq_rotlock);
	
	return 0;
}

int rotunlock(int type, int degree, int range, int pid)
{
	/*
	 * mutually exclusive 한 리스트 수정.
	 * kill, interrupt 를 막음.
	 * 리스트에서 찾아서 삭제하고, 리스트를 재조정함.
	 */
	mutex_lock(&rotlock_mutex);

	delete_lock(type, degree, range, pid);
	refresh_pending_waiting_lists();
	wait_write_to_acquire();
	wait_read_to_acquire();

	__print_all_lists();
	mutex_unlock(&rotlock_mutex);

	pr_debug("[soo] wake up all\n");

	// if (is_rotlock_deleted(p_deleted_rotlock)) {
	//	kfree(p_deleted_rotlock);
	// }

	/**
	 * wait_event_interruptible로 인해 자고있는 interruped 된 것들을 모두 깨움. 깨워서 컨디션 체크 유도.
	 * 새롭게 acquired 된 것들이 있으면 wakeup.
	 */
	wake_up_interruptible_all(&wq_rotlock);

	return 0;
};


int __handle_killed_process_lock(rotlock_t *target)
{
	switch (target->status) {
		case ACQUIRED:
			rotunlock(target->type, target->degree, target->range, target->pid);
			break;
		case WAIT_READ:
		case WAIT_WRITE:
		case PENDING:
			__sync_remove_thread_from_waiting_queue_and_delete_lock(target);
			break;
		default:
			break;
	}

	return 0;
}

/**
 * kernel/exit.c 의 do_exit() 안에 insert.
 * 스레드가 종료될 경우, 혹시 리스트에 남아있거나 웨이팅큐에 남아있는지 확인하고 처리해줍니다.
 */
int exit_rotlock(pid_t pid)
{
	rotlock_t *target = __find_rotlock_by_pid(pid);
	if (target != NULL)
		__handle_killed_process_lock(target);
	
	return 0;
}
