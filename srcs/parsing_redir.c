#include "shell.h"

int parse_redir(cmd *c) 
{
    int i = 0;
    int prem = 0;
    int res = tablen(c->str_opts) + 1;
    while (c->str_opts[i] != NULL) // parcours la commmande pour effectuer les éventuelles redirections
    {
        if(c->str_opts[i+1] != NULL)
        {
            if (strcmp("<", c->str_opts[i]) == 0)
            {       
                c->fd_in = open(c->str_opts[i+1], O_RDONLY); // ouverture du fichier surlequel on va lire à la place de l'entrée standard
                if (!prem) {
                    prem = 1;
                    res = i; 
                }
            }
            else if (strcmp(">>", c->str_opts[i]) == 0) 
            {
                c->fd_out = open(c->str_opts[i+1], O_WRONLY | O_APPEND | O_CREAT, 0664); // ouverture du fichier sur lequel la sortie standard va être redirigée
                if (!prem) {
                    prem = 1;
                    res = i; 
                }
            }
            else if (strcmp(">", c->str_opts[i]) == 0) 
            {
                c->fd_out = open(c->str_opts[i+1], O_WRONLY | O_CREAT | O_EXCL, 0664); 
                if (!prem) {
                    prem = 1;
                    res = i; 
                }

            }
            else if (strcmp(">|", c->str_opts[i]) == 0)
            {
                c->fd_out = open(c->str_opts[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0664);
                if (!prem) {
                    prem = 1;
                    res = i; 
                }
            }
            else if (strcmp("2>|", c->str_opts[i]) == 0) 
            {
                c->fd_err = open(c->str_opts[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0664); // ouverture du fichier sur lequel la sortie erreur va être redirigée
                if (!prem) {
                    prem = 1;
                    res = i; 
                }
            }
            else if (strcmp("2>>", c->str_opts[i]) == 0)
            {
                c->fd_err = open(c->str_opts[i+1], O_WRONLY | O_APPEND | O_CREAT, 0664);
                if (!prem) {
                    prem = 1;
                    res = i; 
                } 
            }
            else if (strcmp("2>", c->str_opts[i]) == 0)
            {
                c->fd_err = open(c->str_opts[i+1], O_WRONLY | O_EXCL | O_CREAT, 0664); 
                if (!prem) {
                    prem = 1;
                    res = i; 
                }
            }
        }
        i++;
    }
    if (c->fd_out == -1 || c->fd_err == -1 || c->fd_in == -1) {
        error_open();
        return -1; 
    }
    if (redir_fic(c)) return -1; // effectue la redirection
    return res;
}
