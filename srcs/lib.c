#include "shell.h"

char	*strjoin(char const *s1, char const *s2)
{
	char	*s3;
	int		i;
	int		a;
	int		b;

	if (!s1 || !s2)
		return (NULL);
	a = strlen((char *)s1);
	b = strlen((char *)s2);
	s3 = (char *)malloc((a + b) * sizeof(char) + 1);
	if (!s3)
		return (NULL);
	i = 0;
	while (i < a)
	{
		s3[i] = s1[i];
		i++;
	}
	i = -1;
	while (++i < b)
	{
		s3[i + a] = s2[i];
	}
	s3[a + b] = 0;
	return (s3);
}

void free_cmd(cmd *c, int b) {
	int i = 0;
    while (c->str_opts[i] != NULL) 
    {
        free(c->str_opts[i]);
        i += 1;
    }
    free(c->str_opts);
	if (b) {
		free(c);
	}
    
}
