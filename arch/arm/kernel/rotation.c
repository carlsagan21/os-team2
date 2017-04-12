#include <linux/kernel.h>
#include <linux/rotation.h>
#include <linux/list.h>

rotation_t rotation;

// struct rotlock_list_node_t pending_
// struct list_head * pending_lh
// struct list_head * wait_read_lh
// struct list_head * wait_write_lh
// struct list_head * acquired_lh
LIST_HEAD(pending_lh);
LIST_HEAD(wait_read_lh);
LIST_HEAD(wait_write_lh);
LIST_HEAD(acquired_lh);
// struct list_head pending_lh;
// struct list_head wait_read_lh;
// struct list_head wait_write_lh;
// struct list_head acquired_lh;
// INIT_LIST_HEAD(&pending_lh);
// INIT_LIST_HEAD(&wait_read_lh);
// INIT_LIST_HEAD(&wait_write_lh);
// INIT_LIST_HEAD(&acquired_lh);
// struct list_head pending_lh = { &(pending_lh), &(pending_lh) }
// rotlock_t head = {
// 	.type = HEAD_LOCK, // -1: head, 0: read, 1: write
// 	.degree = -1,
// 	.range = -1,
// 	.pid = -1,
// 	.pending_lh = {NULL, NULL},
// 	.wait_read_lh = {NULL, NULL},
// 	.wait_write_lh = {NULL, NULL},
// 	.acquired_lh = {NULL, NULL}
// };
int is_range_contains_rotation(int degree, int range, int rotation)
{
        int a = degree - range;
        int b = degree + range;

        int rot_minus = rotation - 360;
        int rot_plus = rotation + 360;

        if( (a<rotation & rotation <b) | (a<rot_minus & rot_minus<b) | (a<rot_plus & rot_plus<b)) return 1;
        else return 0;
}


rotlock_t *p_lock;
rotlock_t *p_temp_lock;

int waiting_list_refresh(void) { // reconstruct pending_lh, wait_read_lh, wait_write_lh
  list_for_each_entry_safe(p_lock, p_temp_lock, &pending_lh, pending_lh) {
    // range 가 포함하면, read / write
    if (is_range_contains_rotation(p_lock->degree, p_lock->range, rotation)) {
      if (p_lock->type == READ_LOCK) {
        list_move(&(p_lock->list_node), &wait_read_lh);
      } else if (p_lock->type == WRITE_LOCK) {
        list_move(&(p_lock->list_node), &wait_write_lh);
      } else {
        printk(KERN_DEBUG "[soo] waiting_list_refresh: invalid type")
      }
    }
  }
  list_for_each_entry_safe(p_lock, p_temp_lock, &wait_read_lh, wait_read_lh) {

  }
  list_for_each_entry_safe(p_lock, p_temp_lock, &wait_write_lh, wait_write_lh) {

  }
  // acquire 는 안돈다
  return 0;
};
// int wl_acquire(void) {
// return 0;
// };
// int rl_acquire(void) {
// return 0;
// };

// int exit_rotlock(pid_t pid) {
// return 0;
// }; // kernel/exit.c 의 do_exit() 안에 insert.
