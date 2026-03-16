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
        char *args[] = {"./myadder", "10", "20", NULL};

        execvp(args[0], args);

        // runs only if exec fails
        printf("Exec failed\n");
    }

    else
    {
        // Parent process
        wait(NULL);
        printf("Parent process finished\n");
    }

    return 0;
}
