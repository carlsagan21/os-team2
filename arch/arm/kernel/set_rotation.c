// #include <linux/unistd.h>
#include <linux/kernel.h> // 지우면 불완전하게나마 lint 사용가능
#include <linux/rotation.h>
#include <linux/sched.h>
// #include <linux/list.h>
// #include <linux/cred.h> /* for task_uid() */
// #include <linux/slab.h> /* for kmalloc(), GFP_KERNEL. linux/gfp 따로 include 안해도 됨. */
// #include <linux/uaccess.h> /* for copy_from_user, copy_to_user, strncpy */
#include <linux/errno.h>

/*
 * sets current device rotation in the kernel.
 * syscall number 380 (you may want to check this number!)
 */
int sys_set_rotation(int degree) /* 0 <= degree < 360 */
{
	if (degree < 0 || degree >= 360)
		return -EINVAL;

	rotation.degree = degree;
	printk("degree: %d, %d\n", rotation.degree, task_pid_nr(current));

	return 0;
};
