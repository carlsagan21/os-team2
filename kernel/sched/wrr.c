#include <linux/kernel.h>
#include <linux/sched.h>

#include "sched.h"

void init_wrr_rq(struct wrr_rq *wrr_rq)
{
	wrr_rq->wrr_nr_running = 0;
	wrr_rq->curr = NULL;
	wrr_rq->next = NULL;
	wrr_rq->first = NULL;
}

//soo
/**************************************************************
 * WRR operations on generic schedulable entities:
 */

static inline struct task_struct *wrr_se_task_of(struct sched_wrr_entity *se)
{
	return container_of(se, struct task_struct, wrr_se);
}

static inline struct rq *rq_of_wrr_rq(struct wrr_rq *wrr_rq)
{
	return container_of(wrr_rq, struct rq, wrr);
}

static inline struct wrr_rq *wrr_rq_of_task(struct task_struct *p)
{
	return &task_rq(p)->wrr;
}

static inline struct wrr_rq *wrr_rq_of_wrr_se(struct sched_wrr_entity *wrr_se)
{
	struct task_struct *p = wrr_se_task_of(wrr_se);
	struct rq *rq = task_rq(p);

	return &rq->wrr;
}

// active load balancing
// TODO rebalance_wrr_rqs()
// TODO find_busiest_wrr_rq()
// TODO update_wrr_rq_load()
// TODO trigger_load_balance()
// TODO entity 의 load는 fair의 update_entity_load_avg 호출되는 부분에서 업데이트 필요.
// set_next_entity, put_prev_entity, entity_tick, enqueue_task, dequeue_task

static struct sched_wrr_entity *pick_next_wrr_entity(struct wrr_rq *wrr_rq)
{
	// FIXME 지금 돌아가고 있는 task 도 리스트에 저장되어 있는가?
	// 있다면, 1번째 entry 가 돌아가고 있다면 2가 next 이고, 안돌아가고 있다면 1이 next이다.
	// 없다면, 1번째 entry 가 항상 next 이다.
	// cfs 의 경우, __pick_first_entity 에서 첫번째 걸 보고, skip 을 체크하고 next 로 second 를 본다.
	// rt 의 경우, active 를 찾아서, active 다음을 리턴한다.
	// 우선 다음걸 찾아서 리턴하는 걸로 구현한다.
	if (wrr_rq->curr == NULL) {
		return wrr_rq->first;
	} else {
		return wrr_rq->next;
	}
}


//soo class methods
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
	printk(KERN_DEBUG "[soo] enqueue_task_wrr");
}

/*fair
 * The dequeue_task method is called before nr_running is
 * decreased. We remove the task from the rbtree and
 * update the fair scheduling stats:
 */
static void dequeue_task_wrr(struct rq *rq, struct task_struct *p, int flags)
{
	printk(KERN_DEBUG "[soo] dequeue_task_wrr");
}

/*fair
 * sched_yield() is very simple
 *
 * The magic of dealing with the ->skip buddy is in pick_next_entity.
 */
static void yield_task_wrr(struct rq *rq)
{
	printk(KERN_DEBUG "[soo] yield_task_wrr");
}

static bool yield_to_task_wrr(struct rq *rq, struct task_struct *p, bool preempt)
{
	printk(KERN_DEBUG "[soo] yield_to_task_wrr");
	return false;
}

/*rt, fair
 * Preempt the current task with a newly woken task if needed:
 */
static void check_preempt_curr_wrr(struct rq *rq, struct task_struct *p, int flags)
{
	printk(KERN_DEBUG "[soo] check_preempt_curr_wrr");
}

static struct task_struct *pick_next_task_wrr(struct rq *rq)
{
	//soo 여기서 printk 하면 너무 많이 불려서 커널 패닉
	struct sched_wrr_entity *wrr_se;
	struct task_struct *p;
	struct wrr_rq *wrr_rq;

	wrr_rq = &rq->wrr;

	if (!wrr_rq->wrr_nr_running)
		return NULL;

	//soo do while 은 group 이 있으면 필요함. leaf 를 찾아 내려가야 하기 때문.
	wrr_se = pick_next_wrr_entity(wrr_rq);

	p = wrr_se_task_of(wrr_se);
	p->wrr_se.exec_start = rq->clock_task;

	return p;
}

/*fair
 * Account for a descheduled task:
 */
static void put_prev_task_wrr(struct rq *rq, struct task_struct *prev)
{
	printk(KERN_DEBUG "[soo] put_prev_task_wrr");
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
	printk(KERN_DEBUG "[soo] select_task_rq_wrr");
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
	printk(KERN_DEBUG "[soo] migrate_task_rq_wrr");
}

static void pre_schedule_wrr(struct rq *rq, struct task_struct *prev)
{
	printk(KERN_DEBUG "[soo] pre_schedule_wrr");
}

static void post_schedule_wrr(struct rq *rq)
{
	printk(KERN_DEBUG "[soo] post_schedule_wrr");
}

static void task_waking_wrr(struct task_struct *p)
{
	printk(KERN_DEBUG "[soo] task_waking_wrr");
}

/*rt
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
static void task_woken_wrr(struct rq *rq, struct task_struct *p)
{
	printk(KERN_DEBUG "[soo] task_woken_wrr");
}

static void set_cpus_allowed_wrr(struct task_struct *p,
				const struct cpumask *new_mask)
{
	printk(KERN_DEBUG "[soo] set_cpus_allowed_wrr");
}

/*rt Assumes rq->lock is held */
static void rq_online_wrr(struct rq *rq)
{
	printk(KERN_DEBUG "[soo] rq_online_wrr");
}

/*rt Assumes rq->lock is held */
static void rq_offline_wrr(struct rq *rq)
{
	printk(KERN_DEBUG "[soo] rq_offline_wrr");
}

/*fair Account for a task changing its policy or group.
 *
 * This routine is mostly called to set cfs_rq->curr field when a task
 * migrates between groups/classes.
 */
static void set_curr_task_wrr(struct rq *rq)
{
	printk(KERN_DEBUG "[soo] set_curr_task_wrr");
}

/*fair
 * scheduler tick hitting a task of our scheduling class:
 */
static void task_tick_wrr(struct rq *rq, struct task_struct *curr, int queued)
{
	printk(KERN_DEBUG "[soo] task_tick_wrr");
}

/*fair
 * called on fork with the child task as argument from the parent's context
 *  - child not yet on the tasklist
 *  - preemption disabled
 */
static void task_fork_wrr(struct task_struct *p)
{
	printk(KERN_DEBUG "[soo] task_fork_wrr");
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

// for sched_debug
#ifdef CONFIG_SCHED_DEBUG
void print_wrr_stats(struct seq_file *m, int cpu)
{
	// struct wrr_rq *wrr_rq;

	rcu_read_lock();
	// for_each_leaf_wrr_rq(cpu_rq(cpu), wrr_rq)
		// print_wrr_rq(m, cpu, wrr_rq);
	rcu_read_unlock();
}
#endif
