#define __NR_ptree 380
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

struct prinfo {
  long state;
  pid_t pid;
  pid_t parent_pid;
  pid_t first_child_pid;
  pid_t next_sibling_pid;
  long uid;
  char comm[64];
};

/* Implement Stack for printing out result*/
struct node {
  pid_t parent_pid;
  struct node *next;
};

void init(struct node *head) { head = NULL; }

struct node *push(struct node *head, pid_t parent_pid, int *p_stack_size) {
  struct node *temp = (struct node *)malloc(sizeof(struct node));
  temp->parent_pid = parent_pid;
  temp->next = head;
  head = temp;
  *p_stack_size = *p_stack_size + 1;
  return head;
}

struct node *pop(struct node *head, int *p_stack_size) {
  struct node *temp = head;
  // pid_t ele = head->parent_pid;
  head = head->next;
  free(temp);
  *p_stack_size = *p_stack_size - 1;
  return head;
}

pid_t peek(struct node *head) { return head->parent_pid; }

struct node *find_location(struct node *stack, struct prinfo *current_process,
                           int *p_stack_size) {
  pid_t prev_parent_pid = stack->parent_pid;
  pid_t curr_parent_pid = current_process->parent_pid;

  if (prev_parent_pid == curr_parent_pid) {
    ;
  } else if (prev_parent_pid < curr_parent_pid) {
    stack = push(stack, curr_parent_pid, p_stack_size);
  } else {
    pid_t top_ele_parent_pid = peek(stack);
    while (top_ele_parent_pid != curr_parent_pid) {
      stack = pop(stack, p_stack_size);
      top_ele_parent_pid = peek(stack);
  }
  }
  return stack;
}

int main() {
  // stack initalization
  struct node *stack = NULL;
  int stack_size = 0;
  init(stack);

  // allocate memory space for buffer && nr
  struct prinfo *buf;
  int bufferNum = 50;
  int *nr = &(bufferNum);
  buf = (struct prinfo *)malloc((*nr) * sizeof(struct prinfo));

  // normal case
  int res = syscall(__NR_ptree, buf, nr);

  if (res < 0) {
    printf("Fail to run syscall(__NR_ptree): %d\n", res);
    return -1;
  }

  struct prinfo *saving_target_buffer;

  for (int i = 0; i < (*nr); i++) {
    saving_target_buffer = (buf + i);
    if (i == 0) {
      stack = push(stack, saving_target_buffer->parent_pid, &stack_size);
    } else {
      stack = find_location(stack, saving_target_buffer, &stack_size);
      for (int j = 0; j < stack_size; j++)
        printf("\t");
    }
    printf("%s, %d, %ld, %d, %d, %d, %ld\n", saving_target_buffer->comm,
           saving_target_buffer->pid, saving_target_buffer->state,
           saving_target_buffer->parent_pid,
           saving_target_buffer->first_child_pid,
           saving_target_buffer->next_sibling_pid, saving_target_buffer->uid);
  }
  printf("Success to run syscall(__NR_ptree): %d\n", res);

  free(buf);
  return 0;
}
