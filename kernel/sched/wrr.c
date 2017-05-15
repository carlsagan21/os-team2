#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/jiffies.h>

#include "sched.h"

void print_wrr_list(struct wrr_rq *wrr_rq);

void init_wrr_rq(struct wrr_rq *wrr_rq, struct rq *rq)
{
	wrr_rq->total_weight = 0;
	wrr_rq->wrr_nr_running = 0;
	INIT_LIST_HEAD(&wrr_rq->run_list);
	raw_spin_lock_init(&wrr_rq->lock);
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
// set_next_entity, put_prev_entity, entity_tick, enqueue_task, dequeue_task

// list 에 더하고
// wrr_rq 의 wrr_nr_running 을 ++
static void
enqueue_wrr_entity(struct wrr_rq *wrr_rq, struct sched_wrr_entity *wrr_se, int flags)
{
	list_add_tail(&wrr_se->run_list, &wrr_rq->run_list);

	wrr_rq->wrr_nr_running++;
}

static struct sched_wrr_entity *pick_next_wrr_entity(struct wrr_rq *wrr_rq)
{
	// FIXME 지금 돌아가고 있는 task 도 리스트에 저장되어 있는가?
	// 있다면, 1번째 entry 가 돌아가고 있다면 2가 next 이고, 안돌아가고 있다면 1이 next이다.
	// 없다면, 1번째 entry 가 항상 next 이다.
	// cfs 의 경우, __pick_first_entity 에서 첫번째 걸 보고, skip 을 체크하고 next 로 second 를 본다.
	// rt 의 경우, active 를 찾아서, active 다음을 리턴한다.
	// 우선 다음걸 찾아서 리턴하는 걸로 구현한다.
	struct sched_wrr_entity *wrr_se = list_first_entry_or_null(&wrr_rq->run_list, struct sched_wrr_entity, run_list);

	// if (wrr_rq->curr == NULL) {
	return wrr_se;
	// } else {
	//      return wrr_se;//TODO
	// }
}

static void requeue_task_wrr(struct rq *rq, struct task_struct *p, int head)
{
	struct sched_wrr_entity *wrr_se = &p->wrr_se;
	struct wrr_rq *wrr_rq = &rq->wrr;

	list_move_tail(&wrr_se->run_list, &wrr_rq->run_list);
}

static void update_curr_wrr(struct rq *rq)
{
       // struct task_struct *curr;
       // struct sched_wrr_entity *wrr_se;
  // struct list_head *rq_list;
  // struct list_head *se_list;
  // struct list_head *next;
  // struct wrr_rq *wrr_rq;
       //
  // wrr_rq = &rq->wrr;
       //
       // // raw_spin_lock(&wrr_rq->lock);
       //
  // rq_list = wrr_rq_list(wrr_rq);
       //
       // if (rq->curr == NULL || rq->wrr.curr == NULL) {
       //      // raw_spin_unlock(&wrr_rq->lock);
       //      return;
       // }
       // wrr_se = rq->wrr.curr;
       // // se = &curr->wrr;
       // // se_list = &se->run_list;
       //
       // /* Decrease the time slice of currently running task until it reaches zero */
       // if (--wrr_se->time_slice) {
       //      // raw_spin_unlock(&wrr_rq->lock);
       //      return;
       // }
       //
       // if (se_list->next != se_list->prev) { /* < If more than one element in the list, move the cursor to the next task and resched */
       //      next = se_list->next;
       //      if (next == &wrr_rq->run_queue)
       //              next = next->next;
       //      wrr_rq->curr = wrr_task_of(list_entry(next, struct sched_wrr_entity, run_list));
       //      set_tsk_need_resched(curr);
       // } else
       //      se->time_slice = se->weight * WRR_TIMESLICE; /* < Else, refill the current task's time_slice */
       //
       // // raw_spin_unlock(&wrr_rq->lock);
}
static void update_curr(struct rq *rq)
{
      //  struct task_struct *curr;
      //  struct sched_wrr_entity *wrr_se;
      //  wrr_se = rq->wrr.curr;
      //  struct wrr_rq *wrr_rq = &rq->wrr;
			 //
      //  if (wrr_se == NULL)
      //          return;
			 //
      //  // curr = rq->wrr.curr;
			 //
      //  // struct sched_wrr_entity *se;
      //  // se = &curr->wrr;
			 //
      //  if(wrr_se->time_slice > 0) {
      //          --wrr_se->time_slice;
      //          return;
      //  } //if current task has remaining time slice, -1 to time slice and terminate
			 //
      //  // if(&wrr_se->run_list.next != &wrr_se->run_list.prev)//get next task
      //  // {
      //          struct list_head *next = &wrr_se->run_list.next;
      //          if(next == &wrr_rq->run_list)
      //                  next = next->next;
      //          curr = wrr_se_task_of(list_entry(next, struct sched_wrr_entity, run_list));
      //          set_tsk_need_resched(curr);//FIXME implement wrr_task_of()
      //  // }
      //  // else//when there is no next task, refill the timeslice
      //  // {
      //  //      wrr_se->time_slice = wrr_se->weight * TIME_SLICE;
      //  // }
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
	struct wrr_rq *wrr_rq = wrr_rq_of_task(p);
	raw_spin_lock(&wrr_rq->lock);
	struct sched_wrr_entity *wrr_se = &p->wrr_se;

	enqueue_wrr_entity(wrr_rq, wrr_se, 0);

	inc_nr_running(rq);
	wrr_rq->total_weight += wrr_se->weight;
	p->on_rq = 1;
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func enqueue_task_wrr: %d", p->pid);
	print_wrr_list(wrr_rq);
#endif
	raw_spin_unlock(&wrr_rq->lock);
}

/*fair
 * The dequeue_task method is called before nr_running is
 * decreased. We remove the task from the rbtree and
 * update the fair scheduling stats:
 */
static void dequeue_task_wrr(struct rq *rq, struct task_struct *p, int flags)
{
	struct wrr_rq *wrr_rq = wrr_rq_of_task(p);
	raw_spin_lock(&wrr_rq->lock);
	struct sched_wrr_entity *wrr_se = &p->wrr_se;

	list_del_init(&wrr_se->run_list);

	wrr_rq->wrr_nr_running--;

	dec_nr_running(rq);
	wrr_rq->total_weight -= wrr_se->weight;
	p->on_rq = 0;
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func dequeue_task_wrr: %d", p->pid);
	print_wrr_list(&rq->wrr);
#endif
	raw_spin_unlock(&wrr_rq->lock);
}

/*fair
 * sched_yield() is very simple
 *
 * The magic of dealing with the ->skip buddy is in pick_next_entity.
 */
static void yield_task_wrr(struct rq *rq)
{
//        struct task_struct *temp = rq->curr;
//        dequeue_task_wrr(rq, temp, 0);
//        enqueue_task_wrr(rq, temp, 0);
//        // requeue_task_wrr(rq, rq->curr, 0);
//        // TODO list_move_tail
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func yield_task_wrr");
	print_wrr_list(&rq->wrr);
#endif
}

//soo yield_to_task_wrr 은 필요가 없다. 특정 task 로 yield 하는 경우가 없기 때문.
// static bool yield_to_task_wrr(struct rq *rq, struct task_struct *p, bool preempt)
// {
//     printk(KERN_DEBUG "[soo] wrr_func yield_to_task_wrr");
//     return false;
// }

/*rt, fair
 * Preempt the current task with a newly woken task if needed:
 */
static void check_preempt_curr_wrr(struct rq *rq, struct task_struct *p, int flags)
{
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func check_preempt_curr_wrr: %d", p->pid);
#endif
}


static struct task_struct *pick_next_task_wrr(struct rq *rq)
{
	// //soo 여기서 printk 하면 너무 많이 불려서 커널 패닉
	// struct sched_wrr_entity *wrr_se;
	// struct task_struct *p = NULL;
      //  struct wrr_rq *wrr_rq;
			//
      //  wrr_rq = &rq->wrr;
			//
      //  if (!wrr_rq->wrr_nr_running)
      //          return NULL;
			//
      //  //soo do while 은 group 이 있으면 필요함. leaf 를 찾아 내려가야 하기 때문.
      //  wrr_se = pick_next_wrr_entity(wrr_rq);
			//
      //  p = wrr_se_task_of(wrr_se);
      //  p->wrr_se.time_slice = p->wrr_se.weight * TIME_SLICE;
      // //  p->wrr_se.exec_start = rq->clock_task;
			// wrr_rq->curr = p;

	struct wrr_rq *wrr_rq;
	struct task_struct *p = NULL;
	wrr_rq = &rq->wrr;

	if (!wrr_rq->wrr_nr_running)
		return NULL;

	//soo do while 은 group 이 있으면 필요함. leaf 를 찾아 내려가야 하기 때문.
	wrr_se = pick_next_wrr_entity(wrr_rq);

	p = wrr_se_task_of(wrr_se);
	p->wrr_se.time_slice = p->wrr_se.weight * TIME_SLICE;
	//  p->wrr_se.exec_start = rq->clock_task;
	// wrr_rq->curr = p;

#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func pick_next_task_wrr2: %d", wrr_se_task_of(wrr_se)->pid);
	print_wrr_list(&rq->wrr);
#endif
	return p;
}

/*fair
 * Account for a descheduled task:
 */
static void put_prev_task_wrr(struct rq *rq, struct task_struct *prev)
{
	// struct wrr_rq *wrr_rq = &rq->wrr;
	// wrr_rq->curr = NULL;
	//TODO list_move_tail(&prev->wrr_se.run_list, &wrr_rq->run_list);
	// requeue_task_wrr(rq, rq->curr, 0);
	// enqueue_task_wrr(rq, prev, 0);// FIXME 리스트에 추가하고, rq 와 wrr_rq 에 nr 을 올려줘야하나? 지금은 올려줌.
	// 안올려줄경우
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func put_prev_task_wrr: %d", prev->pid);
#endif
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
	// int cpu = smp_processor_id();
	int prev_cpu = task_cpu(p);
	// int new_cpu = cpu;
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func select_task_rq_wrr: %d", p->pid);
#endif
	return prev_cpu;
}

//TODO soo migration 상황에서 불리는 것. rt 에 없음. 일단 제외시켜놓는다.
/*fair
 * Called immediately before a task is migrated to a new cpu; task_cpu(p) and
 * cfs_rq_of(p) references at time of call are still valid and identify the
 * previous cpu.  However, the caller only guarantees p->pi_lock is held; no
 * other assumptions, including the state of rq->lock, should be made.
 */
// static void
// migrate_task_rq_wrr(struct task_struct *p, int next_cpu)
// {
//     printk(KERN_DEBUG "[soo] wrr_func migrate_task_rq_wrr");
// }

//NOTE soo
// 현재 pre_schedule에서는 오직 RT schedule class만이 구현되어있는데, 이는 Linux라는 OS의 특성이 realre_schedule 함수는
// if(rq->rt.highest_prio.curr > prev->prio) pull_rt_task(rq); 구문을 통해 현재 프로세스의 priority를e 내 Real Time 프로세스 중 가장 높은 priority보다 낮을 경우 RT task를 가져온다.
// 없으면 호출 안됨. 필요없을듯.
// static void pre_schedule_wrr(struct rq *rq, struct task_struct *prev)
// {
//     printk(KERN_DEBUG "[soo] wrr_func pre_schedule_wrr");
// }

//NOTE soo
// pre_schedule 과 마찬가지. rt 전용이고 안불러도 될듯.
// static void post_schedule_wrr(struct rq *rq)
// {
//     printk(KERN_DEBUG "[soo] wrr_func post_schedule_wrr");
// }

//NOTE fair 전용. min_vruntime 을 업데이트 해주는거 같음.필요없겠지?
// static void task_waking_wrr(struct task_struct *p)
// {
//     printk(KERN_DEBUG "[soo] wrr_func task_waking_wrr");
// }

//NOTE task_woken rt 에서 push_rt_tasks 를 함.
/*rt
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
// static void task_woken_wrr(struct rq *rq, struct task_struct *p)
// {
//     printk(KERN_DEBUG "[soo] wrr_func task_woken_wrr");
// }

//NOTE rt에서만 쓰고... 별 관련없어보임.
// static void set_cpus_allowed_wrr(struct task_struct *p,
//                             const struct cpumask *new_mask)
// {
//     printk(KERN_DEBUG "[soo] wrr_func set_cpus_allowed_wrr");
// }

/*rt Assumes rq->lock is held */
static void rq_online_wrr(struct rq *rq)
{
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func rq_online_wrr");
	print_wrr_list(&rq->wrr);
#endif
}

/*rt Assumes rq->lock is held */
static void rq_offline_wrr(struct rq *rq)
{
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func rq_offline_wrr");
	print_wrr_list(&rq->wrr);
#endif
}

//TODO 다른 클래스로부터 넘어올 때 불림.
/*fair Account for a task changing its policy or group.
 *
 * This routine is mostly called to set cfs_rq->curr field when a task
 * migrates between groups/classes.
 */
static void set_curr_task_wrr(struct rq *rq)
{
	// struct wrr_rq *wrr_rq = &rq->wrr;
	// wrr_rq->curr = &rq->curr->wrr_se;
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func set_curr_task_wrr");
	print_wrr_list(&rq->wrr);
#endif
}

/*fair
 * scheduler tick hitting a task of our scheduling class:
 */
static void task_tick_wrr(struct rq *rq, struct task_struct *curr, int queued)
{
	// update_curr_wrr(rq);
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func task_tick_wrr: %d", curr->pid);
#endif
}

/*fair
 * called on fork with the child task as argument from the parent's context
 *  - child not yet on the tasklist
 *  - preemption disabled
 */
static void task_fork_wrr(struct task_struct *p)
{
	p->wrr_se.weight = p->real_parent->wrr_se.weight;
	p->wrr_se.time_slice = p->wrr_se.weight * TIME_SLICE;
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func task_fork_wrr: %d", p->pid);
#endif
}

//TODO weight 가 없을 떄 weight 를 잡아줌.
/*rt
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
static void switched_from_wrr(struct rq *rq, struct task_struct *p)
{
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func switched_from_wrr: %d", p->pid);
#endif
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
	p->wrr_se.weight = DEFAULT_WEIGHT;
	p->wrr_se.time_slice = DEFAULT_WEIGHT * TIME_SLICE;
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func switched_to_wrr: %d", p->pid);
#endif
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
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func prio_changed_wrr: %d", p->pid);
#endif
}

static unsigned int get_rr_interval_wrr(struct rq *rq, struct task_struct *task)
{
#ifdef CONFIG_SCHED_DEBUG
	printk(KERN_DEBUG "[soo] wrr_func get_rr_interval_wrr: %d", task->pid);
#endif
	return 0;
}

// static void task_move_group_wrr(struct task_struct *p, int on_rq)
// {
// #ifdef CONFIG_SCHED_DEBUG
//     printk(KERN_DEBUG "[soo] wrr_func task_move_group_wrr: %d", p->pid);
// #endif
// }


const struct sched_class wrr_sched_class = {
	//      const struct sched_class *next;
	.next                   = &fair_sched_class, // fair 여야 함. rt.c 를 wrr 로 바꾸고.

	//      void (*enqueue_task) (struct rq *rq, struct task_struct *p, int flags);
	.enqueue_task           = enqueue_task_wrr,
	//      void (*dequeue_task) (struct rq *rq, struct task_struct *p, int flags);
	.dequeue_task           = dequeue_task_wrr,
	//      void (*yield_task) (struct rq *rq);
	.yield_task             = yield_task_wrr,
	//      bool (*yield_to_task) (struct rq *rq, struct task_struct *p, bool preempt);
	// yield_to_task가 fair 에는 있고 rt 에는 없음.
	// .yield_to_task               = yield_to_task_wrr,

	//      void (*check_preempt_curr) (struct rq *rq, struct task_struct *p, int flags);
	.check_preempt_curr     = check_preempt_curr_wrr,

	//   struct task_struct * (*pick_next_task) (struct rq *rq);
	.pick_next_task         = pick_next_task_wrr,
	//      void (*put_prev_task) (struct rq *rq, struct task_struct *p);
	.put_prev_task          = put_prev_task_wrr,

#ifdef CONFIG_SMP
	//      int  (*select_task_rq)(struct task_struct *p, int sd_flag, int flags);
	.select_task_rq         = select_task_rq_wrr,
	//      void (*migrate_task_rq)(struct task_struct *p, int next_cpu);
	// fair only
	// .migrate_task_rq = migrate_task_rq_wrr,

	//      void (*pre_schedule) (struct rq *this_rq, struct task_struct *task);
	// rt only
	// .pre_schedule                = pre_schedule_wrr,
	//      void (*post_schedule) (struct rq *this_rq);
	// .post_schedule               = post_schedule_wrr,
	//      void (*task_waking) (struct task_struct *task);
	// fair only
	// .task_waking = task_waking_wrr,
	//      void (*task_woken) (struct rq *this_rq, struct task_struct *task);
	// rt only
	// .task_woken          = task_woken_wrr,

	//      void (*set_cpus_allowed)(struct task_struct *p,
	//                               const struct cpumask *newmask);
	// rt only
	// .set_cpus_allowed       = set_cpus_allowed_wrr,

	//      void (*rq_online)(struct rq *rq);
	.rq_online              = rq_online_wrr,
	//      void (*rq_offline)(struct rq *rq);
	.rq_offline             = rq_offline_wrr,
#endif
	//      void (*set_curr_task) (struct rq *rq);
	.set_curr_task          = set_curr_task_wrr,
	//      void (*task_tick) (struct rq *rq, struct task_struct *p, int queued);
	.task_tick              = task_tick_wrr,
	//      void (*task_fork) (struct task_struct *p);
	// fair only
	.task_fork = task_fork_wrr,

	//      void (*switched_from) (struct rq *this_rq, struct task_struct *task);
	// 정의에는 SMP 바깥. fair 에도 바깥. rt 에는 안.
	.switched_from          = switched_from_wrr,
	//      void (*switched_to) (struct rq *this_rq, struct task_struct *task);
	// fair only
	.switched_to            = switched_to_wrr,
	//      void (*prio_changed) (struct rq *this_rq, struct task_struct *task,
	//                           int oldprio);
	.prio_changed           = prio_changed_wrr,

	//      unsigned int (*get_rr_interval) (struct rq *rq,
	//                                       struct task_struct *task);
	.get_rr_interval        = get_rr_interval_wrr,

#ifdef CONFIG_FAIR_GROUP_SCHED
       //      void (*task_move_group) (struct task_struct *p, int on_rq);
       // fair only
       // .task_move_group = task_move_group_wrr
#endif
};

// for sched_debug
#ifdef CONFIG_SCHED_DEBUG
void print_wrr_stats(struct seq_file *m, int cpu)
{
 // // struct wrr_rq *wrr_rq;
 //
 // rcu_read_lock();
 // // for_each_leaf_wrr_rq(cpu_rq(cpu), wrr_rq)
 //         // print_wrr_rq(m, cpu, wrr_rq);
 // rcu_read_unlock();
}

void print_wrr_list(struct wrr_rq *wrr_rq)
{
//  struct sched_wrr_entity *wrr_se;
//  struct task_struct *p;
//  list_for_each_entry(wrr_se, &wrr_rq->run_list, run_list) {
//          p = wrr_se_task_of(wrr_se);
//          printk(KERN_DEBUG "[soo] print_wrr_list task: %d, %d, %d, %d", p->pid, p->wrr_se.weight, p->wrr_se.time_slice, HZ);
//  }
//  printk(KERN_DEBUG "[soo] print_wrr_list curr, nr_wrr_rq, current, rq->clock: %d, %d, %d, %llu", wrr_rq->curr, wrr_rq->wrr_nr_running, current->pid, rq_of_wrr_rq(wrr_rq)->clock);
}
#endif
