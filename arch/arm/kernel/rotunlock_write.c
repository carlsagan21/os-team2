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
int sys_rotunlock_write(int degree, int range) /* degree - range <= LOCK RANGE <= degree + range */
{
	// rotlock_t *p_lock;
	// rotlock_t *p_temp_lock;
	int pid;

	pr_debug("[soo] sys_rotunlock_write\n");

	pid = task_pid_nr(current);

	delete_lock(WRITE_LOCK, degree, range, pid);

	refresh_pending_waiting_lists();
	wait_write_to_acquire();
	wait_read_to_acquire();

	__print_all_lists();

	return 0;
};
