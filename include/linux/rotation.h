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

int is_unlock_match(rotlock_t *lock1, int type, int degree, int range, int pid);

int refresh_pending_waiting_lists(void);
int wait_write_to_acquire(void);
int wait_read_to_acquire(void);

int delete_lock(int type, int degree, int range, int pid);

int exit_rotlock(pid_t pid);
