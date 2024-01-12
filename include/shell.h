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

typedef struct job {
	pid_t groupe;
	pid_t pid;
	char *etat;
	char *ligne;
} job;

typedef struct cmd {
    int val_retour;
    char **str_opts;
    int bg;
    char *chem_jsh;
    int nb_jobs;
    pid_t all_jobs;
    job *jobs;
    int fd_in;
    int fd_out;
    int fd_err;
    struct cmd *next;
    int nb_c;
} cmd; 

// signaux functions

void ignore_signals();
void cmprtment_par_defaut();

//parsing_redir
int     parse_redir(cmd *c);
int     redir_fic(cmd *c);
char    **pipe_split(char *str);
cmd     *parsing_pipe(char *str, cmd *commande);
int     nb_cmd(cmd *c);

//exec functions
void	execloop(cmd *commande, char *ligne, char **envp);
void	execute(cmd *c, char **envp);
void    process(cmd *c, char ** envp, char *ligne, int *fd);
void	error(void);

// string utils functions

char	*strjoin(char const *s1, char const *s2);
char	**split(char const *s, char c);
void    free_cmd(cmd *c, int free_all);
char    *last_cmd(char **cmd);
int	    tablen(char **cmd);
void    petit_tab(int i, cmd *c);   
int	    is_pipe(char *str);  
void    error_open(); 
void	print_cmd(cmd *c);  

// int utils functions

int nb_digits(int n);

// builtins fonctions

int is_builtins(cmd *c);
int builtins(cmd *c);
int pwd ();
void print_val_ret(int val) ;
int cd(char *ref);
void exit_maison (cmd *c);
void jobs (cmd *c);
void kill_maison(cmd *c);
void fg(cmd *c);
void bg(cmd *c);

// prompt fonctions

char * prompt(cmd *c);

// jobs fonctions

void print_job(job j, int sortie);
void check_jobs(cmd *c, int sortie);

#endif
