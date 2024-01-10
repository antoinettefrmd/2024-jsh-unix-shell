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