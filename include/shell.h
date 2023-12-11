#ifndef SHELL_H
# define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <fcntl.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>
#include <bsd/bsd.h>

typedef struct cmd {
    int val_retour;
    char **str_opts;
    int bg;
    char *chem_jsh;
} cmd; 

//exec functions

void	execute(cmd *c, char **envp);
void    process(cmd *c, char ** envp);
void	error(void);

// string utils functions

char	*strjoin(char const *s1, char const *s2);
char	**split(char const *s, char c);
void    free_cmd(cmd *c, int free_all);
char    *last_cmd(char **cmd);
int	    tablen(char **cmd);

// builtins fonctions

int is_builtins(cmd *c);
int pwd ();
void print_val_ret(int val) ;
int cd(char *ref);
void exit_maison ();

// prompt fonctions

char * prompt();

#endif