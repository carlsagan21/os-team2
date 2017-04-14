// #include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/sched.h>
#include <linux/list.h>
// #include <linux/cred.h> /* for task_uid() */
// #include <linux/uaccess.h> /* for copy_from_user, copy_to_user, strncpy */
// #include <linux/errno.h>

/*
 * Release a read/or write lock using the given rotation range
 * returning 0 on success, -1 on failure.
 * system call numbers 383 and 384
 */
int sys_rotunlock_read(int degree, int range) /* 0 <= degree < 360 , 0 < range < 180 */
{
	int pid;

	pr_debug("[soo] sys_rotunlock_read\n");

	pid = task_pid_nr(current);

	mutex_lock(&rotlock_mutex); // kill, interrupt 를 막아버림.
	delete_lock(READ_LOCK, degree, range, pid);
	refresh_pending_waiting_lists();
	wait_write_to_acquire();
	wait_read_to_acquire();

	__print_all_lists();
	mutex_unlock(&rotlock_mutex);
	// TODO 새롭게 acquired 된 것들이 있으면 wakeup
	pr_debug("[soo] wake up all\n");

	return 0;
};
