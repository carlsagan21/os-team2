// #include <linux/unistd.h>
#include <linux/kernel.h>
// #include <linux/sched.h>
// #include <linux/list.h>
// #include <linux/cred.h> /* for task_uid() */
// #include <linux/slab.h> /* for kmalloc(), GFP_KERNEL. linux/gfp 따로 include 안해도 됨. */
// #include <linux/uaccess.h> /* for copy_from_user, copy_to_user, strncpy */
// #include <linux/errno.h>

/*
 * sets current device rotation in the kernel.
 * syscall number 380 (you may want to check this number!)
 */
int sys_set_rotation(int degree) /* 0 <= degree < 360 */
{
	printk("[soo] sys_set_rotation\n");
	return 0;
};
