#include "shell.h"

void print_job(job j) {
	printf("[%d] %d %s %s\n", j.nb, j.pid, j.etat, j.ligne);
}

void check_jobs(cmd *c) {
	job *jobs = c -> jobs;
	int i = 0;
	while(i < c -> all_jobs) {
		if(strcmp(jobs[i].etat, "Done") != 0 && strcmp(jobs[i].etat, "Killed") != 0) {
			int status;
			waitpid(jobs[i].pid, &status, WNOHANG);
			if(WIFEXITED(status)) {
				jobs[i].etat = "Done";
				c -> nb_jobs = c -> nb_jobs - 1;
				print_job(jobs[i]);
			}
			else if(WIFSIGNALED(status) && WTERMSIG(status) != 32) {
				jobs[i].etat = "Killed";
				c -> nb_jobs = c -> nb_jobs - 1;
				print_job(jobs[i]);
			}
			else if(WIFSTOPPED(status)) {
				jobs[i].etat = "Stopped";
				print_job(jobs[i]);
			}
			else if(WIFCONTINUED(status)) {
				jobs[i].etat = "Running";
				print_job(jobs[i]);
			}
		}
		i++;
	}
}
