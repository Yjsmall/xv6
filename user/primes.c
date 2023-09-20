#include "kernel/types.h"
#include "user/user.h"

#define WRITE 1
#define READ 0
#define stdin 0
#define stdout 1
#define stderr 2

int
main(int argc, char *argv[]) {
  int pid, fd[2];
  pipe(fd);
  if ((pid = fork()) < 0) {
    fprintf(stderr, "primes: fork failed\n");
    exit(1);
  } else if (pid > 0) {
    close(READ);
    // The first process feeds the numbers 2 through 35 into the pipeline.
    for (int i = 2; i <= 35; i++) {
      write(fd[WRITE], &i, sizeof(int));
    }
    close(fd[WRITE]);
    // parentit should wait until the entire pipeline terminates, 
    // including all children, grandchildren
    wait(0);
    exit(0);
  } else {
  }
  return 0;
}
