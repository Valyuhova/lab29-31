#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum {LBRACE=257, RBRACE, READL, PRITL, RETRL, IFL, THENL, DOL, WHILEL, INTL, CONSTL, IDEN, NUMB};

int nst = 0, lval, lex;
char nch = '\n';

FILE *PF, *padres;
char TNM[400], *ptn = TNM;

void get(void);
void number(void);
void word(void);
char *add(char *nm);

int main() 
{
    PF = fopen("input.spl", "r");
    padres = fopen("identifiers.txt", "w");

    if (!PF) 
    {
        puts("File input.spl isn't open");
        return 1;
    }

    get();

    fclose(PF);
    fclose(padres);
    return 0;
}

void get() 
{
    while (nch != EOF) 
    {
        while (isspace(nch)) 
        {
            if (nch == '\n') nst += 1;
            nch = getc(PF);
        }

        if (isdigit(nch)) number();
        else if (isalpha(nch)) word();

        else if (nch == '{') 
        {
            lex = LBRACE;
            nch = getc(PF);
        } 
        
        else if (nch == '}') 
        {
            lex = RBRACE;
            nch = getc(PF);
        } 
        
        else if (strchr("(),;=+-*/%", nch)) 
        {
            lex = nch;
            nch = getc(PF);
        } 
        
        else if (nch == EOF) 
        {
            lex = EOF;
        } 
        
        else 
        {
            printf("Invalid symbol: %c\n", nch);
            exit(1);
        }
    }
}

void number() 
{
    lval = 0;

    while (isdigit(nch)) 
    {
        lval = lval * 10 + nch - '0';
        nch = getc(PF);
    }

    lex = NUMB;
}

void word() 
{
    static int cdl[] = {READL, PRITL, RETRL, IFL, THENL, WHILEL, DOL, INTL, CONSTL, IDEN, NUMB};
    static char *serv[] = {"read", "print", "return", "if", "then", "while", "do", "int", "const"};

    char tx[40], *p = tx;

    while (isalnum(nch)) 
    {
        *p++ = nch;
        nch = getc(PF);
    }

    *p = '\0';

    for (int i = 0; i < 9; i += 1) 
    {
        if (strcmp(tx, serv[i]) == 0) 
        {
            lex = cdl[i];
            return;
        }
    }

    lex = IDEN;
    lval = (int)add(tx);
    printf("Address for %s = %p\n", tx, (void *)lval);
    fprintf(padres, "Address for %s = %p\n", tx, (void *)lval);
}

char *add(char *nm) 
{
    for (char *p = TNM; p < ptn; p += strlen(p) + 1) 
    {
        if (strcmp(p, nm) == 0) return p;
    }

    if ((ptn + strlen(nm) + 1) > TNM + sizeof(TNM)) 
    {
        puts("Overload of TNM");
        exit(1);
    }

    strcpy(ptn, nm);
    char *result = ptn;
    ptn += strlen(nm) + 1;
    return result;
}
