#include "shell.h"

char	*find_path(char *cmd, char **envp) // cherche la commande dans l'environnement
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (strnstr(envp[i], "PATH", 4) == 0) // on cherche la variable PATH
		i++;
	paths = split(envp[i] + 5, ':'); // chaque chemin est stocké dans le tableau de string
	i = 0;
	while (paths[i])
	{
		part_path = strjoin(paths[i], "/"); // la commande est rajouté à la fin du chemin
		path = strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0) // on test si la commande se trouve dans le chemin 
			return (path);
		free(path);
		i++;
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths); 
	return (0); // si la commande n'est pas trouvée, on libère la mémoire puis on retourne 0
}

int	is_slash(char *s) // test si un slash se trouve dans la string s
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

void	error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

// execute la commande externe 
void	execute(cmd *c, char **envp)
{
	int		i;
	char	*path;
	char 	**tab = c->str_opts;

	if (is_slash(tab[0]))
		path = tab[0];
	else
		path = find_path(tab[0], envp); // si aucun chemin n'est spécifié, on cherche la commande dans l'environnement
	if (!path || execve(path, tab, envp) == -1) // exécution de la commande
	{
		i = -1;
		while (tab[++i]) // si l'exécution échoue ou que la commande n'est pas trouvée, on libère la mémoire
			free(tab[i]);
		free(tab);
		if (!path)
			error();
		error();
		return;
	}
}

void	execloop(cmd *commande, char *ligne, char **envp)
{
	cmd *c;
	int	i = 0;

	c = parsing_pipe(ligne, commande); // répartit la commande dans le tableau pour separer les arguments
	c -> nb_c = nb_cmd(c);
	int fd[c->nb_c > 1 ? 1 : c->nb_c - 1][2];
	while (c)
	{
		//print_cmd(c);
		if(strcmp(ligne, "") != 0) {
			if (!strcmp(last_cmd(c->str_opts), "&"))
			{
				free(c -> str_opts[tablen(c->str_opts)]);
				c->str_opts[tablen(c->str_opts)] = NULL;
			}
			add_history(ligne);
			if(c->str_opts[0] != NULL) {
				if(!is_builtins(c)) {
					if (c -> bg) {
						process(c, envp, strndup(ligne, strlen(ligne) - 2), fd[i]); // on considère alors que c'est une commande externe
					}
					else process(c, envp, strdup(ligne),fd[i]);
				}
				else builtins(c);
			}
		}
		free_cmd(c, 0); // free seulement le tableau des commandes et options
		check_jobs(c, 2);
		c = c->next;
		if (c && c->next == NULL)
			close(fd[i][1]);
		else if (is_pipe(ligne))
			close(fd[i][0]);
	}
	free(ligne);
}
