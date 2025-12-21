#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {
    if (kill(1, SIGKILL) == -1) 
    {
        printf("Can't sent signal SIGKILL to init process: %s\n", strerror(errno));
    } 
    else 
    {
        printf("Signal sent to process init\n");
    }

    return 0;
}