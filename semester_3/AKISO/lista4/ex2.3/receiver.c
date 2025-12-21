#include <signal.h>
#include <stdio.h>
#include <unistd.h>

volatile sig_atomic_t sigusr1_count = 0;

void handle_sigusr1(int signal) {
    sigusr1_count++;
}

int main() {

    if (signal(SIGUSR1, handle_sigusr1) == SIG_ERR) {
        perror("signal");
        return 1;
    }

    printf("Receiver PID: %d\n", getpid());
    
    sleep(10);

    printf("Otrzymano %d sygnałów SIGUSR1\n", sigusr1_count);

    return 0;
}