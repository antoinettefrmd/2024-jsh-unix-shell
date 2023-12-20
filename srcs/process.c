#include "shell.h"


void add_job(cmd *c, pid_t pid, char *ligne) {
	int nb = c -> all_jobs;
	job *newJobs = malloc(nb * sizeof(job));
	if(c -> jobs != NULL) {
		for(int i = 0; i < nb - 1; i++) {
			newJobs[i] = (c -> jobs)[i];
		}
		free(c -> jobs);
	}
	job new = {.nb = (c -> all_jobs), .pid = pid, .etat = "Running", .ligne = ligne};
        newJobs[nb - 1] = new;
	print_job(new);
	c -> jobs = newJobs;
}	

// créé un processus fils pour executer une commande externe
void process(cmd *c, char ** envp, char *ligne)
{
    pid_t   pid;
    int     status;

    pid = fork();
    if (pid == -1)
        error();
    else if (pid == 0) {
		int indice_redir = parse_redir(c);
		petit_tab(indice_redir, c);
		if (!(is_builtins(c))) { // regarde si l'arg est une commande interne
        	execute(c, envp); // execute la commande dans le processus fils
		}
        exit(errno);
    }
    else {
        if (!c->bg) {
			free(ligne);
            	waitpid(pid, &status, 0); // attend que le pocessus fils soit fini
			if (WIFEXITED(status)) {
                c->val_retour = WEXITSTATUS(status); // récupère le statut du fils et le stocke dans val_retour
			}
        }
        else {
		c -> nb_jobs = (c -> nb_jobs) + 1;
		c -> all_jobs = (c -> all_jobs) + 1;
		add_job(c, pid, ligne);
	}
    }
}
