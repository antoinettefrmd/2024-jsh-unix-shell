#include "shell.h"


void add_job(cmd *c, pid_t pid, char *ligne, char *etat, int print) {
	int nb = c -> all_jobs;
	job *newJobs = malloc(nb * sizeof(job));
	if(c -> jobs != NULL) {
		for(int i = 0; i < nb - 1; i++) {
			newJobs[i] = (c -> jobs)[i];
		}
		free(c -> jobs);
	}
	job new = {.groupe = (c -> all_jobs), .pid = malloc(sizeof(int)), .etat = malloc(sizeof(char *)), .ligne = malloc(sizeof(char *)), .nb_process = 1};
	new.pid[0] = pid;
	new.etat[0] = etat;
	new.ligne[0] = ligne;
    newJobs[nb - 1] = new;
	if(print) print_job(new, 2);
	c -> jobs = newJobs;
}

void	child_process(cmd *c, int **fd, int i, char **envp)
{
	if (i) {
		dup2(fd[i - 1][0], STDIN_FILENO);
	}
	if (c->next) {
		dup2(fd[i][1], STDOUT_FILENO);
	}
	close_pipes(fd);
	execute(c, envp);
}
/*
void	parent_process(cmd *c, int **fd, int i, char **envp)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	execute(c, envp);
}
*/
// créé un processus fils pour executer une commande externe
void process(cmd *c, char ** envp, char *ligne, int **fd, int i)
{
    pid_t   pid;
    int     status;

    if(c -> bg) {
	    c -> nb_jobs = (c -> nb_jobs) + 1;
	    c -> all_jobs = (c -> all_jobs) + 1;
    }
	
    pid = fork();
    if (pid == -1)
        error();
    else if (pid == 0) {
		cmprtment_par_defaut();
		setpgid(getpid(), 0);
		if(!c->bg) {
			sigset_t *set = malloc(sizeof(sigset_t));
			sigemptyset(set);
			sigaddset(set, SIGTTIN);
			sigaddset(set, SIGTTOU);
			sigprocmask(SIG_BLOCK, set, NULL);
			tcsetpgrp(0, getpid());
			tcsetpgrp(1, getpid());
			tcsetpgrp(2, getpid());
			sigprocmask(SIG_UNBLOCK, set, NULL);
			free(set);
		}
		if (c->next || is_pipe(ligne))
			child_process(c, fd, i, envp);
		int indice_redir = parse_redir(c);
		if (indice_redir == -1) exit(1);
		petit_tab(indice_redir, c);
        execute(c, envp); // execute la commande dans le processus fils
        exit(errno);
    }
    else
	{
        if (!c->bg) 
		{
        	while(1) {
				status = INT_MIN;
            	waitpid(-pid, &status, WUNTRACED);
            	if(status != INT_MIN) {
					if(WIFSTOPPED(status)) {
						c -> nb_jobs = (c -> nb_jobs) + 1;
						c -> all_jobs = (c -> all_jobs) + 1;
						add_job(c, pid, strdup(ligne), "Stopped", 1);
						break;
					}
					else if (WIFEXITED(status)) {
						c->val_retour = WEXITSTATUS(status); // récupère le statut du fils et le stocke dans val_retour
						break;
					}
					else if(WIFSIGNALED(status)) {
                	    c->val_retour = 1;
						break;
					}				
				}
			}
			free(ligne);
			sigset_t *set = malloc(sizeof(sigset_t));
            sigemptyset(set);
         	sigaddset(set, SIGTTIN);
            sigaddset(set, SIGTTOU);
           	sigprocmask(SIG_BLOCK, set, NULL);
           	tcsetpgrp(0, getpid());
           	tcsetpgrp(1, getpid());
            tcsetpgrp(2, getpid());
          	sigprocmask(SIG_UNBLOCK, set, NULL);
           	free(set);
        }
        else {
			if(c -> next) {
				add_job(c, pid, ligne, "Running", 0);
			}
		else add_job(c, pid, ligne, "Running", 1);
		}
	}
}
