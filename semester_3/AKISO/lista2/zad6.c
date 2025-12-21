#include <stdio.h>

int main() 
{
    for(int i=0; i<256; i++)
    {
      printf("\E[38;5;%dm", i);
      printf("Hello World \n");
    }
    return 0;
}
