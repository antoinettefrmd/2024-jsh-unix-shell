#include "shell.h"

int	wordlen(char const *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && *(str + i) != c)
		i++;
	return (i);
}

int	wordcount(char const *str, char c)
{
	int	i;
	int	w;

	w = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		i = wordlen(str, c);
		str += i;
		if (i)
			w++;
	}
	return (w);
}

char	*wordcpy(char const *src, int n)
{
	char	*dest;

	dest = malloc((n + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	dest[n] = '\0';
	while (n--)
		dest[n] = src[n];
	return (dest);
}

char	**split(char const *s, char c)
{
	char	**t;
	int		size;
	int		i;
	int		n;

	size = wordcount(s, c);
	t = malloc((size + 1) * sizeof(char *));
	if (!t)
		return (NULL);
	i = -1;
	while (++i < size)
	{
		while (*s && *s == c)
			s++;
		n = wordlen(s, c);
		t[i] = wordcpy(s, n);
		if (!(t[i]))
			return (NULL);
		s += n;
	}
	t[size] = 0;
	return (t);
}
