#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int parent_to_child[2];
    int child_to_parent[2];
    char buffer;

    // Create pipes
    if (pipe(parent_to_child) < 0 || pipe(child_to_parent) < 0) {
        printf("Pipe creation failed.\n");
        exit(1);
    }

    // Fork a child process
    int pid = fork();
    if (pid < 0) {
        printf("Fork failed.\n");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        close(parent_to_child[1]); // Close write end of parent_to_child pipe
        close(child_to_parent[0]); // Close read end of child_to_parent pipe

        // Read from parent pipe
        read(parent_to_child[0], &buffer, sizeof(buffer));
        printf("%d: received ping\n", getpid());

        // Write to parent pipe
        write(child_to_parent[1], &buffer, sizeof(buffer));

        close(parent_to_child[0]);
        close(child_to_parent[1]);
        
        exit(0);
    } else {
        // Parent process
        close(parent_to_child[0]); // Close read end of parent_to_child pipe
        close(child_to_parent[1]); // Close write end of child_to_parent pipe
        
        buffer = 'A';
        
        // Write to child pipe
        write(parent_to_child[1], &buffer, sizeof(buffer));
        
        // Read from child pipe
        read(child_to_parent[0], &buffer, sizeof(buffer));
        printf("%d: received pong\n", getpid());

        close(parent_to_child[1]);
        close(child_to_parent[0]);
        
        exit(0);
    }

    return 0;
}