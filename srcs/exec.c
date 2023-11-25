#include "shell.h"

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = strjoin(paths[i], "/");
		path = strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (0);
}

int	is_slash(char *s)
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

void	execute(char *cmd, char **envp)
{
	char	**tab;
	int		i;
	char	*path;

	tab = split(cmd, ' ');
	if (is_slash(tab[0]))
		path = tab[0];
	else
		path = find_path(tab[0], envp);
	if (!path || execve(path, tab, envp) == -1)
	{
		i = -1;
		while (tab[++i])
			free(tab[i]);
		free(tab);
		if (!path)
			error();
		error();
	}
}
