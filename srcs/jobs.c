#include "shell.h"

char *aux(int i) {
	char *res = malloc(4 + i);
	for(int j = 0; j < 3 + i; j++) {
		res[j] = ' ';
	}
	res[3 + i] = '\0';
	return res;
}

void print_job(job j, int sortie, int tree) {
	if(tree) {
		for(int i = 1; i < j.nb_process + 1; i++) {
			int size = strlen(j.ligne[i]) + strlen(j.etat[i]) + nb_digits(j.groupe) + nb_digits(j.pid[i]) + 6;
        	char *s = malloc(size + 1);
        	if(i == 1) sprintf(s, "[%d] %d %s %s\n", j.groupe, j.pid[i], j.etat[i], j.ligne[i]);
			else {
				char *tmp = aux(nb_digits(j.groupe));
				sprintf(s, "%s%d %s %s\n", tmp, j.pid[i], j.etat[i], j.ligne[i]);
				free(tmp);
			}
        	s[size] = '\0';
        	write(sortie, s, size);
        	free(s);
		}
	}
	else {
		int size = strlen(j.ligne[0]) + strlen(j.etat[0]) + nb_digits(j.groupe) + nb_digits(j.pid[0]) + 6;
		char *s = malloc(size + 1);
		sprintf(s, "[%d] %d %s %s\n", j.groupe, j.pid[0], j.etat[0], j.ligne[0]);
		s[size] = '\0';
		write(sortie, s, size);
		free(s);
	}
}

void check_jobs(cmd *c, int sortie) {
	job *jobs = c -> jobs;
	int status = 0;
    for(int i = 0; i < c -> all_jobs; i++) {
		for(int j = 0; j < c -> jobs[i].nb_process + 1; j++) {
        	status = INT_MIN;
        	if(strcmp(jobs[i].etat[j], "Running") == 0 || strcmp(jobs[i].etat[j], "Stopped") == 0) {
        	    if(j == 0) waitpid(-jobs[i].pid[j], &status, WUNTRACED | WNOHANG | WCONTINUED);
				else waitpid(jobs[i].pid[j], &status, WUNTRACED | WNOHANG | WCONTINUED);
        	    if(status != INT_MIN) {
        	        if(WIFEXITED(status)) {
        	            jobs[i].etat[j] = "Done";
						if(j == 0) {
        	            	c -> nb_jobs = c -> nb_jobs - 1;
        	            	print_job(jobs[i], sortie, 0);
						}
        	        }
        	        else if(WIFSIGNALED(status)) {
        	            jobs[i].etat[j] = "Killed";
						kill(-jobs[i].pid[0], WTERMSIG(status));
        	            c -> nb_jobs = c -> nb_jobs - 1;
        	            print_job(jobs[i], sortie, 0);
        	        }
        	        else if(WIFSTOPPED(status)) {
        	            jobs[i].etat[j] = "Stopped";
        	            if(j == 0) print_job(jobs[i], sortie, 0);
        	        }
        	        else if(WIFCONTINUED(status)) {
        	            jobs[i].etat[j] = "Running";
        	            if(j == 0) print_job(jobs[i], sortie, 0);
        	        }
				}
            }
        }
    }
}
