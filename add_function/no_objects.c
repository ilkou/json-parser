#include "header.h"

int		no_objects(char *str)
{
	int		i;
	int		no_ob;

	no_ob = 1;
	i = 0;
	while (str[i])
	{
		if (str[i] == '{')
		{
			no_ob++;i++;
			while (str[i] && str[i] != '}')
			{
				if (str[i] == '{')
				{
					while (str[i] && str[i++] != '}');
				}
				i++;
			}
		}
		i++;
	}
	return (no_ob);
}
