#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/list.h>

rotation_t rotation;

LIST_HEAD(pending_lh);
LIST_HEAD(wait_read_lh);
LIST_HEAD(wait_write_lh);
LIST_HEAD(acquired_lh);

int is_unlock_match(rotlock_t *lock1, int type, int degree, int range, int pid) {
	if (
		(lock1->type == type) &&
		(lock1->degree == degree) &&
		(lock1->range == range) &&
		(lock1->pid == pid)
	) {
		return 1;
	} else {
		return 0;
	}
}

int is_range_contains_rotation(int degree, int range, int rotation) {
	int a = degree - range;
	int b = degree + range;

	int rot_minus = rotation - 360;
	int rot_plus = rotation + 360;

	if(
		(a < rotation && rotation <b) ||
		(a < rot_minus && rot_minus < b) ||
		(a < rot_plus && rot_plus < b)
	)
		return 1;
	else
		return 0;
}

int is_not_overlaped(int deg1, int rang1, int deg2, int rang2)
{
				int a = (deg1-rang1)<0? (deg1-rang1)+360 : (deg1 - rang1);
				int b = (deg1 + rang1)%360;
				int c = (deg2-rang2)<0? (deg2-rang2)+360 : (deg2 - rang2);
				int d = (deg2 + rang2)%360;

				//printf("%d,%d,%d,%d\n",a,b,c,d);
				return	(
								(a<b)&&
								( (c<d && ((c<a&&d<a)||(c>a&&d>a))) || (c>d && (c>b&&d<a)))
								)
						||	(a>b)&&
								(c<d && (c>b&&d<a));
}


rotlock_t *p_lock;
rotlock_t *p_temp_lock;

int waiting_list_refresh(void) { // reconstruct pending_lh, wait_read_lh, wait_write_lh
	list_for_each_entry_safe(p_lock, p_temp_lock, &pending_lh, list_node) {
		// range 가 포함하면, read / write
		if (is_range_contains_rotation(p_lock->degree, p_lock->range, rotation.degree)) {
			if (p_lock->type == READ_LOCK) {
				list_move_tail(&(p_lock->list_node), &wait_read_lh);
				p_lock->status = WAIT_READ;
			} else if (p_lock->type == WRITE_LOCK) {
				list_move_tail(&(p_lock->list_node), &wait_write_lh);
				p_lock->status = WAIT_WRITE;
			} else {
				printk(KERN_DEBUG "[soo] waiting_list_refresh: invalid type");
			}
		}
	}
	list_for_each_entry_safe(p_lock, p_temp_lock, &wait_read_lh, list_node) {
		if (!is_range_contains_rotation(p_lock->degree, p_lock->range, rotation.degree)) {
			list_move_tail(&(p_lock->list_node), &pending_lh);
			p_lock->status = PENDING;
		}
	}
	list_for_each_entry_safe(p_lock, p_temp_lock, &wait_write_lh, list_node) {
		if (!is_range_contains_rotation(p_lock->degree, p_lock->range, rotation.degree)) {
			list_move_tail(&(p_lock->list_node), &pending_lh);
			p_lock->status = PENDING;
		}
	}
	return 0;
};


int waitingLockAcquireLock(rotlock_t *waiting_lock,acquired_lock){
	/*
	 * Check whether degree/range of waiting_lock_node overlaps with that of acquired_lock
	 * if not overlapped -> 1 , else -> 0
	 * Regarding Reader Case, reader should look at acquired_lock_list if there is writer lock which has same degree/range
	 * Writer Case You don't need to
	 */

	//Reader Case
	if(waiting_lock->type==0){
		if(acquired_lock->type==0) return 1;
		else return is_not_overlapped(waiting_lock->degree,waiting_lock->range,acquired_lock->degree,acquired_lock->range);
	}
	//Writer Case
	else	return is_not_overlapped(waiting_lock->degree,waiting_lock->range,acquired_lock->degree,acquired_lock->range);
};

 int wl_acquire(void) {
	 rotlock_t *waiting_lock;
	 rotlock_t *waiting_safe_lock;
	 rotlock_t *acquired_lock;
	 rotlock_t *acquired_safe_lock;

	 list_for_each_entry_safe(waiting_lock,waiting_safe_lock,&wait_write_lh,list_node){
		 list_for_each_entry_safe(acquired_lock,acquired_safe_lock,&acquired_lh,list_node){
			 if(waitingLockAcquireLock(waiting_lock,acquired_lock)==1){
				 //remove from waiting list and append to acquired lock list
				 list_move_tail(&acquired_lh,waiting_lock);
				 //Finish procedure
				 return 0;
			 }
		 }
	 }

	 return 0;
 };

 int rl_acquire(void){
	rotlock_t *waiting_lock;
	rotlock_t *waiting_safe_lock;
	rotlock_t *acquired_lock;
	rotlock_t *acquired_safe_lock;

	list_for_each_entry_safe(waiting_lock,waiting_safe_lock,&wait_write_lh,list_node){
		list_for_each_entry_safe(acquired_lock,acquired_safe_lock,&acquired_lh,list_node){
			if(waitingLockAcquireLock(waiting_lock,acquired_lock)==1){
				list_move_tail(&acquired_lh,waiting_lock);
			}
		}
	}

	return 0;
};

// int exit_rotlock(pid_t pid) {
// return 0;
// }; // kernel/exit.c 의 do_exit() 안에 insert.
