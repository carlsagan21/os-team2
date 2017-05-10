#include <linux/sched.h>
#include <linux/kernel.h>

/*
 * Set the SCHED_WRR weight of process, as identified by 'pid'.
 * If 'pid' is 0, set the weight for the calling process.
 * System call number 380.
 */

int sys_sched_setweight(pid_t pid, int weight){
  printk("[os-team2] sched_setweight");
  return 0;
};
