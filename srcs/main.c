#include "shell.h"

int main(int argc, char const *argv[], char **envp)
{
	ignore_signals();
	if (argc != 1) {
		write(2, "Just launch without args\n", 25);
		exit(1);
	}

	(void)argv;
	struct cmd *c = malloc(sizeof (cmd));
	c -> nb_jobs = 0;
	c -> all_jobs = 0;
	c -> jobs = NULL;
	c -> val_retour = 0;

	int in = dup(0); //sauvergarde des redirections
	int out = dup(1);
	int err = dup(2);
	
	char buf[PATH_MAX];
	getcwd(buf, sizeof(buf)); // Stocke le chemin du dépot
	c->chem_jsh = strdup(buf);
	char * ligne = prompt(c);
	c ->str_opts = NULL;
	while(1) {
		ignore_signals();
		c -> fd_in = 0;
		c -> fd_out = 1;
		c -> fd_err = 2;
		c ->str_opts = NULL;
		if (ligne == NULL) 
		{
			exit_maison(c);
			return 0;
		}
		execloop(c, ligne, envp);
		dup2(in, STDIN_FILENO); // Remets l'entrée standard potentiellement redirigée sur la vraie entrée standard
		dup2(out, STDOUT_FILENO); // Remets la sortie standard potentiellement redirigée sur la vraie sortie standard
		dup2(err, STDERR_FILENO); // Remets la sortie erreur potentiellement redirigée sur la vraie erreur standard
		close(in);
		close(out);
		close(err);
		ligne = prompt(c);
	}
	close(in);
	close(out);
	close(err);
	return 0;
}
