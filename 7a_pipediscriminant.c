/*
Using Pipe – Evaluate the expression √b2−4 ac The first process evaluates b2. The
second process evaluates 4ac . and sends it to the first process which evaluates the final
expression and displays it.
*/

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>

int main()
{
    int fd[2];
    pid_t pid;

    int a,b,c;
    int fourac;
    int b2;

    printf("Enter a b c: ");
    scanf("%d %d %d",&a,&b,&c);

    pipe(fd);

    pid = fork();

    if(pid == 0)
    {
        // Child process
        close(fd[0]);   // close read end

        fourac = 4*a*c;

        write(fd[1], &fourac, sizeof(fourac));
        close(fd[1]);
    }
    else
    {
        // Parent process
        close(fd[1]);   // close write end

        b2 = b*b;

        read(fd[0], &fourac, sizeof(fourac));
        close(fd[0]);

        double result = sqrt(b2 - fourac);

        printf("sqrt(b^2 - 4ac) = %.2f\n", result);
    }

    return 0;
}
