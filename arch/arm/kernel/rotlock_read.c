// #include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/sched.h>
// #include <linux/list.h>
// #include <linux/cred.h> /* for task_uid() */
#include <linux/slab.h> /* for kmalloc(), GFP_KERNEL. linux/gfp 따로 include 안해도 됨. */
// #include <linux/uaccess.h> /* for copy_from_user, copy_to_user, strncpy */
// #include <linux/errno.h>

/*
 * Take a read/or write lock using the given rotation range
 * returning 0 on success, -1 on failure.
 * system call numbers 381 and 382
 */
int sys_rotlock_read(int degree, int range) /* 0 <= degree < 360 , 0 < range < 180 */
{
	rotlock_t *p_new_lock = kmalloc(sizeof(rotlock_t), GFP_KERNEL);

	if (p_new_lock == NULL) /* kmalloc 은 NULL 로 제대로 되었는지 여부 판단 */
		return -ENOMEM;

	rotlock_t *p_lock;
	rotlock_t *p_temp_lock;

	p_new_lock->type = READ_LOCK;
	p_new_lock->degree = degree;
	p_new_lock->range = range;
	p_new_lock->pid = task_pid_nr(current);
	INIT_LIST_HEAD(&(p_new_lock->list_node));
	list_add(&(p_new_lock->list_node), &pending_lh); // 일단 pending 으로 보냄
	p_new_lock->status = PENDING;

	// 경우나누기
	// call refresh
	waiting_list_refresh()


	// rotlock_t new_lock2;
	//
	// new_lock2.type = READ_LOCK;
	// new_lock2.degree = degree;
	// new_lock2.range = 1;
	// new_lock2.pid = task_pid_nr(current);
	// INIT_LIST_HEAD(&(new_lock2.pending_lh));
	// list_add(&(new_lock2.pending_lh), &pending_lh);
	// list_add(&(new_lock.wait_read_lh), &wait_read_lh);
	// list_add(&(new_lock.wait_write_lh), &wait_write_lh);
	// list_add(&(new_lock.acquired_lh), &acquired_lh);
	// INIT_LIST_HEAD(&(new_lock.pending_lh));
	// INIT_LIST_HEAD(&(new_lock.wait_read_lh));
	// INIT_LIST_HEAD(&(new_lock.wait_write_lh));
	// INIT_LIST_HEAD(&(new_lock.acquired_lh));
	// list_add(&(new_lock->pending_lh), &(head.pending_lh));


	list_for_each_entry_safe(p_lock, p_temp_lock, &pending_lh, pending_lh) {
		printk(KERN_DEBUG "[soo] sys_rotlock_read pending_lh: %d, %d, %d, %d, %p, %p, %p, %p, %p, %p\n", p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, &(p_lock->pending_lh), &(p_lock->wait_read_lh), &(p_lock->wait_write_lh), &(p_lock->acquired_lh), p_lock->pending_lh.next, p_lock->pending_lh.prev);
	}

	list_for_each_entry_safe(p_lock, p_temp_lock, &wait_read_lh, wait_read_lh) {
		printk(KERN_DEBUG "[soo] sys_rotlock_read wait_read_lh: %d, %d, %d, %d, %p, %p, %p, %p, %p, %p\n", p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, &(p_lock->pending_lh), &(p_lock->wait_read_lh), &(p_lock->wait_write_lh), &(p_lock->acquired_lh), p_lock->pending_lh.next, p_lock->pending_lh.prev);
	}

	list_for_each_entry_safe(p_lock, p_temp_lock, &wait_write_lh, wait_write_lh) {
		printk(KERN_DEBUG "[soo] sys_rotlock_read wait_write_lh: %d, %d, %d, %d, %p, %p, %p, %p, %p, %p\n", p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, &(p_lock->pending_lh), &(p_lock->wait_read_lh), &(p_lock->wait_write_lh), &(p_lock->acquired_lh), p_lock->pending_lh.next, p_lock->pending_lh.prev);
	}

	list_for_each_entry_safe(p_lock, p_temp_lock, &acquired_lh, acquired_lh) {
		printk(KERN_DEBUG "[soo] sys_rotlock_read acquired_lh: %d, %d, %d, %d, %p, %p, %p, %p, %p, %p\n", p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, &(p_lock->pending_lh), &(p_lock->wait_read_lh), &(p_lock->wait_write_lh), &(p_lock->acquired_lh), p_lock->pending_lh.next, p_lock->pending_lh.prev);
	}

	return 0;
};
