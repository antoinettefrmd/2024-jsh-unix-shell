#include "shell.h"


void add_job(cmd *c, pid_t pid, char *ligne, int run) {
	int nb = c -> all_jobs;
	job *newJobs = malloc(nb * sizeof(job));
	if(c -> jobs != NULL) {
		for(int i = 0; i < nb - 1; i++) {
			newJobs[i] = (c -> jobs)[i];
		}
		free(c -> jobs);
	}
	job new = {.groupe = (c -> all_jobs), .pid = pid, .etat = "Stopped", .ligne = ligne};
	if(run) new.etat = "Running";
        newJobs[nb - 1] = new;
	if(run) print_job(new, 2);
	c -> jobs = newJobs;
}	

// créé un processus fils pour executer une commande externe
void process(cmd *c, char ** envp, char *ligne)
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
			job new = {.groupe = 0, .pid = pid, .etat = "Running", .ligne = ligne};
        		while(1) {
				status = INT_MIN;
                        	waitpid(-pid, &status, WUNTRACED | WNOHANG);
                        	if(status != INT_MIN) {
					if(WIFSTOPPED(status)) {
						new.etat = "Stopped";
						print_job(new, 2);
						c -> nb_jobs = (c -> nb_jobs) + 1;
						c -> all_jobs = (c -> all_jobs) + 1;
						add_job(c, pid, strdup(ligne), 0);
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
		add_job(c, pid, ligne, 1);
	}
    }
}
