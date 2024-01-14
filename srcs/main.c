#include "shell.h"

int e_in;
int s_out;
int s_err;

int main(int argc, char const *argv[], char **envp)
{

	ignore_signals();
	if (argc != 1) {
		write(2, "Just launch without args\n", 25);
		exit(1);
	}
	struct cmd *c;
	char buf[PATH_MAX];

	(void)argv;
	
	// initialisation de la commande
	c = malloc(sizeof (cmd));
	c -> nb_jobs = 0; 
	c -> all_jobs = 0;
	c -> jobs = NULL;
	c -> val_retour = 0;
	c -> origin = c;
	getcwd(buf, sizeof(buf));
	c->chem_jsh = strdup(buf);
	c ->str_opts = NULL;
	
	// Récupération de la première ligne de commandes
	char * ligne = prompt(c);

	while(1) {
		ignore_signals(); // Rappel qu'on doit ignorer SIGTERM

		if (ligne == NULL) //vérification que la première ligne de commande n'est pas null
		{
			exit_maison(c);
			return 0;
		}
		
		e_in = dup(0); // Sauvergarde des directions
		s_out = dup(1);
		s_err = dup(2);
		c -> fd_in = 0; // Remise à la normal pour une commande
		c -> fd_out = 1;
		c -> fd_err = 2;
		c ->str_opts = NULL;
		
		execloop(c, ligne, envp);
		dup2(e_in, STDIN_FILENO); // Remets l'entrée standard potentiellement redirigée sur la vraie entrée standard
		dup2(s_out, STDOUT_FILENO); // Remets la sortie standard potentiellement redirigée sur la vraie sortie standard
		dup2(s_err, STDERR_FILENO); // Remets la sortie erreur potentiellement redirigée sur la vraie erreur standard
		close(e_in); // fermeture des fd de sauvegarde
		close(s_out);
		close(s_err);
		check_jobs(c, 2);
		ligne = prompt(c); // Récupération de la nouvelle ligne de commande
	}
	close(e_in); //fermeture des sauvegardes
	close(s_out);
	close(s_err);
	return 0;
}
