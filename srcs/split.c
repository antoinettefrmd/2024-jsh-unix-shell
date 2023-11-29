#include "shell.h"

int	wordlen(char const *str, char c) // compte la taille de la prochaine sous-chaîne jusqu'au délimiteur c
{
	int	i;

	i = 0;
	while (str[i] && *(str + i) != c) // tant qu'on est pas arrivé au délimiteur, on incrémente i
		i++;
	return (i);
}

int	wordcount(char const *str, char c) // retourne le nombre de sous-chaînes de la string str, séparées par le délimiteur c
{
	int	i;
	int	w;

	w = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		i = wordlen(str, c); // on fait avancer le pointeur de la taille de la prochaine sous-chaîne
		str += i;
		if (i)
			w++;
	}
	return (w);
}

char	*wordcpy(char const *src, int n) // copie les n premiers caractères de la string src
{
	char	*dest;

	dest = malloc((n + 1) * sizeof(char)); // alloue uniquement n caractères
	if (!dest)
		return (NULL);
	dest[n] = '\0'; // le caractère NULL est placé à la fin de la string
	while (n--) // on copie chaque caractère tant qu'on a pas atteint n
		dest[n] = src[n];
	return (dest);
}

char	**split(char const *s, char c) // crée un tableau de string contenat les sous-chaînes délimités par le caractère c
{
	char	**t;
	int		size;
	int		i;
	int		n;

	size = wordcount(s, c); // on stocke à l'avance le nombre de mots puis on alloue assez d'espace
	t = malloc((size + 1) * sizeof(char *));
	if (!t)
		return (NULL);
	i = -1;
	while (++i < size) // on ajoute chaque sous-chaîne au tableau
	{
		while (*s && *s == c) // tant que la carctère courant est un délimiteur, on ne l'ajoute pas au tableau
			s++;
		n = wordlen(s, c);
		t[i] = wordcpy(s, n); // on copie la sous chaîne au tableau
		if (!(t[i]))
			return (NULL);
		s += n;
	}
	t[size] = 0; // le dernier élément du tableau doit être un caractère NULL afin de pouvoir le parcourir
	return (t);
}
