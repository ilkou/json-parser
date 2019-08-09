/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   header.h                                                                 */
/*                                                                            */
/*   By: oouklich                                                             */
/*                                                                            */
/*   Created: 2018/12/13 13:35:55 by oouklich                                 */
/*   Updated: 2018/12/26 22:46:37 by oouklich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# define BUFF_SIZE 10000

# define ATTRIBUT_LONG 20

# define VALEUR_LONG 80

# define SPACE_LONG 20

# define FREE(x) { if (x) free(x); x = NULL; }

typedef struct att_val att_val;
typedef struct nb_occ nb_occ;
typedef struct s_arbre s_arbre;
typedef struct nb_occ1 nb_occ1;
typedef struct objets objets;

struct		objets
{
	char	obj[1000];
	objets	*suivant;
};

struct		nb_occ1
{
	char	attribut[ATTRIBUT_LONG];
	int		n;
}			;

struct		s_arbre
{
	char	key[ATTRIBUT_LONG];
	s_arbre	*f_drt;
	s_arbre *f_gch;
}			;

struct		nb_occ
{
	char	attrib[ATTRIBUT_LONG];
	int		n;
	nb_occ	*suivant;
}			;

struct		att_val
{
	char	attribut[ATTRIBUT_LONG];
	char	valeur[VALEUR_LONG];
	att_val	*suivant;
}			;

typedef struct			s_object
{
	char				*attribut;
	char				*valeur;
}						t_object;

typedef struct			s_attribut
{
	char				*att;
	int					occur;
}						t_attribut;

typedef struct s_att	t_att;
struct s_att
{
	char				*att;
	int					occur;
	t_att				*next;
};

char					*get_full_text(int fd);
int						no_attributs(char **src);
char					*no_espaces_blancs(char *src);
int						no_objects(char *str);
int						object_len(char *str, int i);
char					*space(int i);
int						vrf(t_attribut *str1,int k, t_object *str2 ,int j);
int						vrf_liste(t_att *str1, t_object *str2 ,int i);
int						hachage(char *chaine, int HACH_SIZE);
int						levenshtein(char *a,char *b);
int 					min(int a,int b,int c);
int 					str_c(char a,char b);

#endif
