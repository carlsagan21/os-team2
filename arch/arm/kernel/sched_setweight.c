#include <linux/sched.h>
#include <linux/kernel.h>
// #include <linux/sched.h>
 #include <linux/slab.h>
// #include <sched/sched.h>

/*
 * Set the SCHED_WRR weight of process, as identified by 'pid'.
 * If 'pid' is 0, set the weight for the calling process.
 * System call number 380.
 */

int sys_sched_setweight(pid_t pid, int weight){
  printk(KERN_DEBUG "[os-team2] sched_setweight");

	struct task_struct *p;
	int delta;
	struct rq *rq;
	// kuid_t root_uid = KUIDT_INIT(0);

	if (weight < 1 || weight > 20) {
		return -EINVAL;
	}

	if (pid == 0) {
		/* set calling process weight */
		p = current;
	} else {
		// if (!uid_eq(current->cred->euid, root_uid)) {
		// 	return -EINVAL;
		// }
		p = pid_task(find_vpid(pid), PIDTYPE_PID);
		if (p == NULL) {
			return -EINVAL;
		}
	}

	if (p->policy != SCHED_WRR)
		return -EINVAL;

	delta = p->wrr_se.weight - weight;
	// if (!uid_eq(current->cred->euid, root_uid) && delta < 0) {
	// 	return -EINVAL;
	// }

	p->wrr_se.weight = weight;
	// rq = task_rq(p);
	// rq->wrr.total_weight -= delta;

	return 0;
};
