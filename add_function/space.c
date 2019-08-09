#include "header.h"

char	*space(int i)
{
	char	*space;

	space = malloc(i + 1);
	space[i--] = '\0';
	while (i >= 0)
		space[i--] = ' ';
	return (space);
}
