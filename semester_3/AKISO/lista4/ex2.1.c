#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void handle_signal(int signal)
{
    printf("Signal %d (%s) received\n", signal, strsignal(signal));
}

int main()
{
    for (int i = 1; i <= 64; i++)
    {
        if (signal(i, handle_signal) == SIG_ERR)
        {
            printf("Signal %d (%s) cannot be caught\n", i, strsignal(i));
        }
    }

    while (1)
    {
        pause(); 
    }

    return 0;
}