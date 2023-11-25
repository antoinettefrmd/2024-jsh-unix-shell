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
# include <errno.h>
# include <signal.h>
#include <bsd/bsd.h>

//exec functions

void	execute(char *cmd, char **envp);
void    process(char *cmd, char ** envp);
void	error(void);

// string utils functions

char	*strjoin(char const *s1, char const *s2);
char	**split(char const *s, char c);

#endif