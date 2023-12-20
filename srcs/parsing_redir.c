#include "shell.h"

int parse_redir(cmd *c) 
{
    int i = 0;
    int prem = 0;
    int res = tablen(c->str_opts) + 1;
    while (c->str_opts[i] != NULL)
    {
        if(c->str_opts[i+1] != NULL)
        {
            if (strcmp("<", c->str_opts[i]) == 0)
            {       
                c->fd_in = open(c->str_opts[i+1], O_RDONLY); 
                if (!prem) {
                    prem = 1;
                    res = i; 
                }
            }
            else if (strcmp(">>", c->str_opts[i]) == 0)
            {
                c->fd_out = open(c->str_opts[i+1], O_WRONLY | O_APPEND | O_CREAT, 0664);
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
                c->fd_err = open(c->str_opts[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0664); 
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
    redir_fic(c);
    return res;
}
