// #include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/sched.h>
#include <linux/list.h>
// #include <linux/cred.h> /* for task_uid() */
#include <linux/slab.h> /* for kmalloc(), GFP_KERNEL. linux/gfp 따로 include 안해도 됨. */
// #include <linux/uaccess.h> /* for copy_from_user, copy_to_user, strncpy */
// #include <linux/errno.h>
#include <linux/poison.h>

/*
 * Take a read/or write lock using the given rotation range
 * returning 0 on success, -1 on failure.
 * system call numbers 381 and 382
 */
int sys_rotlock_read(int degree, int range) /* 0 <= degree < 360 , 0 < range < 180 */
{
	rotlock_t *p_new_lock;
	// int acquirable;

	pr_debug("[soo] sys_rotlock_read\n");

	p_new_lock = kmalloc(sizeof(rotlock_t), GFP_KERNEL);
	if (p_new_lock == NULL) /* kmalloc 은 NULL 로 제대로 되었는지 여부 판단 */
		return -ENOMEM;

	p_new_lock->type = READ_LOCK;
	p_new_lock->degree = degree;
	p_new_lock->range = range;
	p_new_lock->pid = task_pid_nr(current);
	p_new_lock->status = PENDING;

	// acquirable = 0;
	mutex_lock(&rotlock_mutex); // kill, interrupt 를 막아버림.
	// acquirable = is_acquirable(p_new_lock);

	list_add_pending(p_new_lock); // 일단 pending 으로 보냄
	refresh_pending_waiting_lists();
	wait_write_to_acquire();
	wait_read_to_acquire();

	__print_all_lists();
	mutex_unlock(&rotlock_mutex);

	pr_debug("[soo] p_lock status: %d\n", p_new_lock->status);

	// aquire 이거나 이미 삭제되었거나.
	// 이 부분에서 멀티쓰레드에 의해 list 구조가 변형되었을 가능성이 있음.
	wait_event_interruptible(wq_rotlock, p_new_lock->status == ACQUIRED || is_rotlock_deleted(p_new_lock));

	if (is_rotlock_deleted(p_new_lock)) { // NULL 이 아니면서 리스트가 삭제되어 있을 시
		kfree(p_new_lock);
	}
	// if (!acquirable) {
	//
	// 	wait_event_interruptible(wq, p_new_lock->status == ACQUIRED);
	// 	// TODO wait queue
	// }

	return 0;
};
