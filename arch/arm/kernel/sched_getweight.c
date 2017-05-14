#include <linux/sched.h>
#include <linux/kernel.h>
// #include <linux/sched.h>
 #include <linux/slab.h>
// #include <sched/sched.h>

/*
 * Obtain the SCHED_WRR weight of a process as identified by 'pid'.
 * If 'pid' is 0, return the weight of the calling process.
 * System call number 381.
 */

unsigned int sys_sched_getweight(pid_t pid){
  printk("[os-team2] sched_getweight");
	struct task_struct *p;

	if (pid == 0) {
		p = current;
	} else {
		p = pid_task(find_vpid(pid), PIDTYPE_PID);
		if (p == NULL) {
			return -EINVAL;
		}
	}
	if (p->policy != SCHED_WRR) {
		return -EINVAL;
	}

	return p->wrr_se.weight;
};
