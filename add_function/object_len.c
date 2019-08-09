#include "header.h"

int		object_len(char *str, int i)
{
	int		size;

	size = 2;
	while (str[i] && str[i] != '}')
	{
		size++;
		i++;
		if (str[i] == '{')
		{
			while (str[i] && str[i] != '}')
			{
				size++;i++;
			}
			size++;
			i++;
		}
	}
	return (size);
}
