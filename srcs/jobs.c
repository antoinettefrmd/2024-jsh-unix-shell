#include "shell.h"

void print_job(job j, int sortie) {
	int size = strlen(j.ligne[0]) + strlen(j.etat[0]) + nb_digits(j.groupe) + nb_digits(j.pid[0]) + 6;
	char *s = malloc(size + 1);
	sprintf(s, "[%d] %d %s %s\n", j.groupe, j.pid[0], j.etat[0], j.ligne[0]);
	s[size] = '\0';
	write(sortie, s, size);
	free(s);
}

void check_jobs(cmd *c, int sortie) {
	job *jobs = c -> jobs;
	int status = 0;
    int i = 0;
    while(i < c -> all_jobs) {
        status = INT_MIN;
        if(strcmp(jobs[i].etat[0], "Running") == 0 || strcmp(jobs[i].etat[0], "Stopped") == 0) {
            waitpid(-jobs[i].pid[0], &status, WUNTRACED | WNOHANG | WCONTINUED);
            if(status != INT_MIN) {
                if(WIFEXITED(status)) {
                    jobs[i].etat[0] = "Done";
                    c -> nb_jobs = c -> nb_jobs - 1;
                    print_job(jobs[i], sortie);
                }
                else if(WIFSIGNALED(status)) {
                    jobs[i].etat[0] = "Killed";
                    c -> nb_jobs = c -> nb_jobs - 1;
                    print_job(jobs[i], sortie);
                }
                else if(WIFSTOPPED(status)) {
                    jobs[i].etat[0] = "Stopped";
                    print_job(jobs[i], sortie);
                }
                else if(WIFCONTINUED(status)) {
                    jobs[i].etat[0] = "Running";
                    print_job(jobs[i], sortie);
                }
            }
        }
        i++;
    }
}
