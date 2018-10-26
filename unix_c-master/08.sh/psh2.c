/*
 * prompting shell version 2
 * Solves the 'one-shot' problem of version 1
 * Uses execvp(), but fork()s first so that the
 * shell waits around to perform another command
 * New problem: shell catches signals. Run vi, press ^c
 */

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#define MAXARGS 20      // cmdline args
#define ARGLEN  100     // token length

int main()
{
    char *arglist[MAXARGS + 1]; // an array of ptrs
    int numargs;                // index into array
    char argbuf[ARGLEN];        // read stuff here
    char * makestring();        // malloc etc

    numargs = 0;
    while (numargs < MAXARGS)
    {
        printf("Arg[%d]?", numargs);
        if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
            arglist[numargs ++] = makestring(argbuf);
        else
        {
            if (numargs > 0)
            {
                arglist[numargs] = NULL;    // close list
                execute(arglist);
                numargs = 0;
            }
        }
    }
    return 0;
}

int execute(char *arglist[])
/*
 * use fork and execvp and wait to do it
 */
{
    int pid, exitstatus;

    pid = fork();
    switch(pid)
    {
        case -1:
            perror("fork failed");
            exit(1);
        case 0:
            execvp(arglist[0], arglist); //成功直接结束当前进程
			sleep(3);
            perror("execvp failed");
            exit(1);
        default:
            while (wait(&exitstatus) != pid)
                ;
            printf("child exited with status %d, %d\n",
                    exitstatus>>8, exitstatus & 0377);
    }
}

char * makestring(char *buf)
/*
 * trim off newline and create storage for the string
 */
{
    char *cp, *malloc();

    buf[strlen(buf) - 1] = '\0';    // trim newline
    cp = malloc(strlen(buf) + 1);   // get memory
    if (cp == NULL)
    {
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}
