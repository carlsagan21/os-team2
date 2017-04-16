#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/slab.h>

/**
 * rotlock_read 주석 참고.
 */
int sys_rotlock_write(int degree, int range)
{
	rotlock_t *p_new_lock;

	pr_debug("[soo] sys_rotlock_write\n");

	p_new_lock = kmalloc(sizeof(rotlock_t), GFP_KERNEL);
	if (p_new_lock == NULL)
		return -ENOMEM;

	p_new_lock->type = WRITE_LOCK;
	p_new_lock->degree = degree;
	p_new_lock->range = range;
	p_new_lock->pid = task_pid_nr(current);
	p_new_lock->status = PENDING;

	mutex_lock(&rotlock_mutex);

	list_add_pending(p_new_lock);
	refresh_pending_waiting_lists();
	wait_write_to_acquire();
	wait_read_to_acquire();

	__print_all_lists();
	mutex_unlock(&rotlock_mutex);

	pr_debug("[soo] p_lock status: %d\n", p_new_lock->status);

	wait_event_interruptible(wq_rotlock, p_new_lock->status == ACQUIRED);

	// if (is_rotlock_deleted(p_new_lock)) {
	//	kfree(p_new_lock);
	// }

	return 0;
};
