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
int sys_rotlock_write(int degree, int range) /* degree - range <= LOCK RANGE <= degree + range */
{
	rotlock_t new_lock;
	rotlock_t *p_lock;
	rotlock_t *p_temp_lock;
	// if (new_lock == NULL) /* kmalloc 은 NULL 로 제대로 되었는지 여부 판단 */
	// 	return -ENOMEM;

	new_lock.type = WRITE_LOCK;
	new_lock.degree = degree;
	new_lock.range = range;
	new_lock.pid = task_pid_nr(current);
	INIT_LIST_HEAD(&(new_lock.pending_lh));
	// new_lock.pending_lh.next = &new_lock.pending_lh;
	// new_lock.pending_lh.prev = &new_lock.pending_lh;
	// list_add(&(new_lock.pending_lh), &pending_lh);
	// list_add(&(new_lock.wait_read_lh), &wait_read_lh);
	// list_add(&(new_lock.wait_write_lh), &wait_write_lh);
	// list_add(&(new_lock.aquired_lh), &aquired_lh);
	// INIT_LIST_HEAD(&(new_lock.pending_lh));
	// INIT_LIST_HEAD(&(new_lock.wait_read_lh));
	// INIT_LIST_HEAD(&(new_lock.wait_write_lh));
	// INIT_LIST_HEAD(&(new_lock.aquired_lh));
	// list_add(&(new_lock->pending_lh), &(head.pending_lh));

	printk(KERN_DEBUG "[soo] sys_rotlock_write p:  %p\n", &pending_lh);

	printk(KERN_DEBUG "[soo] sys_rotlock_write: %d, %d, %d, %d, %p, %p, %p, %p, %p, %p\n", new_lock.type, new_lock.degree, new_lock.range, new_lock.pid, &(new_lock.pending_lh), &(new_lock.wait_read_lh), &(new_lock.wait_write_lh), &(new_lock.aquired_lh), new_lock.pending_lh.next, new_lock.pending_lh.prev);

	// list_for_each_entry_safe(p_lock, p_temp_lock, &pending_lh, pending_lh) {
	// 	printk(KERN_DEBUG "[soo] sys_rotlock_write pending: %d, %d, %d, %d, %p, %p, %p, %p, %p, %p\n", p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, &(p_lock->pending_lh), &(p_lock->wait_read_lh), &(p_lock->wait_write_lh), &(p_lock->aquired_lh), p_lock->pending_lh.next, p_lock->pending_lh.prev);
	// }

	return 0;
};
