#include "shell.h"

void ignore_signals() {
    struct sigaction action = {0};
    action.sa_handler = SIG_IGN; 
    
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGTTIN, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
    sigaction(SIGTTOU, &action, NULL);
    sigaction(SIGTSTP, &action, NULL);
}

void cmprtment_par_defaut()
{
    struct sigaction action = {0};
    action.sa_handler = SIG_DFL; 
    
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGTTIN, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
    sigaction(SIGTTOU, &action, NULL);
    sigaction(SIGTSTP, &action, NULL);
}

void give_fg(pid_t pid) {
	sigset_t *set = malloc(sizeof(sigset_t));
    sigemptyset(set);
    sigaddset(set, SIGTTIN);
    sigaddset(set, SIGTTOU);
    sigprocmask(SIG_BLOCK, set, NULL);
    tcsetpgrp(0, pid);
    tcsetpgrp(1, pid);
    tcsetpgrp(2, pid);
    sigprocmask(SIG_UNBLOCK, set, NULL);
    free(set);
}