#include "header.h"
#include <ctype.h>

char	*get_full_text(int fd)
{
	int		sum;
	int		ret;
	char	*text;

	sum = 0;
	text = (char*)malloc(sizeof(char*) * BUFF_SIZE + 1);
	while ((ret = read(fd, &text[sum], BUFF_SIZE)))
	{
		sum = sum + ret;
		text[sum] = '\0';
		text = realloc(text, BUFF_SIZE + sum + 1);
	}
    text[sum] = '\0';
	return (text);
}
