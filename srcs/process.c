#include "shell.h"

// créé un processus fils pour executer une commande externe
void process(cmd *c, char ** envp)
{
    pid_t   pid;
    int     status;

    pid = fork();
    if (pid == -1)
        error();
    else if (pid == 0) {
        execute(c, envp); // execute la commande dans le processus fils
        exit(errno);
    }
    else {
        waitpid(pid, &status, 0); // attend que le pocessus fils soit fini
        if (WIFEXITED(status)) {
            c->val_retour = WEXITSTATUS(status);
        }
    }
}