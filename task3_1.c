#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig) {
    if (sig == SIGINT) {
        printf("\nThis is a special signal handler for SIGINT\n");
    } else if (sig == SIGTSTP) {
        printf("\nThis is a special signal handler for SIGTSTP\n");
    } else if (sig == SIGQUIT) {
        printf("\nThis is a special signal handler for SIGQUIT\n");
    }
}

int main(void) {
    // Daftarkan signal handler untuk setiap isyarat
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("Ralat pendaftaran SIGINT");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGTSTP, signal_handler) == SIG_ERR) {
        perror("Ralat pendaftaran SIGTSTP");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGQUIT, signal_handler) == SIG_ERR) {
        perror("Ralat pendaftaran SIGQUIT");
        exit(EXIT_FAILURE);
    }

    printf("Program sedang berjalan (PID: %d).\n", getpid());
    printf("Tekan Ctrl+C, Ctrl+Z, atau Ctrl+\\ untuk uji...\n");

    // Loop berterusan supaya program tidak terus tamat
    while (1) {
        pause(); // Menunggu sebarang isyarat masuk
    }

    return 0;
}
