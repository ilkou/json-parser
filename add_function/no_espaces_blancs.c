#include "header.h"

char	*no_espaces_blancs(char *src)
{
	char	*dest;
	int		i;
	int		j;
	int		size;

	size = 0;
	j = 0;
	i = 0;
	while (src[i])
	{
		if (src[i] != '\n' && src[i] != ' ' && src[i] != '\t')
			size++;
		i++;
	}
	dest = (char*)malloc(sizeof(char*) * (size + 1));
	i = 0;
	while (src[i])
	{
		if (src[i] != '\n' && src[i] != ' ' && src[i] != '\t')
			dest[j++] = src[i];
		i++;
	}
	dest[j] = '\0';
	return (dest);
}
