/*
The first process sends a string to the second process. The second process reverses the
received string and sends it back to the first process. The first process compares the original
string and the reversed string received from the second one and then prints whether the
string is a palindrome or not.
  */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main()
{
    int p1[2], p2[2];
    pid_t pid;

    char str[100], rev[100];

    pipe(p1);
    pipe(p2);

    pid = fork();

    if(pid > 0)
    {
        // Parent process
        printf("Enter a string: ");
        scanf("%s", str);

        close(p1[0]); 
        write(p1[1], str, sizeof(str));

        close(p2[1]);
        read(p2[0], rev, sizeof(rev));

        if(strcmp(str, rev) == 0)
            printf("Palindrome\n");
        else
            printf("Not a palindrome\n");
    }

    else
    {
        // Child process
        close(p1[1]);
        read(p1[0], str, sizeof(str));

        int n = strlen(str);

        for(int i = 0; i < n; i++)
            rev[i] = str[n-i-1];

        rev[n] = '\0';

        close(p2[0]);
        write(p2[1], rev, sizeof(rev));
    }

    return 0;
}
