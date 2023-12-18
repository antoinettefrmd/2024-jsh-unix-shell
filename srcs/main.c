#include "shell.h"

int main(int argc, char const *argv[], char **envp)
{
	
	if (argc != 1) {
		write(2, "Just launch without args\n", 25);
		exit(1);
	}
	struct cmd *c = malloc(sizeof (cmd));
	c -> nb_jobs = 0;
	c -> all_jobs = 0;
	c -> jobs = NULL;
	c -> val_retour = 0;
	(void)argv;
	char buf[PATH_MAX];
	getcwd(buf, sizeof(buf)); // Stocke le chemin du dépot
	c->chem_jsh = buf;
	char * ligne = prompt(c);
	while(1) {
		if (ligne == NULL) 
		{
			exit(c->val_retour);
			free(c);
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
				if (!(is_builtins(c))) { // regarde si l'arg est une commande interne
					process(c, envp, strndup(ligne, strlen(ligne) - 2)); // on considère alors que c'est une commande externe
				}
			}
		}
		free_cmd(c, 0); // free seulement le tableau des commandes et options
		free(ligne);
		check_jobs(c);
		ligne = prompt(c);
	}
	return 0;
}
