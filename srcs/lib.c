#include "shell.h"

char	*strjoin(char const *s1, char const *s2) // concatène deux chaînes et retourne le résultat
{
	char	*s3;
	int		i;
	int		a;
	int		b;

	if (!s1 || !s2)
		return (NULL);
	a = strlen((char *)s1);
	b = strlen((char *)s2);
	s3 = (char *)malloc((a + b) * sizeof(char) + 1); // on alloue assez d'espace pour la concaténation
	if (!s3)
		return (NULL);
	i = 0;
	while (i < a) // on ajoute les caractères de la première string
	{
		s3[i] = s1[i];
		i++;
	}
	i = -1;
	while (++i < b) // on ajoute les caractères de la seconde string
	{
		s3[i + a] = s2[i];
	}
	s3[a + b] = 0;
	return (s3);
}

void petit_tab(int i, cmd *c)
{
	for(int j = i ; j < tablen(c->str_opts) ; j++)
	{
		free(c->str_opts[j]);
		c->str_opts[j] = NULL;
	}
}

void error_open()
{
	switch (errno)
	{
		case EACCES:
			write(2, "The requested access to the file is not allowed, or search permission is denied for one of the directories in the path prefix of pathname, or the file did not exist yet and write access to the parent directory is not allowed.\n", 227);
			break;
		case EDQUOT:
			write(2, "Where O_CREAT is specified, the file does not exist, and the user's quota of disk blocks or inodes on the filesystem has been exhausted.\n", 138);
			break;
		case EEXIST:
			write(2, "pathname already exists and O_CREAT and O_EXCL were used.\n", 59);
			break;
		case EFAULT:
			write(2, "pathname points outside your accessible address space.\n", 56);
			break;
		case EFBIG:
			write(2, "See EOVERFLOW.\n", 16);
			break;
		case EINTR:
			write(2, "While blocked waiting to complete an open of a slow device, the call was interrupted by a signal handler.\n", 107);
			break;
		case EINVAL:
			write(2, "The filesystem does not support the O_DIRECT flag.\n Or invalid value in flags.\nOr O_TMPFILE was specified in flags, but neither O_WRONLY nor O_RDWR was specified.\nO_CREAT was specified in flags and the final component (\"basename\") of the new file's pathname is invalid (e.g., it contains characters not permitted by the underlying filesystem).\n", 345);
			break;
		case EISDIR:
			write(2, "Pathname refers to a directory and the access requested involved writing.\nPathname refers to an existing directory, O_TMPFILE and one of O_WRONLY or O_RDWR were specified in flags, but this kernel version does not provide the O_TMPFILE functionality.\n", 252);
			break;
		case ELOOP:
			write(2, "Too many symbolic links were encountered in resolving pathname.\nPathname was a symbolic link, and flags specified O_NOFOLLOW but not O_PATH.\n", 142);
			break;
		case EMFILE:
			write(2, "The per-process limit on the number of open file descriptors has been reached.\n", 80);
			break;
		case ENAMETOOLONG:
			write(2, "Pathname was too long.\n", 24);
			break;
		case ENFILE:
			write(2, "The system-wide limit on the total number of open files has been reached.\n", 75);
			break;
		default:
			write(2, "Autre erreur voir man open pour essayer de corrriger.", 54);
			break;
	}
}

void erreur_dup2()
{
	switch(errno)
	{
		case EBADF : write(2,"oldfd n'est pas un descripteur de fichier ouvert. Ou newfd est hors de portée autorisée pour les descripteurs de fichiers\n", 125); break;
		default : printf("erreur non reconnue");
	}
}

int redir_fic(cmd *c)
{
	int err1 = 0;
	int err2 = 0;
	int err3 = 0;	

	if (c->fd_out != STDOUT_FILENO)
	{
		err1 = dup2(c->fd_out, STDOUT_FILENO); // effectue la redirection de la sortie standard
	}
	if (c->fd_in != STDIN_FILENO)
	{
		err2 = dup2(c->fd_in, STDIN_FILENO); // effectue la redirection de l'entrée standard
	}
	if (c->fd_err != STDERR_FILENO)
	{
		err3 = dup2(c->fd_err, STDERR_FILENO); // effectue la redirection de la sortie erreur
	}
	if (err1 == -1 ||err2 == -1 || err3 == -1)
	{
		erreur_dup2();
		return -1;
	} 
	return 0;
}

void free_jobs(cmd *c) {
	if(c -> jobs != NULL) 
	{
		for(int i = 0; i < c -> all_jobs; i++) {
				free((c -> jobs)[i].ligne);
		}
		free(c -> jobs);
	}
}

// free la structure complete si free_all, seulement str_opts sinon
void free_cmd(cmd *c, int free_all) {
	int i = 0;
	while (c->str_opts[i] != NULL) 
	{
		free(c->str_opts[i]);
		i += 1;
	}
	free(c->str_opts);
	if (free_all) {
		free_jobs(c);
		free(c->chem_jsh);
		free(c);
	}    
}

int	tablen(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
		i++;
	return (i - 1);
}

char    *last_cmd(char **cmd)
{
    return (cmd[tablen(cmd)]);
}

int nb_digits(int n) {
	int digits = 1;
        int tmp = n;
        while((tmp / 10) != 0) {
                digits++;
                tmp = tmp / 10;
        }
	return digits;
}
