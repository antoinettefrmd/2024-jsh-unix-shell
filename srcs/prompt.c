#include "shell.h"	

//Takes the current path and returns a new one to respect the prompt format
char * fmt_cwd(char * cwd) {
	int len = strlen(cwd);
	if(len <= 25) {
		char * res = malloc(len + 1);
		int i;
		for(i = 0; i < len; i++) {
			res[i] = cwd[i];
		}
		res[i] = '\0';
		return res;
	}
	char * res = malloc(26);
	res[25] = '\0';
	int i = len - 1;
	int j;
	for(j = 24; j >= 3; j--) {
		res[j] = cwd[i];
		i--;
	}
	res[0] = '.';
	res[1] = '.';
	res[2] = '.';
	return res;
}

//Takes all the elements needed to make the prompt and concat them all
char * cat_maison(char * pr1, char * f_cwd, char * pr2) {
	char * res = malloc(27 + strlen(f_cwd));
	int i = 0;
	for(int j = 0; j < 17; j++) {
		res[i] = pr1[j];
		i++;
	}
	while(*f_cwd != '\0') {
                res[i] = *f_cwd;
		f_cwd++;
                i++;
        }
	for(int j = 0; j < 9; j++) {
                res[i] = pr2[j];
                i++;
        }
	res[i] = '\0';
	return res;
}

//Creates a prompt using readline and returns the line read by it
char * prompt() {
	char cwd[PATH_MAX];
	if(getcwd(cwd, PATH_MAX) == NULL) {
		exit(1);
	}
	char * f_cwd = fmt_cwd(cwd);
	char * pr1 = "\001\033[32m\002[0]\001\033[34m\002";
	char * pr2 = "\001\033[00m\002$ ";
	char * pr = cat_maison(pr1, f_cwd, pr2);

	rl_outstream = stderr;
	char * ligne = readline(pr);
	free(f_cwd);
	free(pr);
	return ligne;
}
