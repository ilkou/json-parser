/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   headers.h                                                                */
/*                                                                            */
/*   By: oouklich                                                             */
/*                                                                            */
/*   Created: 2018/12/13 13:35:55 by oouklich                                 */
/*   Updated: 2018/12/24 11:31:24 by oouklich                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERS_H
# define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "./add_function/header.h"
#include "./add_function/get_full_text.c"
#include "./add_function/no_attributs.c"
#include "./add_function/no_espaces_blancs.c"
#include "./add_function/no_objects.c"
#include "./add_function/object_len.c"
#include "./add_function/space.c"
#include "./add_function/hachage.c"
#include "./add_function/vrf_liste.c"
#include "./add_function/levenshtein.c"
#include "./add_function/min.c"
#include "./add_function/str_c.c"
#include "./add_function/vrf.c"

char			**lire(char    *chemin);
t_object		*tab_dynamique(char **src);
t_attribut		*occurence_hachage(t_object *src,char *chemin);
t_attribut		*occurence_tab(t_object *src,char *chemin);
t_att           *occurence_liste(t_object *src,char *chemin);
int             nb_obj(char *chemin);
char            **lire_f(int j,char *chemin);
att_val         *attr_vale(char **p1,int j);
nb_occ          *nbr_occu(att_val *t1);
nb_occ          *nbr_occu1(att_val *t1);
s_arbre         *arbre(att_val *t1);
void            aide(s_arbre *a,att_val *b);
nb_occ          *nbr_occu_lev(att_val *t1);
int             compteur_fd(s_arbre *a);
int             compteur_fg(s_arbre *a);
nb_occ1         *nbr_occu_arbre(s_arbre *a);
objets          *lire_liste(char *chemin);
att_val         *attr_vale_liste(objets *a);

#endif
