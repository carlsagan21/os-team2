#include <linux/types.h>
#include <linux/list.h>

// type
#define READ_LOCK 0
#define WRITE_LOCK 1

#define PENDING 0
#define READ_WAITING 1
#define WRITE_WAITING 2
#define ACQUIRED 3

typedef struct rotation_t {
	int degree;
} rotation_t;

extern rotation_t rotation;

typedef struct rotlock_t { // task_struct
	int type; // 0: read, 1: write
	int degree;
	int range;
	pid_t pid;
	struct list_head list_node;
	int status;
} rotlock_t;

// extern rotlock_t head;

extern struct list_head pending_lh;
extern struct list_head wait_read_lh;
extern struct list_head wait_write_lh;
extern struct list_head acquired_lh;

// struct rotlock_list_node_t {
// struct rotlock_t lock;
// struct list_head head;
// };
int is_range_contains_rotation(int degree, int range, int rotation);

int waiting_list_refresh(void);
int wl_acquire(void);
int rl_acquire(void);

int exit_rotlock(pid_t pid);