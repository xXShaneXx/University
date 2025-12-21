#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <PID> <COUNT>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);
    int count = atoi(argv[2]);

    for (int i = 0; i < count; i++) {
        if (kill(pid, SIGUSR1) == -1) {
            perror("kill");
            return 1;
        }
    }

    return 0;
}