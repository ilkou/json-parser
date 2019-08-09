#include "header.h"

int		vrf(t_attribut *str1,int k, t_object *str2 ,int j)
{
	int		i;

	i = 0;
	while (i < k)
	{
		if (!(strcmp(str1[i].att,str2[j].attribut)))
			return (0);
		i++;
	}
	return (1);
}
