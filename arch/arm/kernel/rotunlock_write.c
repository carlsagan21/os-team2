// #include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/sched.h>
#include <linux/list.h>
// #include <linux/cred.h> /* for task_uid() */
// #include <linux/slab.h> /* for kmalloc(), GFP_KERNEL. linux/gfp 따로 include 안해도 됨. */
// #include <linux/uaccess.h> /* for copy_from_user, copy_to_user, strncpy */
// #include <linux/errno.h>

/*
 * Release a read/or write lock using the given rotation range
 * returning 0 on success, -1 on failure.
 * system call numbers 383 and 384
 */
int sys_rotunlock_write(int degree, int range) /* degree - range <= LOCK RANGE <= degree + range */
{
	rotlock_t *p_lock;
	rotlock_t *p_temp_lock;
	int pid;

	printk(KERN_DEBUG "[soo] sys_rotunlock_write\n");

	pid = task_pid_nr(current);

	list_for_each_entry_safe_reverse(p_lock, p_temp_lock, &pending_lh, list_node) {
		if (is_unlock_match(p_lock, WRITE_LOCK, degree, range, pid))
			list_del(&(p_lock->list_node)); // 끝내야. flag 를 쓰던가
	}

	list_for_each_entry_safe_reverse(p_lock, p_temp_lock, &wait_read_lh, list_node) {
		if (is_unlock_match(p_lock, WRITE_LOCK, degree, range, pid))
			list_del(&(p_lock->list_node));
	}

	list_for_each_entry_safe_reverse(p_lock, p_temp_lock, &wait_write_lh, list_node) {
		if (is_unlock_match(p_lock, WRITE_LOCK, degree, range, pid))
			list_del(&(p_lock->list_node));
	}

	list_for_each_entry_safe_reverse(p_lock, p_temp_lock, &acquired_lh, list_node) {
		if (is_unlock_match(p_lock, WRITE_LOCK, degree, range, pid))
			list_del(&(p_lock->list_node));
	}

	list_for_each_entry_safe(p_lock, p_temp_lock, &pending_lh, list_node) {
		printk(KERN_DEBUG "[soo] pending_lh: %d, %d, %d, %d, %d, %p, %p, %p\n",
		p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, p_lock->status,
		&(p_lock->list_node), p_lock->list_node.next, p_lock->list_node.prev);
	}

	list_for_each_entry_safe(p_lock, p_temp_lock, &wait_read_lh, list_node) {
		printk(KERN_DEBUG "[soo] wait_read_lh: %d, %d, %d, %d, %d, %p, %p, %p\n",
		p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, p_lock->status,
		&(p_lock->list_node), p_lock->list_node.next, p_lock->list_node.prev);
	}

	list_for_each_entry_safe(p_lock, p_temp_lock, &wait_write_lh, list_node) {
		printk(KERN_DEBUG "[soo] wait_write_lh: %d, %d, %d, %d, %d, %p, %p, %p\n",
		p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, p_lock->status,
		&(p_lock->list_node), p_lock->list_node.next, p_lock->list_node.prev);
	}

	list_for_each_entry_safe(p_lock, p_temp_lock, &acquired_lh, list_node) {
		printk(KERN_DEBUG "[soo] acquired_lh: %d, %d, %d, %d, %d, %p, %p, %p\n",
		p_lock->type, p_lock->degree, p_lock->range, p_lock->pid, p_lock->status,
		&(p_lock->list_node), p_lock->list_node.next, p_lock->list_node.prev);
	}

	return 0;
};
