#include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/cred.h> /* for task_uid() */
#include <linux/slab.h> /* for kmalloc(), GFP_KERNEL. linux/gfp 따로 include 안해도 됨. */
#include <linux/uaccess.h> /* for copy_from_user, copy_to_user, strncpy */
#include <linux/errno.h>

/*
 * Take a read/or write lock using the given rotation range
 * returning 0 on success, -1 on failure.
 * system call numbers 381 and 382
 */
int sys_rotlock_write(int degree, int range) /* degree - range <= LOCK RANGE <= degree + range */
{
	printk("[soo] rotlock_write");
	return 0;
};
