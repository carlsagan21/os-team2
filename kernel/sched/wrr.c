#include <linux/kernel.h>
#include <linux/sched.h>

#include "sched.h"

/*rt
 * Adding/removing a task to/from a priority array:
 */
/*fair
 * The enqueue_task method is called before nr_running is
 * increased. Here we update the fair scheduling stats and
 * then put the task into the rbtree:
 */
static void
enqueue_task_wrr(struct rq *rq, struct task_struct *p, int flags)
{
	printk(KERN_DEBUG "enqueue_task_wrr");
}

/*fair
 * The dequeue_task method is called before nr_running is
 * decreased. We remove the task from the rbtree and
 * update the fair scheduling stats:
 */
static void dequeue_task_wrr(struct rq *rq, struct task_struct *p, int flags)
{
	printk(KERN_DEBUG "dequeue_task_wrr");
}

/*fair
 * sched_yield() is very simple
 *
 * The magic of dealing with the ->skip buddy is in pick_next_entity.
 */
static void yield_task_wrr(struct rq *rq)
{
	printk(KERN_DEBUG "yield_task_wrr");
}

static bool yield_to_task_wrr(struct rq *rq, struct task_struct *p, bool preempt)
{
	printk(KERN_DEBUG "yield_to_task_wrr");
	return false;
}

/*rt, fair
 * Preempt the current task with a newly woken task if needed:
 */
static void check_preempt_curr_wrr(struct rq *rq, struct task_struct *p, int flags)
{
	printk(KERN_DEBUG "check_preempt_curr_wrr");
}

static struct task_struct *pick_next_task_wrr(struct rq *rq)
{
	printk(KERN_DEBUG "pick_next_task_wrr");
	return NULL;
}

/*fair
 * Account for a descheduled task:
 */
static void put_prev_task_wrr(struct rq *rq, struct task_struct *prev)
{
	printk(KERN_DEBUG "put_prev_task_wrr");
}

/*fair
 * sched_balance_self: balance the current task (running on cpu) in domains
 * that have the 'flag' flag set. In practice, this is SD_BALANCE_FORK and
 * SD_BALANCE_EXEC.
 *
 * Balance, ie. select the least loaded group.
 *
 * Returns the target CPU number, or the same CPU if no balancing is needed.
 *
 * preempt must be disabled.
 */
static int
select_task_rq_wrr(struct task_struct *p, int sd_flag, int wake_flags)
{
	printk(KERN_DEBUG "select_task_rq_wrr");
	return 0;
}

/*fair
 * Called immediately before a task is migrated to a new cpu; task_cpu(p) and
 * cfs_rq_of(p) references at time of call are still valid and identify the
 * previous cpu.  However, the caller only guarantees p->pi_lock is held; no
 * other assumptions, including the state of rq->lock, should be made.
 */
static void
migrate_task_rq_wrr(struct task_struct *p, int next_cpu)
{
	printk(KERN_DEBUG "migrate_task_rq_wrr");
}

static void pre_schedule_wrr(struct rq *rq, struct task_struct *prev)
{
	printk(KERN_DEBUG "pre_schedule_wrr");
}

static void post_schedule_wrr(struct rq *rq)
{
	printk(KERN_DEBUG "post_schedule_wrr");
}

static void task_waking_wrr(struct task_struct *p)
{
	printk(KERN_DEBUG "task_waking_wrr");
}

/*rt
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
static void task_woken_wrr(struct rq *rq, struct task_struct *p)
{
	printk(KERN_DEBUG "task_woken_wrr");
}

static void set_cpus_allowed_wrr(struct task_struct *p,
				const struct cpumask *new_mask)
{
	printk(KERN_DEBUG "set_cpus_allowed_wrr");
}

/*rt Assumes rq->lock is held */
static void rq_online_wrr(struct rq *rq)
{
	printk(KERN_DEBUG "rq_online_wrr");
}

/*rt Assumes rq->lock is held */
static void rq_offline_wrr(struct rq *rq)
{
	printk(KERN_DEBUG "rq_offline_wrr");
}

/*fair Account for a task changing its policy or group.
 *
 * This routine is mostly called to set cfs_rq->curr field when a task
 * migrates between groups/classes.
 */
static void set_curr_task_wrr(struct rq *rq)
{
	printk(KERN_DEBUG "set_curr_task_wrr");
}

/*fair
 * scheduler tick hitting a task of our scheduling class:
 */
static void task_tick_wrr(struct rq *rq, struct task_struct *curr, int queued)
{
	printk(KERN_DEBUG "task_tick_wrr");
}

/*fair
 * called on fork with the child task as argument from the parent's context
 *  - child not yet on the tasklist
 *  - preemption disabled
 */
static void task_fork_wrr(struct task_struct *p)
{
	printk(KERN_DEBUG "task_fork_wrr");
}

/*rt
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
static void switched_from_wrr(struct rq *rq, struct task_struct *p)
{
	printk(KERN_DEBUG "switched_from_wrr");
}

/*rt
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
/*fair
 * We switched to the sched_fair class.
 */
static void switched_to_wrr(struct rq *rq, struct task_struct *p)
{
	printk(KERN_DEBUG "switched_to_wrr");
}

/*rt
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
/*fair
 * Priority of the task has changed. Check to see if we preempt
 * the current task.
 */
static void
prio_changed_wrr(struct rq *rq, struct task_struct *p, int oldprio)
{
	printk(KERN_DEBUG "prio_changed_wrr");
}

static unsigned int get_rr_interval_wrr(struct rq *rq, struct task_struct *task)
{
	printk(KERN_DEBUG "get_rr_interval_wrr");
	return 0;
}

static void task_move_group_wrr(struct task_struct *p, int on_rq)
{
	printk(KERN_DEBUG "task_move_group_wrr");
}


const struct sched_class wrr_sched_class = {
	//	const struct sched_class *next;
	.next			= &fair_sched_class, // fair 여야 함. rt.c 를 wrr 로 바꾸고.

	//	void (*enqueue_task) (struct rq *rq, struct task_struct *p, int flags);
	.enqueue_task		= enqueue_task_wrr,
	//	void (*dequeue_task) (struct rq *rq, struct task_struct *p, int flags);
	.dequeue_task		= dequeue_task_wrr,
	//	void (*yield_task) (struct rq *rq);
	.yield_task		= yield_task_wrr,
	//	bool (*yield_to_task) (struct rq *rq, struct task_struct *p, bool preempt);
	// yield_to_task가 fair 에는 있고 rt 에는 없음.
	.yield_to_task		= yield_to_task_wrr,

	//	void (*check_preempt_curr) (struct rq *rq, struct task_struct *p, int flags);
	.check_preempt_curr	= check_preempt_curr_wrr,

  //	struct task_struct * (*pick_next_task) (struct rq *rq);
	.pick_next_task		= pick_next_task_wrr,
	//	void (*put_prev_task) (struct rq *rq, struct task_struct *p);
	.put_prev_task		= put_prev_task_wrr,

#ifdef CONFIG_SMP
	//	int  (*select_task_rq)(struct task_struct *p, int sd_flag, int flags);
	.select_task_rq		= select_task_rq_wrr,
	//	void (*migrate_task_rq)(struct task_struct *p, int next_cpu);
	// fair only
	.migrate_task_rq = migrate_task_rq_wrr,

	//	void (*pre_schedule) (struct rq *this_rq, struct task_struct *task);
	// rt only
	.pre_schedule		= pre_schedule_wrr,
	//	void (*post_schedule) (struct rq *this_rq);
	.post_schedule		= post_schedule_wrr,
	//	void (*task_waking) (struct task_struct *task);
	// fair only
	.task_waking = task_waking_wrr,
	//	void (*task_woken) (struct rq *this_rq, struct task_struct *task);
	// rt only
	.task_woken		= task_woken_wrr,

	//	void (*set_cpus_allowed)(struct task_struct *p,
	//				 const struct cpumask *newmask);
	// rt only
	.set_cpus_allowed       = set_cpus_allowed_wrr,

	//	void (*rq_online)(struct rq *rq);
	.rq_online              = rq_online_wrr,
	//	void (*rq_offline)(struct rq *rq);
	.rq_offline             = rq_offline_wrr,
#endif
	//	void (*set_curr_task) (struct rq *rq);
	.set_curr_task          = set_curr_task_wrr,
	//	void (*task_tick) (struct rq *rq, struct task_struct *p, int queued);
	.task_tick		= task_tick_wrr,
	//	void (*task_fork) (struct task_struct *p);
	// fair only
	.task_fork = task_fork_wrr,

	//	void (*switched_from) (struct rq *this_rq, struct task_struct *task);
	// 정의에는 SMP 바깥. fair 에도 바깥. rt 에는 안.
	.switched_from		= switched_from_wrr,
	//	void (*switched_to) (struct rq *this_rq, struct task_struct *task);
	// fair only
	.switched_to		= switched_to_wrr,
	//	void (*prio_changed) (struct rq *this_rq, struct task_struct *task,
	//			     int oldprio);
	.prio_changed		= prio_changed_wrr,

	//	unsigned int (*get_rr_interval) (struct rq *rq,
	//					 struct task_struct *task);
	.get_rr_interval	= get_rr_interval_wrr,


#ifdef CONFIG_FAIR_GROUP_SCHED
	//	void (*task_move_group) (struct task_struct *p, int on_rq);
	// fair only
	.task_move_group = task_move_group_wrr
#endif
};
