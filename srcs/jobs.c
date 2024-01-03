#include "shell.h"

void print_job(job j, int sortie) {
	int size = strlen(j.ligne) + strlen(j.etat) + nb_digits(j.groupe) + nb_digits(j.pid) + 6;
	char *s = malloc(size + 1);
	sprintf(s, "[%d] %d %s %s\n", j.groupe, j.pid, j.etat, j.ligne);
	s[size] = '\0';
	write(sortie, s, size);
	free(s);
}

void check_jobs(cmd *c, int sortie) {
	job *jobs = c -> jobs;
	int status;
	int i = 0;
	while(i < c -> all_jobs) {
		if(strcmp(jobs[i].etat, "Running") == 0 || strcmp(jobs[i].etat, "Stopped") == 0) {
			status = INT_MIN;
                	waitpid(-jobs[i].pid, &status, WUNTRACED | WNOHANG | WCONTINUED);
			if(status != INT_MIN) {
                		if(WIFEXITED(status)) {
                        		jobs[i].etat = "Done";
                        		c -> nb_jobs = c -> nb_jobs - 1;
                        		print_job(jobs[i], sortie);
                		}
                		else if(WIFSIGNALED(status)) {
                        		jobs[i].etat = "Killed";
                        		c -> nb_jobs = c -> nb_jobs - 1;
                        		print_job(jobs[i], sortie);
                		}
                		else if(WIFSTOPPED(status)) {
                        		jobs[i].etat = "Stopped";
                        		print_job(jobs[i], sortie);
                		}
                		else if(WIFCONTINUED(status)) {
                        		jobs[i].etat = "Running";
                        		print_job(jobs[i], sortie);
                		}
			}
		}
		i++;
	}
}
