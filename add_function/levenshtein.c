#include "header.h"

int levenshtein(char *a,char *b)
{
    int i,j,lenght1,lenght2;
    lenght1=strlen(a);
    lenght2=strlen(b);
    int mat[lenght1+1][lenght2+1];
    mat[0][0]=0;
    for(i=1;i<lenght1+1;i++)
    {
        mat[i][0]=i;
    }
    for(j=1;j<lenght2+1;j++)
    {
        mat[0][j]=j;
    }
    for(i=1;i<lenght1+1;i++)
    {
        for(j=1;j<lenght2+1;j++)
        {
            mat[i][j]=min(mat[i-1][j]+1,mat[i][j-1]+1,mat[i-1][j-1]+str_c(a[i],b[j]));
        }
    }
    return mat[lenght1][lenght2];
}
