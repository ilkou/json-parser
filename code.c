/* ************************************************************************* */
/*                                                                           */
/*                                                                           */
/*                       "Projet algorithmique"                              */
/*                     Gestion D'un Fichier JSON                             */
/*                                                                           */
/*    Realise par : oouklich                                                 */
/*                  arachidi                                                 */
/*                                                                           */
/*                                                                           */
/* ************************************************************************* */

#include "headers.h"

/* ************************ mode Tableau Dynamique ************************** */

char		**lire(char *chemin)
{
	int			i;
	int			j;
	int			k;
	int			fd;
	int			no_obj;
	char		**dest;
	char		*text;

	fd = open(chemin, O_RDONLY); //ouvrir le fihier
	text = get_full_text(fd); // lire tout le fichier
	close(fd); //fermer le fichier
	i = 0;
	dest = (char**)malloc(sizeof(char*) * no_objects(text)); // alloue la taille nécessaire
	no_obj = no_objects(text) - 1;
	text = strstr(text, "{"); //text recoit pointeur vers la première occurrence de '{' dans le texte
	for (i = 0;i < no_obj;)
	{
		k = 0;
		j = 0;
		dest[i] = (char*)malloc(object_len(text, k) * sizeof(char*));// alloue le longueur nécessaire
		k++;
		while (text[k] != '}' && text[k])
		{
			dest[i][j++] = text[k++];
			if (text[k] == '{') // le cas d'un objet comme valeur
			{
				while (text[k] != '}' && text[k])
					dest[i][j++] = text[k++];
				dest[i][j] = '}';
				j++;
				k++;
			}
		}
		dest[i++][j] = '\0'; // indiquer la fin du chaine des caractères
		dest[i - 1] = no_espaces_blancs(dest[i - 1]); //sans espaces blancs
		text = strstr(&text[k], "{"); //text recoit pointeur vers la première occurrence de '{' dans le texte dont l'indice est k
	}
	dest[i] = 0; // indiquer la fin du tableau
	return (dest);
}

t_object	*tab_dynamique(char **src)
{
	int			i;
	int			j;
	int			a;
	int			v;
	int			d;
	int			check;
	int			object_len;
    char        *att;
    char        *val;
	t_object	*dest;

	i = 0;
	d = 0;
	dest = malloc(sizeof(struct s_object) * (no_attributs(src) + 3)); // alloue la taille nécessaire
	while (src[i])
	{
		j = 1;
		object_len = (int)strlen(src[i]); /* calculer la longueur de l'objet*/
		for (j = 1;j < object_len;)
        {
            att = malloc(sizeof(char*) * ATTRIBUT_LONG); /*alloue un espace temporaire qu'on va liberer */
            val = malloc(sizeof(char*) * VALEUR_LONG);
			a = 0;
			v = 0;
			check = 1;
			while (src[i][j] != '"' && j < object_len)
				att[a++] = src[i][j++];
			att[a] = '\0';
            dest[d].attribut = strdup(att);
            FREE(att);
			j = j + 2; /*depasser le " et : */
			if (src[i][j] == '"')
				j++;
			else if (src[i][j] == '{') /* le cas d'une valeur comme objet*/
			{
				while ((src[i][j] != '}') && (j < object_len))
					val[v++] = src[i][j++];
				val[v++] = '}';
				val[v] = '\0';
                dest[d].valeur = strdup(val);
                FREE(val);
				check = 0;
			}
			else if (src[i][j] == '[') /* le cas d'une valeur comme tableau*/
			{
				while ((src[i][j] != ']') && (j < object_len))
					val[v++] = src[i][j++];
				val[v++] = ']';
				val[v] = '\0';
                dest[d].valeur = strdup(val);
                FREE(val);
				check = 0;
			}
			while ((src[i][j] != '"') && (src[i][j - 1] != '"' || src[i][j] != ',') && (j < object_len) && check)
				val[v++] = src[i][j++];
			if (check) /*faites ces insructions s'il est ni objet ni tableau*/
			{
				val[v] = '\0';
                dest[d].valeur = strdup(val);
                FREE(val);
				if (src[i][j] == '"' && src[i][j + 1] == ',')
					j = j + 3;
				else if (src[i][j - 1] == '"' && src[i][j] == ',')
					j = j + 2;
				else if (src[i][j] == '"')
					j = j + 1;
			}
			else
				j = j + 3;
			d++;
		}
		i++;
	}
	dest[d].attribut = strdup("\0");
	dest[d].valeur = strdup("\0");
	return (dest);
}

t_attribut	*occurence_hachage(t_object *src,char *chemin)
{
	int			i;
	int			no_att;
	t_attribut	*dest;
	int         HACH_SIZE;
	int         index;

	no_att = no_attributs(lire(chemin));
	HACH_SIZE = no_att;
	dest = malloc(sizeof(struct s_attribut) * HACH_SIZE);
	for (i = 0;i < HACH_SIZE;i++)
		dest[i].occur = 0;
	i = 0;
	for (i = 0;i < no_att;i++)
	{
	    index = hachage(src[i].attribut,HACH_SIZE);
		if (dest[index].occur == 0)/*nouvel attribut*/
		{
			dest[index].att = strdup(src[i].attribut);
			dest[index].occur = 1;
		}
		else if (!(strcmp(dest[index].att,src[i].attribut))) /*deux attributs semblable*/
			dest[index].occur++;
		else if (strcmp(dest[index].att,src[i].attribut) && dest[index + 1].occur == 0) /*deux chaines de meme indice mais pas semblable **la premiere fois**/
		{
			dest[index + 1].att = strdup(src[i].attribut);
			dest[index + 1].occur = 1;
		}
		else /*la deuxieme fois*/
			dest[index + 1].occur++;
	}
	return (dest);
}

t_attribut	*occurence_tab(t_object *src, char *chemin)
{
	int			i;
	int			k;
	int			verif;
	int			no_att;
	t_attribut	*dest;

	no_att = no_attributs(lire(chemin));
	dest = malloc(sizeof(struct s_attribut) * 2);//alloué une case pour un attribut et une autre pour null *la fin* */
	dest[0].att = strdup(src[0].attribut);
	dest[0].occur = 0;
	i = 0;
	k = 0;
	for (i = 0;i < no_att + 1;i++) /*remplir un tableau avec des attributs dissemblable*/
	{
		verif = vrf(dest, k + 1, src, i); /*comparer l'attribut actuel avec les attributs deja stocker*/
		if (verif)
		{
			k++;
			dest = realloc(dest, sizeof(struct s_attribut) * (k + 1)); /*realloué avec la taille necessaire*/
			dest[k].att = strdup(src[i].attribut);
			dest[k].occur = 0; /*initialiser avec 0*/
		}
	}
	dest[k].att[0] = '\0';
	dest[k].occur = 0;
	k = 0;
	while (dest[k].att[0]) /*tant que le tableau précédente n'est pas terminé*/
	{
		i = 0;
		for (i = 0;i < no_att + 1;i++) /*calculer l'occurence de chaque attribut*/
		{
			if (!(strcmp(dest[k].att, src[i].attribut)))
				dest[k].occur++;
		}
		k++;
	}
	return (dest);
}

nb_occ1         *nbr_occu_arbre(s_arbre *a)
{   int j;
    int i=compteur_fg(a);
    struct nb_occ1 *t;
    t=malloc(sizeof(struct nb_occ1) * (i+1));
    for(j=0;j<i;j++)
    {
        strcpy(t[j].attribut,a->key);
        t[j].n=compteur_fd(a);//nombre d'occurence de a est la chaine des filds droites
        a=a->f_gch;//pour passer au 2ème attribut
    }
    return (t);
}
s_arbre         *arbre(att_val *t1)
{
    struct s_arbre *d1=(struct s_arbre*) malloc(sizeof(struct s_arbre));
    strcpy(d1->key,t1->attribut);//initialisation de l'arbre
    t1=t1->suivant;
    d1->f_drt=NULL;
    d1->f_gch=NULL;
    while(t1!=NULL)
    {
        aide(d1,t1);//pour inserer tout les attributs dans l'arbre
        t1=t1->suivant;
    }
    return (d1);

}
void        aide(s_arbre *a,att_val *b)
{
    if(!strcmp(a->key,b->attribut) && a->f_drt==NULL)//si la case droite est vide et l'attribut égale au clé du noeud
    {
        a->f_drt= malloc(sizeof(struct s_arbre));
        strcpy(a->f_drt->key,b->attribut);
        a->f_drt->f_drt=NULL;
        a->f_drt->f_gch=NULL;
    }
    else { if(!strcmp(a->key,b->attribut) && a->f_drt!=NULL)
    {
        aide(a->f_drt,b);
    }
    else
    {  if(strcmp(a->key,b->attribut) && a->f_gch==NULL)//si la case gauche est vide et l'attribut est différent du clé du noeud
    {
        a->f_gch= malloc(sizeof(struct s_arbre));
        strcpy(a->f_gch->key,b->attribut);
        a->f_gch->f_drt=NULL;
        a->f_gch->f_gch=NULL;
    }
    else
    {
        aide(a->f_gch,b);
    }
    }
    }
}

int             compteur_fd(s_arbre *a)
{
    int i=1;
    while(a->f_drt!=NULL)
    {
        i++;
        a=a->f_drt;
    }
    return i;
}
int             compteur_fg(s_arbre *a)
{
    int i=1;
    while(a->f_gch!=NULL)//calcule la longeur du chaine gauche de l'arbre
    {
        i++;
        a=a->f_gch;
    }
    return i;

}

/* ************************ mode Liste Chainée ************************** */

objets          *lire_liste(char *chemin)
{
    FILE *fichier=NULL;
    fichier=fopen(chemin,"r");
    struct objets *a=(struct objets*) malloc(sizeof(struct objets));
    struct objets *b=a;//b est la structre temporel où on incremente
    b->suivant=NULL;
    int i=0;
    int d=1;
    char c;
    c=fgetc(fichier);
    while(!feof(fichier) && c!='{')//pour enlever tout les caracteres non-reconues
    {
        c=fgetc(fichier);
    }

    while(!feof(fichier)){
        c=fgetc(fichier);
        if(c=='}' && d==1)//pour ne pas tomber dans le cas de objet dans objet
        {  b->obj[i]='\0';
            i=0;
            b->suivant=malloc(sizeof(struct objets));
            b=b->suivant;
            b->suivant=NULL;
            while(c!='{' && !feof(fichier))//pour enlever les espces entre chaque objet
            {
                c=fgetc(fichier);
            }


        }
        else {if(c=='}' && d!=1)//c'est le cas de objet dans objet
        {
            d--;
            b->obj[i]=c;
            i++;

        } else {  if(c=='{')
        {


                d++;
                b->obj[i]=c;
                i++;

        } else{
            if(c!='\n' &  c!=' ' & c!='\t' )
            {
                b->obj[i]=c;
                i++;
            }}}}
    }
    fclose(fichier);
    b=a;
    while(b->suivant->suivant!=NULL)//car on a alloué une case supplémentaire à la fin dont elle est vide
    {
        b=b->suivant;
    }
    b->suivant=NULL;
    return (a);
}

att_val         *attr_vale_liste(objets *a)
{
    struct att_val *t1=(struct att_val*) malloc(sizeof(struct att_val));
    struct att_val  *t2=t1;
    t2->suivant=NULL;
    struct objets *p1=a;
    int k=0,d=0;
    while(p1!=NULL)//la fin du liste
    { while(p1->obj[k]!='\0'){//la fin de chaque chaine de caractere
        while(p1->obj[k]!=':' & p1->obj[k]!='\0' )//Pour prendre l'attribut
        {   if(p1->obj[k]!=',' && p1->obj[k]!='"'){
            t2->attribut[d]=p1->obj[k];
            d++;}
            k++;
        }
        t2->attribut[d]='\0';//la fin de l'attribut
        d=0;
        k++;
        if(p1->obj[k]!='[' && p1->obj[k]!='{' && p1->obj[k]!='"')//le cas d'une valeur numérique
        {
            while(p1->obj[k]!=',' && p1->obj[k]!='\0' )//la fin du valeur
            {
                t2->valeur[d]=p1->obj[k];
                d++;
                k++;
            }
        }
        else {
            if(p1->obj[k]=='[' || p1->obj[k]=='{')//le cas d'une valeur tableau ou objet
            {
                while(p1->obj[k]!=']' && p1->obj[k]!='\0' && p1->obj[k]!='}')
                {  if(p1->obj[k]!='[' && p1->obj[k]!='{'){
                    t2->valeur[d]=p1->obj[k];
                    d++;}
                    k++;
                }
            } else {
                while(p1->obj[k]!=',' & p1->obj[k]!='\0' || p1->obj[k-1]!='"' )//les cas d'une valeur chaine de caractère
                {
                    if(p1->obj[k]!=',' & p1->obj[k]!='"'){
                        t2->valeur[d]=p1->obj[k];
                        d++;}
                    k++;
                }}}
        t2->valeur[d]='\0';
        d=0;
        t2->suivant=malloc(sizeof(struct att_val));//incrementer t2
        t2=t2->suivant;
        t2->suivant=NULL;
        if(p1->obj[k]!='\0'){
            k++;}}
        k=0;
        p1=p1->suivant;
    }
    t2=t1;
    while(t2->suivant->suivant!=NULL)
    {
        t2=t2->suivant;
    }
    t2->suivant=NULL;
    return (t1);
}

char            **lire_f(int j,char *chemin)
{
    FILE* fichier=NULL;
    fichier=fopen(chemin,"r");
    int i=0;
    char c;
    char **p1;
    p1= (char**)malloc((j+2) * sizeof(char*));
    for(i=0;i<j+1;i++)
    {
        p1[i]=(char*)malloc(300 * sizeof(char*));
    }
    i=0;
    int k=0;
    int d=1;
    c=fgetc(fichier);
    while(c!='{' && !feof(fichier))
    {
        c=fgetc(fichier);
    }
    while(!feof(fichier)){
        c=fgetc(fichier);
        if(c=='}' && d==1)
        {  p1[k][i]='\0';
            i=0;
            k++;
            while(c!='{' && !feof(fichier))
            {
                c=fgetc(fichier);
            }


        }
        else {if(c=='}' && d!=1)
        {
            d--;
            p1[k][i]=c;
            i++;

        } else {  if(c=='{')
        {
            d++;
            p1[k][i]=c;
            i++;

        } else{
            if(c!='\n' &&  c!=' ' && c!='\t' )
            {
                p1[k][i]=c;
                i++;
            }}}}
    }
    fclose(fichier);
    return (p1);
}

int             nb_obj(char *chemin)
{
    FILE* fichier=NULL;
    fichier=fopen(chemin,"r");
    int j=0;
    int k=0;
    char c;
    while(!feof(fichier)){
        c=fgetc(fichier);
        if(c=='{')
        {
            j++;
        }
        else { if(c=='}' && j==1)//pour enlever le cas de objet dans objet
        {
            k++;
            j--;
        }
        else {if(c=='}' && j!=1)
        {
            j--;
        }}}
    }
    fclose(fichier);
    return k;
}

att_val         *attr_vale(char **p1,int j)
{
    struct att_val *t1=(struct att_val*) malloc(sizeof(struct att_val));
    struct att_val  *t2=t1;
    t2->suivant=NULL;
    int i=0,k=0,d=0;
    for(i=0;i<j;i++)
    { while(p1[i][k]!='\0'){
        while(p1[i][k]!=':' & p1[i][k]!='\0' )
        {   if(p1[i][k]!=',' & p1[i][k]!='"'){
            t2->attribut[d]=p1[i][k];
            d++;}
            k++;
        }
        t2->attribut[d]='\0';
        d=0;
        k++;
        if(p1[i][k]!='[' && p1[i][k]!='{' && p1[i][k]!='"')
        {
            while(p1[i][k]!=',' && p1[i][k]!='\0' )
            {
                t2->valeur[d]=p1[i][k];
                d++;
                k++;
            }
        }
        else {
            if(p1[i][k]=='[' || p1[i][k]=='{')
            {
                while(p1[i][k]!=']' && p1[i][k]!='\0' && p1[i][k]!='}')
                {  if(p1[i][k]!='[' && p1[i][k]!='{'){
                    t2->valeur[d]=p1[i][k];
                    d++;}
                    k++;
                }
            } else {
                while(p1[i][k]!=',' & p1[i][k]!='\0' || p1[i][k-1]!='"' )
                {
                    if(p1[i][k]!=',' & p1[i][k]!='"'){
                        t2->valeur[d]=p1[i][k];
                        d++;}
                    k++;
                }}}
        t2->valeur[d]='\0';
        d=0;
        t2->suivant=malloc(sizeof(struct att_val));
        t2=t2->suivant;
        t2->suivant=NULL;
        if(p1[i][k]!='\0'){
            k++;}}
        k=0;
    }
    t2=t1;
    while(t2->suivant->suivant!=NULL)
    {
        t2=t2->suivant;
    }
    t2->suivant=NULL;
    return (t1);
}

nb_occ *nbr_occu(att_val *t1)
{
    struct att_val *t2=t1;
   struct att_val *t3=t1;
   int k=0;
    struct nb_occ *d1=(struct nb_occ*) malloc(sizeof(struct nb_occ));
    struct nb_occ *d2=d1;
    d2->suivant=NULL;
    int i=0;
    int d=0;
    int a=0;
   while(t2!=NULL)
   {
   while(t3!=NULL)
   {
       if(!strcmp(t2->attribut,t3->attribut))
       {
           if(i<=k)//pour être sûre qu'on a pas déja traité cet attribut
           {
              d++;
           }
           else {//a=-1 si on a déja traité cet attribut
            a=-1;
           }
       }
       t3=t3->suivant;
       k++;
   }
   if(a==0)
   {
       strcpy(d2->attrib,t2->attribut);
       d2->n=d;
       d2->suivant=malloc(sizeof(struct nb_occ));
       d2=d2->suivant;
       d2->suivant=NULL;

   }
   t3=t1;
   t2=t2->suivant;
   i++;
   k=0;
   a=0;
   d=0;
   }
   d2=d1;
     while(d2->suivant->suivant!=NULL)
     {
         d2=d2->suivant;
     }
     d2->suivant=NULL;
     return (d1);
}

nb_occ          *nbr_occu1(att_val *t1)
{
    struct nb_occ *d1=(struct nb_occ*) malloc(sizeof(struct nb_occ));
    struct att_val *t2=t1;
    struct att_val *t3=t1;
    struct nb_occ *d2=d1;
    d2->suivant=NULL;
    int k=1;
    while(t3!=NULL)
    {
        while(t2!=NULL)
        {  if(t2->suivant!=NULL)
        {
            if(!strcmp(t3->attribut,t2->suivant->attribut) )//on compare un attribut avec l'attribut suivant si ils sont égals on le dépasse
            {
                k++;
                t2->suivant=t2->suivant->suivant;
            }
            else {
                t2=t2->suivant;
            }}
        else t2=t2->suivant;
        }

        strcpy(d2->attrib,t3->attribut);
        d2->n=k;
        d2->suivant=malloc(sizeof(struct nb_occ));
        d2=d2->suivant;
        d2->suivant=NULL;
        t3=t3->suivant;
        t2=t3;
        k=1;
    }
    d2=d1;
    while(d2->suivant->suivant!=NULL)
    {
        d2=d2->suivant;
    }
    d2->suivant=NULL;
    return (d1);


}

nb_occ *nbr_occu_lev(att_val *t1)
{  //meme methode de nbr_occ1 sauf que la comparaison n'est pas exacte
    struct nb_occ *d1=(struct nb_occ*) malloc(sizeof(struct nb_occ));
    struct att_val *t2=t1;
    struct att_val *t3=t1;
    struct nb_occ *d2=d1;
    d2->suivant=NULL;
    int k=1;
    while(t3!=NULL)
    {
        while(t2!=NULL)
        {  if(t2->suivant!=NULL)
        {
            if(levenshtein(t3->attribut,t2->suivant->attribut)<=2 )
            {
                k++;
                t2->suivant=t2->suivant->suivant;
            }
            else {
                t2=t2->suivant;
            }}
        else t2=t2->suivant;
        }

        strcpy(d2->attrib,t3->attribut);
        d2->n=k;
        d2->suivant=malloc(sizeof(struct nb_occ));
        d2=d2->suivant;
        d2->suivant=NULL;
        t3=t3->suivant;
        t2=t3;
        k=1;

    }
    d2=d1;
    while(d2->suivant->suivant!=NULL)
    {
        d2=d2->suivant;
    }
    d2->suivant=NULL;
    return (d1);
}

t_att		*occurence_liste(t_object *src, char *chemin)
{
	int			i;
	int			verif;
	int			no_att;

	no_att = no_attributs(lire(chemin));
	t_att *tmp;
	t_att *tmp2;
	t_att *dest;
	dest = malloc(sizeof(struct s_att));
	dest->att = strdup(src[0].attribut);
	dest->occur = 0;
	dest->next = NULL;
	tmp = dest; /*garder l'adresse du premier maillon*/
	tmp2 = dest;
	i = 0;
	while (i < no_att)
	{
		verif = vrf_liste(tmp2, src, i); /*comparer l'attribut actuel avec les attributs deja stocker*/
		if (verif)
		{
			dest->next = malloc(sizeof(struct s_att));
			dest->next->att = strdup(src[i].attribut);
			dest->next->occur = 0;
			dest->next->next = NULL;
			dest = dest->next;
		}
		tmp2 = tmp;
		i++;
	}
	dest = tmp;
	while (dest)
	{
		i = 0;
		tmp2 = dest;
		while (i < no_att)
		{
			if (!(strcmp(dest->att, src[i].attribut)))
				dest->occur++;
			i++;
		}
		dest = tmp2->next; /*passer au maillon suivant*/
	}
	return (tmp);
}

        /**//**//**//**//**//**//**//**//**//**//*  main  *//**//**//**//**//**//**//**//**//**//**//**/

int		main()
{
	char    chemin[100];
	char    choix[50];
    char    theme[50];
	int     quitter = 0;
    int     quitter_t = 0;
    int     quitter_l = 0;
	int     fd;

    printf("\n\t /*\\/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\");
    printf("\n\t|                                                                          |");
    printf("\n\t|                                                                          |");
    printf("\n\t|                       \"Projet algorithmique\"                             |");
    printf("\n\t|                     Gestion D'un Fichier JSON                            |");
    printf("\n\t|                                                                          |");
    printf("\n\t|    Realise par : oouklich                                                |");
    printf("\n\t|                  arachidi                                                |");
    printf("\n\t|                                                                          |");
    printf("\n\t|                                                                          |");
    printf("\n\t \\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/*\\*/");
	do{
		printf("\n\n\t\t=> entrez le chemin : ");
		scanf("%s", chemin);
		fd = open(chemin, O_RDONLY);
	}while (fd < 0);
    close(fd);
    do {
    do {
        printf("\n   ------------------------------------------------------------------------------------- ");
        printf("\n  |  mode \"tableau dynamique\"                                                   |   1   |");
        printf("\n  ---------------------------------------------------------------------------------------");
        printf("\n  |  mode \"liste chainee\"                                                       |   2   |");
        printf("\n  ---------------------------------------------------------------------------------------");
        printf("\n  |  quitter le programme                                                       |   3   |");
        printf("\n   ------------------------------------------------------------------------------------- ");
        printf("\n\n\t\t\t-> ");
        scanf("%s", theme);
    }while (strcmp(theme, "1") && strcmp(theme, "2") && strcmp(theme, "3"));
    if (!(strcmp(theme, "1")))
    {
	do{
        quitter_t = 0;
		do{
            printf("\n   ------------------------------------------------------------------------------------- ");
			printf("\n  |  tester la fonction lire par afficher tous les objets                       |   1   |");
            printf("\n  ---------------------------------------------------------------------------------------");
			printf("\n  |  tester la fonction \"ATTRIBUT VALEUR\"                                       |   2   |");
            printf("\n  ---------------------------------------------------------------------------------------");
			printf("\n  |  tester la fonction \"occurence tableau\"                                     |   3   |");
            printf("\n  ---------------------------------------------------------------------------------------");
			printf("\n  |  tester la fonction \"occurence hachage\"                                     |   4   |");
            printf("\n  ---------------------------------------------------------------------------------------");
			printf("\n  |  tester la fonction \"occurence arbre\"                                       |   5   |");
            printf("\n  ---------------------------------------------------------------------------------------");
			printf("\n  |  changer le chemin                                                          |   6   |");
            printf("\n  ---------------------------------------------------------------------------------------");
			printf("\n  |   quitter le mode                                                           |   7   |");
            printf("\n   ------------------------------------------------------------------------------------- ");
            printf("\n\n\t\t\t-> ");
			scanf("%s", choix);
		}while (strcmp(choix, "1") && strcmp(choix, "2") && strcmp(choix, "3") && strcmp(choix, "4") && strcmp(choix, "5") && strcmp(choix, "6") && strcmp(choix, "7"));
		if (!(strcmp(choix, "3")))
		{
			int i = 0;
			t_attribut *dest;
            clock_t start = clock();
			dest = occurence_tab(tab_dynamique(lire(chemin)),chemin);
            clock_t end = clock();
            double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
			while (dest[i].att[0])
			{
				printf("%s%s;\t%d\n----------------------------\n",dest[i].att,space(SPACE_LONG - strlen(dest[i].att)),dest[i].occur);
				i++;
			}
            printf ("\nYour function took %f seconds to run.\n", cpu_time_used);
			free(dest);
		}
		else if (!(strcmp(choix,"1")))
		{
			char **dest;
			int i = 0;
            clock_t start = clock();
			dest = lire(chemin);
            clock_t end = clock();
            double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
			while (dest[i])
			{
				printf("%s\n----------------------------------------------------\n",dest[i]);
				i++;
			}
            printf ("\nYour function took %.2lf seconds to run.\n", cpu_time_used );
			free(*dest);
		}
		else if (!(strcmp(choix,"2")))
		{
			t_object *dest;
			int i = 0;
            clock_t start = clock();
			dest = tab_dynamique(lire(chemin));
            clock_t end = clock();
            double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
			while (dest[i].attribut[0])
			{
				printf("attribut : %s%s|   valeur : %s\n------------------------------------------------------------------------------------------\n",dest[i].attribut,space(SPACE_LONG - strlen(dest[i].attribut)),dest[i].valeur);
				i++;
			}
            printf ("\nYour function took %.2lf seconds to run.\n", cpu_time_used );
			free(dest);
		}
		else if (!(strcmp(choix,"4")))
		{
			int i = 0;
			t_attribut *dest;
            int         HACH_SIZE;
            HACH_SIZE = no_attributs(lire(chemin));
            clock_t start = clock();
			dest = occurence_hachage(tab_dynamique(lire(chemin)),chemin);
            clock_t end = clock();
            double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
			for (i = 0;i < HACH_SIZE;i++)
			{
				if (dest[i].occur != 0)
					printf("%s%s;\t%d\n----------------------------\n",dest[i].att,space(SPACE_LONG - strlen(dest[i].att)),dest[i].occur);
			}
            printf ("\nYour function took %.2lf seconds to run.\n", cpu_time_used );
			free(dest);
		}
		else if (!(strcmp(choix,"5")))
		{
            clock_t start = clock();
            int j=nb_obj(chemin);
            struct s_arbre *a = arbre(attr_vale(lire_f(j,chemin),j));
            struct nb_occ1 *b;
            b=nbr_occu_arbre(a);
            int i=compteur_fg(a);
            int k;
            clock_t end = clock();
            double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            for(k=0;k<i;k++)
            {
                printf("%s%s;\t%d \n----------------------------\n",b[k].attribut,space(SPACE_LONG - strlen(b[k].attribut)),b[k].n);
            }
            printf ("\nYour function took %.2lf seconds to run.\n", cpu_time_used );
            free(b);
		}
		else if (!(strcmp(choix,"6")))
		{
			do{
				printf("\n\tentrez le nouveau chemin :  ");
				scanf("%s",chemin);
				fd = open(chemin, O_RDONLY);
			}while (fd < 0);
			close(fd);
		}
		else
			quitter_t = 1;
	}while (quitter_t == 0);
    }
    else if (!(strcmp(theme,"2")))
    {
        do{
        do{
            quitter_l = 0;
            printf("\n   ------------------------------------------------------------------------------------- ");
            printf("\n  |  tester la fonction lire_liste par afficher tous les objets                 |   1   |");
            printf("\n  ---------------------------------------------------------------------------------------");
            printf("\n  |  tester la fonction \"ATTRIBUT VALEUR\"                                       |   2   |");
            printf("\n  ---------------------------------------------------------------------------------------");
            printf("\n  |  tester la fonction \"occurence levenshtein\"                                 |   3   |");
            printf("\n  ---------------------------------------------------------------------------------------");
            printf("\n  |  tester la fonction \"occurence nbr_occu\"                                    |   4   |");
            printf("\n  ---------------------------------------------------------------------------------------");
            printf("\n  |  tester la fonction \"occurence nbr_occu1\"                                   |   5   |");
            printf("\n  ---------------------------------------------------------------------------------------");
            printf("\n  |  tester la fonction \"occurence liste\"                                       |   6   |");
            printf("\n  ---------------------------------------------------------------------------------------");
            printf("\n  |  changer le chemin                                                          |   7   |");
            printf("\n  ---------------------------------------------------------------------------------------");
            printf("\n  |   quitter le mode                                                           |   8   |");
            printf("\n   ------------------------------------------------------------------------------------- ");
            printf("\n\n\t\t\t-> ");
            scanf("%s", choix);
        }while (strcmp(choix, "1") && strcmp(choix, "8") && strcmp(choix, "2") && strcmp(choix, "3") && strcmp(choix, "4") && strcmp(choix, "5") && strcmp(choix, "6") && strcmp(choix, "7") && strcmp(choix, "8"));
        if (!(strcmp(choix,"1")))
        {
            struct objets *a;
            clock_t start = clock();
            a = lire_liste(chemin);
            clock_t end = clock();
            double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            while (a)
            {
                printf("%s\n-----------------------------------------------------\n",a->obj);
                a = a->suivant;
            }
            printf ("\nYour function took %.2lf seconds to run.\n", cpu_time_used );
            free(a);
        }
        else if (!(strcmp(choix,"2")))
        {
            clock_t start = clock();
            att_val     *a = attr_vale_liste(lire_liste(chemin));
            clock_t end = clock();
            double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            while (a)
            {
                printf("attribut : %s%s|   valeur : %s\n------------------------------------------------------------------------------------------\n",a->attribut,space(SPACE_LONG - strlen(a->attribut)),a->valeur);
                a = a->suivant;
            }
            printf ("\nYour function took %.2lf seconds to run.\n", cpu_time_used );
            free(a);
        }
        else if (!(strcmp(choix,"3")))
        {
            struct nb_occ *a;
            clock_t start = clock();
            a = nbr_occu_lev(attr_vale_liste(lire_liste(chemin)));
            clock_t end = clock();
            double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            while (a)
            {
                printf("%s%s;\t%d\n----------------------------\n",a->attrib,space(SPACE_LONG - strlen(a->attrib)),a->n);
                a = a->suivant;
            }
            printf ("\nYour function took %.2lf seconds to run.\n", cpu_time_used );
            free(a);
        }
        else if (!(strcmp(choix,"4")))
        {
            clock_t start = clock();
            struct nb_occ *a;
            a = nbr_occu(attr_vale_liste(lire_liste(chemin)));
            clock_t end = clock();
            double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            while (a)
            {
                printf("%s%s;\t%d\n----------------------------\n",a->attrib,space(SPACE_LONG - strlen(a->attrib)),a->n);
                a = a->suivant;
            }
            printf ("\nYour function took %.2lf seconds to run.\n", cpu_time_used );
            free(a);
        }
        else if (!(strcmp(choix,"5")))
        {
            clock_t start = clock();
            struct nb_occ *a;
            a = nbr_occu1(attr_vale_liste(lire_liste(chemin)));
            clock_t end = clock();
            double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            while (a)
            {
                printf("%s%s;\t%d\n----------------------------\n",a->attrib,space(SPACE_LONG - strlen(a->attrib)),a->n);
                a = a->suivant;
            }
            printf ("\nYour function took %.2lf seconds to run.\n", cpu_time_used );
            free(a);
        }
        else if (!(strcmp(choix,"6")))
        {
            clock_t start = clock();
            t_att *dest;
            dest = occurence_liste(tab_dynamique(lire(chemin)),chemin);
            clock_t end = clock();
            double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            while (dest)
            {
                printf("%s%s;\t%d\n----------------------------\n",dest->att,space(SPACE_LONG - strlen(dest->att)),dest->occur);
                dest = dest->next;
            }
            printf ("\nYour function took %.2lf seconds to run.\n", cpu_time_used );
            free(dest);
        }
        else if (!(strcmp(choix,"7")))
        {
            do{
                printf("\n\tentrez le nouveau chemin :  ");
                scanf("%s",chemin);
                fd = open(chemin, O_RDONLY);
            }while (fd < 0);
            close(fd);
        }
        else
            quitter_l = 1;
        }while (quitter_l == 0);
    }
    else
        quitter = 1;
    }while (quitter == 0);
	return (0);
}
