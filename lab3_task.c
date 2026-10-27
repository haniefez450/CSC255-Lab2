#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

// Signal handler untuk SIGINT (Ctrl + C)
void sigint_handler(int sig) {
    printf("\n[Signal Caught] SIGINT (Ctrl+C) received. Exiting program...\n");
    exit(0);
}

// Fungsi menyemak nombor perdana
bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

int main(void) {
    // Daftar pengendali SIGINT
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Signal registration error");
        exit(EXIT_FAILURE);
    }

    int pipefds[2];
    if (pipe(pipefds) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // --- PROSES ANAK (CHILD) ---
        close(pipefds[0]); // Tutup bahagian membaca

        int number;
        printf("[Child PID %d] Enter an integer number: ", getpid());
        if (scanf("%d", &number) != 1) {
            fprintf(stderr, "Invalid input!\n");
            close(pipefds[1]);
            exit(EXIT_FAILURE);
        }

        // Hantar nombor ke parent melalui pipe
        write(pipefds[1], &number, sizeof(number));
        close(pipefds[1]);
        exit(EXIT_SUCCESS);
    } else {
        // --- PROSES INDUK (PARENT) ---
        close(pipefds[1]); // Tutup bahagian menulis

        // Tunggu proses anak selesai membaca dan menghantar
        wait(NULL);

        int received_number;
        read(pipefds[0], &received_number, sizeof(received_number));
        close(pipefds[0]);

        printf("[Parent PID %d] Received number from child: %d\n", getpid(), received_number);

        if (isPrime(received_number)) {
            printf("[Parent PID %d] Result: %d is a PRIME number.\n", getpid(), received_number);
        } else {
            printf("[Parent PID %d] Result: %d is NOT a prime number.\n", getpid(), received_number);
        }
    }

    return EXIT_SUCCESS;
}
