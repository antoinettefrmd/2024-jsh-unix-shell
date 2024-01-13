#include "shell.h"

void add_process(cmd *c, pid_t pid, char *ligne, int print) {
	job j;
	int groupe = c -> all_jobs;
	for(int i = 0; i < c -> all_jobs; i++) {
		if((c -> jobs)[i].groupe == groupe) {
			j = (c -> jobs)[i];
			break;
		}
	}

	if(j.pid[0] == 0) {
		j.pid[0] = pid;
		if(print) print_job(j, 2);
	}

	for(int i = 1; i < j.nb_process + 1; i++) {
		if((j.pid + i) == NULL) {
			j.pid[i] = pid;
			j.etat[i] = "Running";
			j.ligne[i] = ligne;
			break;
		}
	}

	setpgid(pid, j.pid[0]);
}

void add_job(cmd *c, job new) {
	int nb = c -> all_jobs;
	job *newJobs = malloc(nb * sizeof(job));
	if(c -> jobs != NULL) {
		for(int i = 0; i < nb - 1; i++) {
			newJobs[i] = (c -> jobs)[i];
		}
		free(c -> jobs);
	}
    newJobs[nb - 1] = new;
	c -> jobs = newJobs;
}

void	child_process(cmd *c, int *fd, char **envp)
{
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	execute(c, envp);
}

void	parent_process(cmd *c, int *fd, char **envp)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	execute(c, envp);
}

// créé un processus fils pour executer une commande externe
void process(cmd *c, char ** envp, char *ligne, int *fd)
{
    pid_t   pid;
    int     status;

	c -> all_jobs = c -> all_jobs + 1;
	int nb_p = nb_cmd(c);
    job new = {.groupe = c -> all_jobs, .pid = malloc(sizeof(int) * (nb_p + 1)), .etat = malloc(sizeof(char *) * (nb_p + 1)), .ligne = malloc(sizeof(char *) * (nb_p + 1)), .nb_process = nb_p};
    new.pid[0] = 0;
	new.etat[0] = "Running";
    new.ligne[0] = ligne;
    if(c -> bg) {
	    c -> nb_jobs = (c -> nb_jobs) + 1;
    }
	add_job(c, new);

	if (c->next)
	{
		pipe(fd);
	}
	
    pid = fork();
    if (pid == -1)
        error();
    else if (pid == 0) {
		cmprtment_par_defaut();
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
		if (c->next)
			child_process(c, fd, envp);
		else if (is_pipe(ligne))
			parent_process(c, fd, envp);
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
			add_process(c -> origin, pid, ligne, 0);
        	while(1) {
				status = INT_MIN;
            	waitpid(-pid, &status, WUNTRACED);
            	if(status != INT_MIN) {
					if(WIFSTOPPED(status)) {
						new.etat[0] = "Stopped";
						print_job(new, 2);
						c -> nb_jobs = (c -> nb_jobs) + 1;
						break;
					}
					else if (WIFEXITED(status)) {
						new.etat[0] = "Done";
						c->val_retour = WEXITSTATUS(status); // récupère le statut du fils et le stocke dans val_retour
						break;
					}
					else if(WIFSIGNALED(status)) {
						new.etat[0] = "Killed";
                	    c->val_retour = 1;
						break;
					}				
				}
			}
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
		else add_process(c -> origin, pid, ligne, 1);
	}
}
