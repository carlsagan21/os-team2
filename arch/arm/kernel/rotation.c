#include <linux/rotation.h>
#include <linux/list.h>

rotation_t rotation;

// struct rotlock_list_node_t pending_
// struct list_head * pending_lh
// struct list_head * wait_read_lh
// struct list_head * wait_write_lh
// struct list_head * aquired_lh
LIST_HEAD(pending_lh);
LIST_HEAD(wait_read_lh);
LIST_HEAD(wait_write_lh);
LIST_HEAD(aquired_lh);
// struct list_head pending_lh;
// struct list_head wait_read_lh;
// struct list_head wait_write_lh;
// struct list_head aquired_lh;
// INIT_LIST_HEAD(&pending_lh);
// INIT_LIST_HEAD(&wait_read_lh);
// INIT_LIST_HEAD(&wait_write_lh);
// INIT_LIST_HEAD(&aquired_lh);
// struct list_head pending_lh = { &(pending_lh), &(pending_lh) }
// rotlock_t head = {
// 	.type = HEAD_LOCK, // -1: head, 0: read, 1: write
// 	.degree = -1,
// 	.range = -1,
// 	.pid = -1,
// 	.pending_lh = {NULL, NULL},
// 	.wait_read_lh = {NULL, NULL},
// 	.wait_write_lh = {NULL, NULL},
// 	.aquired_lh = {NULL, NULL}
// };

// int waiting_list_refresh(void) { // reconstruct pending_lh, wait_read_lh, wait_write_lh
// list_for_each_entry_safe(type *rotlock_t, head) {
// //pending_lh
// // wait_read_lh
// // wait_write_lh
// }
// return 0;
// };
// int wl_acquire(void) {
// return 0;
// };
// int rl_acquire(void) {
// return 0;
// };

// int exit_rotlock(pid_t pid) {
// return 0;
// }; // kernel/exit.c 의 do_exit() 안에 insert.
