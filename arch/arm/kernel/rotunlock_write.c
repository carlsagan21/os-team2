#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/sched.h>
#include <linux/list.h>

/*
 * rotunlock_read 주석 참고.
 */
int sys_rotunlock_write(int degree, int range)
{
	int pid;
	// rotlock_t *p_deleted_rotlock;

	pr_debug("[soo] sys_rotunlock_write\n");

	pid = task_pid_nr(current);

	mutex_lock(&rotlock_mutex);
	// p_deleted_rotlock = delete_lock(WRITE_LOCK, degree, range, pid);
	delete_lock(WRITE_LOCK, degree, range, pid);
	refresh_pending_waiting_lists();
	wait_write_to_acquire();
	wait_read_to_acquire();

	__print_all_lists();
	mutex_unlock(&rotlock_mutex);

	pr_debug("[soo] wake up all\n");

	// if (is_rotlock_deleted(p_deleted_rotlock)) {
	// 	kfree(p_deleted_rotlock);
	// }

	wake_up_interruptible_all(&wq_rotlock);

	return 0;
};
