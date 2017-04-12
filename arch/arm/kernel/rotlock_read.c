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
	rotlock_t new_lock;
	rotlock_t *p_lock;
	rotlock_t *p_temp_lock;

	new_lock.type = READ_LOCK;
	new_lock.degree = degree;
	new_lock.range = range;
	new_lock.pid = task_pid_nr(current);
	INIT_LIST_HEAD(&(new_lock.pending_lh));
	list_add(&(new_lock.pending_lh), &pending_lh);

	rotlock_t new_lock2;

	new_lock2.type = READ_LOCK;
	new_lock2.degree = degree;
	new_lock2.range = 1;
	new_lock2.pid = task_pid_nr(current);
	INIT_LIST_HEAD(&(new_lock2.pending_lh));
	list_add(&(new_lock2.pending_lh), &pending_lh);
	// list_add(&(new_lock.wait_read_lh), &wait_read_lh);
	// list_add(&(new_lock.wait_write_lh), &wait_write_lh);
	// list_add(&(new_lock.aquired_lh), &aquired_lh);
	// INIT_LIST_HEAD(&(new_lock.pending_lh));
	// INIT_LIST_HEAD(&(new_lock.wait_read_lh));
	// INIT_LIST_HEAD(&(new_lock.wait_write_lh));
	// INIT_LIST_HEAD(&(new_lock.aquired_lh));
	// list_add(&(new_lock->pending_lh), &(head.pending_lh));
	printk(KERN_DEBUG "[soo] sys_rotlock_read pending_lh:  %p\n", &pending_lh);

	printk(KERN_DEBUG "[soo] sys_rotlock_read: %d, %d, %d, %d, %p, %p, %p, %p, %p, %p\n", new_lock.type, new_lock.degree, new_lock.range, new_lock.pid, &(new_lock.pending_lh), &(new_lock.wait_read_lh), &(new_lock.wait_write_lh), &(new_lock.aquired_lh), new_lock.pending_lh.next, new_lock.pending_lh.prev);
	printk(KERN_DEBUG "[soo] sys_rotlock_read: %d, %d, %d, %d, %p, %p, %p, %p, %p, %p\n", new_lock2.type, new_lock2.degree, new_lock2.range, new_lock2.pid, &(new_lock2.pending_lh), &(new_lock2.wait_read_lh), &(new_lock2.wait_write_lh), &(new_lock2.aquired_lh), new_lock2.pending_lh.next, new_lock2.pending_lh.prev);

	list_for_each_entry_safe(p_lock, p_temp_lock, &pending_lh, pending_lh) {
		printk(KERN_DEBUG "[soo] sys_rotlock_read pending list: %d, %d, %d, %d, %p, %p, %p, %p, %p, %p\n", p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, &(p_lock->pending_lh), &(p_lock->wait_read_lh), &(p_lock->wait_write_lh), &(p_lock->aquired_lh), p_lock->pending_lh.next, p_lock->pending_lh.prev);
	}

	return 0;
};
