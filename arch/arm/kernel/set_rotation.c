// #include <linux/unistd.h>
#include <linux/kernel.h> // 지우면 불완전하게나마 lint 사용가능
#include <linux/rotation.h>
// #include <linux/sched.h>
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
	printk("degree: %d\n", rotation.degree);

	// rotation *rot;
	// rot = (struct rotation *)kmalloc(sizeof(struct rotation), GFP_KERNEL);

	// if (k_buf == NULL) /* kmalloc 은 NULL 로 제대로 되었는지 여부 판단 */
	// return -ENOMEM;
	return 0;
};
