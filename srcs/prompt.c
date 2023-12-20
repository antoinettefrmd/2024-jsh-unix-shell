#include "shell.h"	

//Takes the current path and returns a new one to respect the prompt format
char * fmt_cwd(char * cwd, int jobs_len) {
	int len = strlen(cwd);
	if(len <= (26 - jobs_len)) {
		char * res = malloc(len + 1);
		int i;
		for(i = 0; i < len; i++) {
			res[i] = cwd[i];
		}
		res[i] = '\0';
		return res;
	}
	char * res = malloc(26 - jobs_len);
	res[25 - jobs_len] = '\0';
	int i = len - 1;
	int j;
	for(j = 24 - jobs_len; j >= 3; j--) {
		res[j] = cwd[i];
		i--;
	}
	res[0] = '.';
	res[1] = '.';
	res[2] = '.';
	return res;
}

//Takes all the elements needed to make the prompt and concat them all
char * cat_maison(char * pr1, char * jobs, char * pr2, char * f_cwd, char * pr3) {
	char * res = malloc(27 + strlen(f_cwd) + strlen(jobs));
	int i = 0;
	for(int j = 0; j < 8; j++) {
		res[i] = pr1[j];
		i++;
	}
	for(size_t j = 0; j < strlen(jobs); j++) {
		res[i] = jobs[j];
		i++;
	}
	for(int j = 0; j < 8; j++) {
		res[i] = pr2[j];
		i++;
	}
	while(*f_cwd != '\0') {
                res[i] = *f_cwd;
		f_cwd++;
                i++;
        }
	for(int j = 0; j < 9; j++) {
                res[i] = pr3[j];
                i++;
        }
	res[i] = '\0';
	return res;
}

//Creates a prompt using readline and returns the line read by it
char * prompt(struct cmd *c) {
	char cwd[PATH_MAX];
	if(getcwd(cwd, PATH_MAX) == NULL) {
		exit(1);
	}

	int digits = nb_digits(c -> nb_jobs);
	char * jobs = malloc(digits + 1);
	jobs[0] = '\0';
	sprintf(jobs, "%d", c -> nb_jobs);

	char * f_cwd = fmt_cwd(cwd, strlen(jobs));
	char * pr1 = "\001\033[32m\002[";
	char * pr2 = "]\001\033[34m\002";
	char * pr3 = "\001\033[00m\002$ ";
	char * pr = cat_maison(pr1, jobs, pr2, f_cwd, pr3);

	rl_outstream = stderr;
	char * ligne = readline(pr);
	free(jobs);
	free(f_cwd);
	free(pr);
	return ligne;
}
