// #include <linux/unistd.h>
#include <linux/kernel.h>
// #include <linux/sched.h>
// #include <linux/list.h>
// #include <linux/cred.h> /* for task_uid() */
// #include <linux/slab.h> /* for kmalloc(), GFP_KERNEL. linux/gfp 따로 include 안해도 됨. */
// #include <linux/uaccess.h> /* for copy_from_user, copy_to_user, strncpy */
// #include <linux/errno.h>

/*
 * Release a read/or write lock using the given rotation range
 * returning 0 on success, -1 on failure.
 * system call numbers 383 and 384
 */
int sys_rotunlock_read(int degree, int range) /* 0 <= degree < 360 , 0 < range < 180 */
{
	printk("[soo] sys_rotunlock_read\n");

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
