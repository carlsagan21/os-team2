#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/sched.h>
#include <linux/list.h>

/*
 * Release a read/or write lock using the given rotation range
 * returning 0 on success, -1 on failure.
 * system call numbers 383 and 384
 */
int sys_rotunlock_read(int degree, int range) /* 0 <= degree < 360 , 0 < range < 180 */
{
	int pid;
	// rotlock_t *p_deleted_rotlock;

	pr_debug("[soo] sys_rotunlock_read\n");

	pid = task_pid_nr(current);

	/*
	 * mutually exclusive 한 리스트 수정.
	 * kill, interrupt 를 막음.
	 * 리스트에서 찾아서 삭제하고, 리스트를 재조정함.
	 */
	mutex_lock(&rotlock_mutex);
	// p_deleted_rotlock = delete_lock(READ_LOCK, degree, range, pid);
	delete_lock(READ_LOCK, degree, range, pid);
	refresh_pending_waiting_lists();
	wait_write_to_acquire();
	wait_read_to_acquire();

	__print_all_lists();
	mutex_unlock(&rotlock_mutex);

	pr_debug("[soo] wake up all\n");

	// if (is_rotlock_deleted(p_deleted_rotlock)) {
	// 	kfree(p_deleted_rotlock);
	// }

	/**
	 * wait_event_interruptible로 인해 자고있는 interruped 된 것들을 모두 깨움. 깨워서 컨디션 체크 유도.
	 * 새롭게 acquired 된 것들이 있으면 wakeup.
	 */
	wake_up_interruptible_all(&wq_rotlock);

	return 0;
};
