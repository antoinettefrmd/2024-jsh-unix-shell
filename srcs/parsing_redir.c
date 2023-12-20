#include "shell.h"

int parse_redir(cmd *c) 
{
    int i = 0;
    while (c->str_opts[i] != NULL)
    {
        if(c->str_opts[i+1] != NULL)
        {
            if (strcmp("<", c->str_opts[i]) == 0)
            {       
                c->fd_in = open(c->str_opts[i+1], O_RDONLY); break;
            }
            else if (strcmp(">>", c->str_opts[i]) == 0)
            {
                c->fd_out = open(c->str_opts[i+1], O_WRONLY | O_APPEND | O_CREAT, 0644); break;
            }
            else if (strcmp(">", c->str_opts[i]) == 0)
            {
                c->fd_out = open(c->str_opts[i+1], O_WRONLY | O_CREAT | O_EXCL, 0644); break;
            }
            else if (strcmp("|>", c->str_opts[i]) == 0)
            {
                c->fd_out = open(c->str_opts[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0644); break;
            }
            else if (strcmp("2|>", c->str_opts[i]) == 0)
            {
                c->fd_err = open(c->str_opts[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0644); break;
            }
            else if (strcmp("2>>", c->str_opts[i]) == 0)
            {
                c->fd_err = open(c->str_opts[i+1], O_WRONLY | O_APPEND | O_CREAT, 0644); break;
            }
            else if (strcmp("2>", c->str_opts[i]) == 0)
            {
                c->fd_err = open(c->str_opts[i+1], O_WRONLY | O_EXCL | O_CREAT, 0644); break;
            }
        }
        i++;
    }
    redir_fic(c);
    return i;
}
