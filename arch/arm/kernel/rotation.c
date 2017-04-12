#include <linux/rotation.h>
#include <linux/list.h>

rotation_t rotation;

// struct rotlock_list_node_t pending_
rotlock_t head = {
	.type = HEAD_LOCK, // -1: head, 0: read, 1: write
	.degree = -1,
	.range = -1,
	.pid = -1,
	.pending_lh = LIST_HEAD_INIT(pending_lh),
	.wait_read_lh = LIST_HEAD_INIT(wait_read_lh),
	.wait_write_lh = LIST_HEAD_INIT(wait_write_lh),
	.aquired_lh = LIST_HEAD_INIT(aquired_lh)
};

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
