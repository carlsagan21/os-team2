#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/sched.h>
#include <linux/list.h>

/*
 * Release a read/or write lock using the given rotation range
 * returning 0 on success, -1 on failure.
 * system call numbers 383 and 384
 */
int sys_rotunlock_read(int degree, int range) /* 0 <= degree < 360 , 0 < range < 180 */
{
	int pid;
	// rotlock_t *p_deleted_rotlock;

	pr_debug("[soo] sys_rotunlock_read\n");

	pid = task_pid_nr(current);

	rotunlock(READ_LOCK, degree, range, pid);

	return 0;
};
