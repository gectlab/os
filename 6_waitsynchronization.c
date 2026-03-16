#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    pid = fork();

    if(pid < 0)
    {
        printf("Fork failed\n");
        return 1;
    }

    else if(pid == 0)
    {
        // Child process
        printf("PCCSL407 ");
    }

    else
    {
        // Parent process
        wait(NULL);   // wait for child to finish
        printf("Operating Systems Lab");
    }

    return 0;
}
