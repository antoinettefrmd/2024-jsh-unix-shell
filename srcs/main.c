#include "shell.h"

int main(int argc, char const *argv[], char **envp)
{
	struct cmd *c = malloc(sizeof (cmd));
	if (argc != 1) {
		write(2, "Just launch without args", 25);
		exit(1);
	}
	(void)argv;
	char buf[PATH_MAX];
	getcwd(buf, sizeof(buf));
	c->chem_jsh = buf;
	char * ligne = prompt();
	while(1) {
		if (ligne == NULL) 
		{
			exit(c->val_retour);
			free(c);
			return 0;
		}
		c->str_opts = split(ligne, ' ');
		add_history(ligne);
		free(ligne);
		if(c->str_opts[0] != NULL) {
			if (!(is_builtins(c))) {
				process(c, envp);
			}
		}	
		free_cmd(c, 0);	
		ligne = prompt();
	}
	return 0;
}
