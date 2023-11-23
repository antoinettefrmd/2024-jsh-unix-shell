#include "shell.h"

void    process(char *cmd, char ** envp)
{
    pid_t   pid;
    int     status;

    pid = fork();
    if (pid == -1)
        error();
    else if (pid == 0)
        execute(cmd, envp);
    else
        waitpid(pid, &status, 0);
}