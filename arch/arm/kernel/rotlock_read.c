#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/slab.h>

/*
 * Take a read/or write lock using the given rotation range
 * returning 0 on success, -1 on failure.
 * system call numbers 381 and 382
 */
int sys_rotlock_read(int degree, int range) /* 0 <= degree < 360 , 0 < range < 180 */
{
	rotlock_t *p_new_lock;

	pr_debug("[soo] sys_rotlock_read\n");

	p_new_lock = kmalloc(sizeof(rotlock_t), GFP_KERNEL);
	if (p_new_lock == NULL) /* kmalloc 은 NULL 로 제대로 되었는지 여부 판단 */
		return -ENOMEM;

	p_new_lock->type = READ_LOCK;
	p_new_lock->degree = degree;
	p_new_lock->range = range;
	p_new_lock->pid = task_pid_nr(current);
	p_new_lock->status = PENDING;

	/*
	 * mutually exclusive 한 리스트 수정.
	 * kill, interrupt 를 막음.
	 * 일단 pending 으로 보내버리고, 리스트를 재조정시킴.
	 */
	mutex_lock(&rotlock_mutex); //

	list_add_pending(p_new_lock);
	refresh_pending_waiting_lists();
	wait_write_to_acquire();
	wait_read_to_acquire();

	// DEBUG __print_all_lists();
	mutex_unlock(&rotlock_mutex);

	pr_debug("[soo] p_lock status: %d\n", p_new_lock->status);

	/**
	 * aquire 이거나 이미 삭제되었거나.
 	 * 이 부분에서 멀티쓰레드에 의해 위의 뮤텍스 안에서 수정된 list 구조가 그대로 유지되었으리라는 보장 없음.
	 * 그래도 CONDITION 이 성립해야 하는건 마찬가지임.
	 * FIXME p_new_lock 이 없어졌을 경우 커버해야. ACQUIRED 되지 않고 중간에 삭제되었을 경우, p_new_lock->status == ACQUIRED 를 보장할 수 없으므로 딜리트 되었는지 여부도 보아야 함.
	 */
	wait_event_interruptible(wq_rotlock, p_new_lock->status == ACQUIRED);

	// if (is_rotlock_deleted(p_new_lock)) {
	// 	kfree(p_new_lock);
	// }

	return 0;
};
