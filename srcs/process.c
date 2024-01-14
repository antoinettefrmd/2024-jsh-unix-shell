#include "shell.h"

void add_process(cmd *c, pid_t pid, char *ligne, int print) {
	job j = (c -> jobs)[0];
	int groupe = c -> all_jobs;
	for(int i = 0; i < c -> all_jobs; i++) {
		if((c -> jobs)[i].groupe == groupe) {
			j = (c -> jobs)[i];
			break;
		}
	}

	if(j.pid[0] == 0) {
		j.pid[0] = pid;
		if(print) print_job(j, 2, 0);
	}

	for(int i = 1; i < j.nb_process + 1; i++) {
		if(j.pid[i] == 0) {
			j.pid[i] = pid;
			j.etat[i] = "Running";
			j.ligne[i] = pipe_get(ligne, i - 1);
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
	for(int i = 0; i < new.nb_process + 1; i++) {
		new.pid[i] = 0;
	}
    newJobs[nb - 1] = new;
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
	if (!is_builtins(c))
		execute(c, envp);
	else {
		builtins(c);
		exit(errno);
	}
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
	int indice_redir;

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
	
    pid = fork();
    if (pid == -1)
        error();
    else if (pid == 0) {
		cmprtment_par_defaut();
		if (!is_builtins(c)) {
			indice_redir = parse_redir(c);
			if (indice_redir == -1) exit(1);
			petit_tab(indice_redir, c);
		}
		if (is_pipe(ligne)) {
			child_process(c, fd,i,  envp);
		}
		execute(c, envp); // execute la commande dans le processus fils
		exit(errno);

    }
	else {
		if (!c->bg) {
			add_process(c -> origin, pid, ligne, 0);
			give_fg(getpgid(pid));
		}
		else add_process(c -> origin, pid, ligne, 1);
	}
}
