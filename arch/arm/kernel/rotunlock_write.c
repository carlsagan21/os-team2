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

	rotunlock(WRITE_LOCK, degree, range, pid);

	return 0;
};
