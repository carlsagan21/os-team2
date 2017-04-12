// #include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/sched.h>
// #include <linux/list.h>
// #include <linux/cred.h> /* for task_uid() */
// #include <linux/slab.h> /* for kmalloc(), GFP_KERNEL. linux/gfp 따로 include 안해도 됨. */
// #include <linux/uaccess.h> /* for copy_from_user, copy_to_user, strncpy */
// #include <linux/errno.h>

/*
 * Take a read/or write lock using the given rotation range
 * returning 0 on success, -1 on failure.
 * system call numbers 381 and 382
 */
int sys_rotlock_write(int degree, int range) /* degree - range <= LOCK RANGE <= degree + range */
{
	rotlock_t *new_lock = kmalloc(sizeof(rotlock_t), GFP_KERNEL);

	if (new_lock == NULL) /* kmalloc 은 NULL 로 제대로 되었는지 여부 판단 */
		return -ENOMEM;

	new_lock->type = WRITE_LOCK;
	new_lock->degree = degree;
	new_lock->range = range;
	new_lock->pid = task_pid_nr(current);
	INIT_LIST_HEAD(&(new_lock->pending_lh))
	INIT_LIST_HEAD(&(new_lock->wait_read_lh))
	INIT_LIST_HEAD(&(new_lock->wait_write_lh))
	INIT_LIST_HEAD(&(new_lock->aquired_lh))
	list_add(&(new_lock->pending_lh), &(head.pending_lh))

	printk(KERN_DEBUG "[soo] sys_rotlock_write: %d, %d, %d, %d\n", new_lock->type, new_lock->degree, new_lock->range, new_lock->pid);

	return 0;
};
