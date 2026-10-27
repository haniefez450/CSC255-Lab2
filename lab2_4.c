#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void askName(int child_num) {
    char name[100];
    printf("[Child %d - PID %d] Enter your name: ", child_num, getpid());
    fflush(stdout);
    if (scanf(" %99[^\n]", name) == 1) {
        printf("[Child %d - PID %d] Hello, %s!\n", child_num, getpid(), name);
    }
}

int main(void) {
    int num_children = 4;

    for (int i = 1; i <= num_children; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process executes function and exits
            askName(i);
            exit(EXIT_SUCCESS);
        } else {
            // Parent waits for the child to finish before moving to the next
            wait(NULL);
        }
    }

    // After all 4 children are done
    printf("Job is done\n");
    return EXIT_SUCCESS;
}
