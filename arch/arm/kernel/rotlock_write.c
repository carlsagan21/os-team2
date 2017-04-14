// #include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/sched.h>
#include <linux/list.h>
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
	rotlock_t *p_new_lock;

	pr_debug("[soo] sys_rotlock_write\n");

	p_new_lock = kmalloc(sizeof(rotlock_t), GFP_KERNEL);
	if (p_new_lock == NULL) /* kmalloc 은 NULL 로 제대로 되었는지 여부 판단 */
		return -ENOMEM;

	p_new_lock->type = WRITE_LOCK;
	p_new_lock->degree = degree;
	p_new_lock->range = range;
	p_new_lock->pid = task_pid_nr(current);
	p_new_lock->status = PENDING;

	list_add_pending(p_new_lock); // 일단 pending 으로 보냄
	refresh_pending_waiting_lists();
	wait_write_to_acquire();
	wait_read_to_acquire();

	__print_all_lists();

	return 0;
};
