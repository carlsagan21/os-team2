#include <sys/types.h>
#include <linux/list.h>

// type
#define HEAD_LOCK -1
#define READ_LOCK 0
#define WRITE_LOCK 1

typedef struct rotation_t {
	int degree;
} rotation_t;

extern rotation_t rotation;

typedef struct rotlock_t { // task_struct
	int type; // 0: read, 1: write
	int degree;
	int range;
	pid_t pid;
	struct list_head pending_lh;
	struct list_head wait_read_lh;
	struct list_head wait_write_lh;
	struct list_head aquired_lh;
} rotlock_t;

extern rotlock_t head;

// struct rotlock_list_node_t {
// struct rotlock_t lock;
// struct list_head head;
// };

int waiting_list_refresh(void);
int wl_acquire(void);
int rl_acquire(void);

int exit_rotlock(pid_t pid);
