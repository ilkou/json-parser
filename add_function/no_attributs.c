#include "header.h"

int		no_attributs(char **src)
{
	int		i;
	int		j;
	int		no_att;

	no_att = 0;
	i = 0;
	while (src[i])
	{
		j = 0;
		while (src[i][j] != '\0')
		{
            if (src[i][j] == '[')
                while (src[i][j] != ']' && src[i][j] != '\0')
                    j++;
            if (src[i][j] == '{')
                while (src[i][j] != '}' && src[i][j] != '\0')
                    j++;
            if (src[i][j] == '"')
                while (src[i][j] != '"' && src[i][j] != '\0')
                    j++;
			if (src[i][j] == ':')
				no_att++;
			j++;
		}
		i++;
	}
	return (no_att);
}
