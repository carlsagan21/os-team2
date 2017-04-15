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
	pr_debug("[soo] sys_set_rotation: %d, %d\n", rotation.degree, task_pid_nr(current));

	mutex_lock(&rotlock_mutex); // kill, interrupt 를 막아버림.
	refresh_pending_waiting_lists();
	wait_write_to_acquire();
	wait_read_to_acquire();

	__print_all_lists();
	mutex_unlock(&rotlock_mutex);
	// TODO 새롭게 acquired 된 것들이 있으면 wakeup
	pr_debug("[soo] wake up all\n");
	wake_up_interruptible_all(&wq_rotlock);

	return 0;
};
