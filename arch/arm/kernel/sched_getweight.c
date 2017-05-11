#include <linux/sched.h>
#include <linux/kernel.h>

/*
 * Obtain the SCHED_WRR weight of a process as identified by 'pid'.
 * If 'pid' is 0, return the weight of the calling process.
 * System call number 381.
 */

int sys_sched_getweight(pid_t pid){
  printk("[os-team2] sched_getweight");
  return 0;
};
