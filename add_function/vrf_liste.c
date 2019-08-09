#include "header.h"

int		vrf_liste(t_att *str1, t_object *str2 ,int i)
{
	while (str1)
	{
		if (!(strcmp(str1->att,str2[i].attribut)))
			return (0);
        str1 = str1->next;
	}
	return (1);
}
