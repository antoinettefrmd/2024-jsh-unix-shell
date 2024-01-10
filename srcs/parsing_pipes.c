#include "shell.h"

cmd    *parsing_pipe(char *str, cmd *commande)
{
    char    **pipes = split(str, '|');
    cmd *tmp = commande;
    for (int i = 0; i < tablen(pipes) + 1; i++) {

        commande->str_opts = split(pipes[i], ' ');
        free(pipes[i]);
        if (pipes[i + 1])
        {
            commande->next = malloc(sizeof(cmd));
            commande->next-> nb_jobs = 0;
            commande -> next -> all_jobs = 0;
            commande -> next -> jobs = NULL;
            commande -> next -> val_retour = 0;
            commande -> next -> fd_in = 0;
            commande -> next -> fd_out = 1;
            commande -> next -> fd_err = 2;
            commande -> next -> nb_c = commande -> nb_c;
            commande = commande->next;
        }
    }
    commande->next = NULL;
    free(pipes);
    return tmp;
}

int nb_cmd(cmd *c) {
    cmd *tmp;
    int i = 0;

    tmp = c;
    while (tmp) {
        i++;
        tmp = tmp->next;
    }
    return i;
}