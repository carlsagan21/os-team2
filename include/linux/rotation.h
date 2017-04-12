#include <linux/types.h>
#include <linux/list.h>

// type
#define READ_LOCK 0
#define WRITE_LOCK 1

#define PENDING 0
#define WAIT_READ 1
#define WAIT_WRITE 2
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
	int status;
	struct list_head list_node;
} rotlock_t;

extern struct list_head pending_lh;
extern struct list_head wait_read_lh;
extern struct list_head wait_write_lh;
extern struct list_head acquired_lh;

int is_range_contains_rotation(int degree, int range, int rotation);

int waiting_list_refresh(void);
int wl_acquire(void);
int rl_acquire(void);

int exit_rotlock(pid_t pid);
