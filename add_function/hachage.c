#include "header.h"

int hachage(char *chaine, int HACH_SIZE)
{
    int i = 0;
    int nombreHache = 0;

    for (i = 0 ; chaine[i] != '\0' ; i++)
    {
        nombreHache += chaine[i] * (2 ^ i);
    }
    nombreHache %= HACH_SIZE;

    return nombreHache;
}
