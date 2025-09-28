#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handler(int in_signal)
{
    if (in_signal == SIGINT) 
    {
        write(STDOUT_FILENO, "Yeah!\n", 6);  
        signal(SIGINT, handler);
    }
    if (in_signal == SIGHUP) 
    {
        write(STDOUT_FILENO, "Ouch!\n", 6); 
        signal(SIGHUP, handler); 
    }
}

int main(int argc, char *argv[]){
    int n = atoi(argv[1]);
    signal(SIGINT, handler);
    signal(SIGHUP, handler);
    for (int i = 0; i < n; i++) {
        printf("%d\n", i * 2);
        sleep(5);
    }
    return 0;
}