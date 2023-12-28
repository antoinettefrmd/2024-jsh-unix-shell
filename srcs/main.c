#include "shell.h"

int main(int argc, char const *argv[], char **envp)
{
	
	if (argc != 1) {
		write(2, "Just launch without args\n", 25);
		exit(1);
	}
	// Gestion des signaux -----------------
	// volatile sig_atomic_t signal_recu = 0;

	// struct sigaction action = {0};
    // action.sa_handler = SIG_IGN;
	// sigaction(3, &action, NULL);
	// sigaction(22, &action, NULL);
	// sigaction(20, &action, NULL);
	// sigaction(15, &action, NULL);
	// sigaction(2, &action, NULL);
	// void handler () { 
	// 	signal_recu = 1; 
	// }
	// -------------------------------------

	(void)argv;
	struct cmd *c = malloc(sizeof (cmd));
	c -> nb_jobs = 0;
	c -> all_jobs = 0;
	c -> jobs = NULL;
	c -> val_retour = 0;
	c -> fd_in = 0;
	c -> fd_out = 1;
	c -> fd_err = 2;
	int in = dup(0); //sauvergarde des redirections
	int out = dup(1);
	int err = dup(2);
	
	char buf[PATH_MAX];
	getcwd(buf, sizeof(buf)); // Stocke le chemin du dépot
	c->chem_jsh = strdup(buf);
	char * ligne = prompt(c);
	c ->str_opts = NULL;
	while(1) {
		c ->str_opts = NULL;
		if (ligne == NULL) 
		{
			exit_maison(c, 1);
			return 0;
		}
		c->bg = 0;
        	c->str_opts = split(ligne, ' '); // répartit la commande dans le tableau pour separer les arguments
		if(strcmp(ligne, "") != 0) {
			if (!strcmp(last_cmd(c->str_opts), "&"))
			{
				c->bg = 1;
				free(c -> str_opts[tablen(c->str_opts)]);
				c->str_opts[tablen(c->str_opts)] = NULL;
			}
			add_history(ligne);
			if(c->str_opts[0] != NULL) {
				if(strcmp("cd", c->str_opts[0]) == 0 || strcmp("exit", c->str_opts[0]) == 0 || strcmp("kill", c->str_opts[0]) == 0 || strcmp("jobs", c->str_opts[0]) == 0)
					is_builtins(c);
				else {
					if (c -> bg) {
						process(c, envp, strndup(ligne, strlen(ligne) - 2)); // on considère alors que c'est une commande externe
					}
					else process(c, envp, strdup(ligne));
				}
			}
		}
		free_cmd(c, 0); // free seulement le tableau des commandes et options
		free(ligne);
		check_jobs(c, 2);
		dup2(in, STDIN_FILENO); // Remets l'entrée standard potentiellement redirigée sur la vraie entrée standard
		dup2(out, STDOUT_FILENO); // Remets la sortie standard potentiellement redirigée sur la vraie sortie standard
		dup2(err, STDERR_FILENO); // Remets la sortie erreur potentiellement redirigée sur la vraie erreur standard
		ligne = prompt(c);
	}
	close(in);
	close(out);
	close(err);
	return 0;
}
