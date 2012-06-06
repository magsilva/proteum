/* Copyright (C) 2012 -- Marcio Eduardo Delamaro and Jose Carlos Maldonado
# 
# 
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
# 
*/


/*######################################################################
				S I M B T A B . C
	Este modulo implementa as funcoes das tabelas de simbolos do 
parser 


##########################################################################*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include 	"li.h" 

SIMBOLO		*aloca_no();

/**************************************************************************
CRIA_TAB:
	Cria tabela de simbolos
**************************************************************************/

void cria_tab(tab)
TABSIMBOLO	*tab;
{
int	i;

   for (i = 0; i < LEN(tab->vet); i++)
      {
	tab->vet[i] = NULL;
       }
   tab->topo = -1;
}




/**************************************************************************
ESVAZIA_TAB:
	Libera todos os elementos de uma tabela;

**************************************************************************/
void esvazia_tab(tab, n)
TABSIMBOLO	*tab;
int	n;
{
int	i;

   if (n > tab->topo)
	     return;

   for( i = n;  i <=  tab->topo; i++)
      {
	 if (i != 1) 	/* level 1 has simbolos used in level 0 */
	    lib_arvore(tab->vet[i]);
	 tab->vet[i] = NULL;
       }
   tab->topo = n-1;
}


/**************************************************************************
PRINT_TAB:
	Imprime tabela
**************************************************************************/

void print_tab(tab)
TABSIMBOLO	*tab;
{
int	i;

   for( i = 0;  i <= tab->topo; i++)
      {
	printf("\n\n######################  NIVEL %d ##################", i);
	print_arvore(tab->vet[i]);
       }
}





/***************************************************************************
FIND_SIMBOLO:
	Procura simbolo pelo nome na tabela
****************************************************************************/
void find_simbolo(s, tab, nivel, onde)
char	*s;
TABSIMBOLO	*tab;
int	*nivel;
SIMBOLO	**onde;
{
int	i;
SIMBOLO	*p, *find_arvore();

   *onde = NULL;
   *nivel = -1;
   for (i = tab->topo; i >=0; i--)
      {
	if ((p = find_arvore(s, tab->vet[i])) != NULL)
	         {
		    *nivel = i;
		    *onde = p;
		    break;
		  }
       }
}



/***************************************************************************
FIND_ENUM_CONST:
        Procura constant enum pelo nome na tabela
****************************************************************************/
void find_enum_const(s, tab, nivel, onde)
char    *s;
TABSIMBOLO      *tab;
int     *nivel;
SIMBOLO **onde;
{
int     i;
SIMBOLO *p, *find_arvore_enum_const();

   *onde = NULL;
   *nivel = -1;
   for (i = tab->topo; i >=0; i--)
      {
        if ((p = find_arvore_enum_const(s, tab->vet[i])) != NULL)
                 {
                    *nivel = i;
                    *onde = p;
                    break;
                  }
       }
}


ins_fake_enum(TABSIMBOLO *tab, SIMBOLO *ds)
{
   tab->topo++;
   tab->vet[tab->topo] = ds;
}

delete_fake(TABSIMBOLO *tab)
{
   tab->vet[tab->topo--] = NULL;
}


/***************************************************************************
INS_SIMBOLO:
	Inclui um simbolo na tabela
***************************************************************************/
void ins_simbolo(s, tab, nivel, onde)
TABSIMBOLO	*tab;
int	nivel;
SIMBOLO	**onde, *s;
{
SIMBOLO	*p, *cria_arvore(), *ins_arvore();

   if (tab->vet[nivel] == NULL)
	     {
		tab->vet[nivel] = *onde = cria_arvore(s);
		if (tab->topo < nivel)
		        tab->topo = nivel;
	      }
	else  {
		*onde = ins_arvore(s, tab->vet[nivel]);
	      }
}



/***************************************************************************
REPLACE_SIMBOLO:
        Substitui um simbolo na tabela
	Essa rotina eh somente usada para trocar um simbolo definido
como parametro, nos antigos moldes, ou seja:

main(argc, argv)
char	*argv[];
{

	Nesse caso, "argv" que foi por default definido como int tem
que ser substituido por char *[];

***************************************************************************/
replace_simbolo(s, ds, tab, nivel)
TABSIMBOLO      *tab;
int     nivel;
SIMBOLO *ds, *s;
{
SIMBOLO r;

	r = *s;
	lib_arvore(s->campos);

	*s = *ds;
	s->npar = r.npar;
	s->dir = r.dir;
	s->esq = r.esq;
	libera_no(ds);
	replace_pai(s->campos, s);
}

replace_pai(raiz, pai)
SIMBOLO	*raiz, *pai;
{
   if (raiz == NULL)
	return;
   raiz->pai = pai;
   replace_pai(raiz->esq, pai);
   replace_pai(raiz->esq, pai);
}







/****************************************************************************
CRIA_ARVORE:
	cria arvore binaria com um no
****************************************************************************/
SIMBOLO *cria_arvore(s)
SIMBOLO	*s;
{
SIMBOLO	*p;

/*    p = aloca_no(); */
   p = s;
   if (p != NULL)
           {
		*p = *s;
/* 		p->esq = p->dir = NULL; */
	   }
   return p;
}


/*************************************************************************
FIND_ARVORE:
	Procura no da arvore pelo nome
**************************************************************************/
SIMBOLO *find_arvore(s, raiz)
SIMBOLO	*s, *raiz;
{
int	x;

   if (raiz == NULL)
	     return NULL;

   x = strcmp(s->nome, raiz->nome);
   if (x < 0)
	     return find_arvore(s, raiz->esq);
	else
   if (x > 0)
	     return find_arvore(s, raiz->dir);
	else return raiz;
}



/*************************************************************************
FIND_ARVORE_ENUM_CONST:
        Procura constante enum arvore pelo nome
**************************************************************************/
SIMBOLO *find_arvore_enum_const(s, raiz)
SIMBOLO *s, *raiz;
{
int     x;
SIMBOLO *onde;

   if (raiz == NULL)
             return NULL;

   if (raiz->specifier.f_enum)
   {
	onde = find_arvore(s, raiz->campos);
        if (onde != NULL)
	   return onde;
   }

   onde = find_arvore_enum_const(s, raiz->esq);

   if (onde != NULL)
	return onde;

   return find_arvore_enum_const(s, raiz->dir);
}



/**************************************************************************
INS_ARVORE:
	Insere simbolo na arvore
**************************************************************************/
SIMBOLO *ins_arvore(s, raiz)
SIMBOLO	*s, *raiz;
{
SIMBOLO	*p, **q;
static	SIMBOLO	r;

   if (strcmp(s->nome, raiz->nome) == 0)
	     {		/* se nomes sao iguais, deixa o que estava
			pois isso nao deve ocorrer se o prog. estah corretp */
		return raiz;
	      }
   if (strcmp(s->nome, raiz->nome) <= 0)
	     q = &(raiz->esq);
	else q = &(raiz->dir);
   if (*q == NULL)
	     {
/*		if ((p = aloca_no()) == NULL)
		          return NULL; */
		p = s;
		*p = *s;
		p->esq = p->dir = NULL;	
		*q = p;
		return p;
	      }
	else {
		return ins_arvore(s, *q);
	     }
}


/************************************************************************
LIB_ARVORE:
	Libera todos os nos da arvore
*************************************************************************/
lib_arvore(raiz)
SIMBOLO	*raiz;
{

   if (raiz == NULL)
	     return OK;
   lib_arvore(raiz->esq);
   lib_arvore(raiz->dir);
   lib_arvore(raiz->campos);
   libera_no(raiz);
   return OK;
}



/**************************************************************************
PRINT_ARVORE:
*************************************************************************/
print_arvore(raiz)
SIMBOLO	*raiz;
{
   if (raiz == NULL)
	return OK;

   printf("\n%-15.15s\t%-15.15s\t%-15.15s", raiz->nome,
		raiz->tipo, raiz->ptar);
   print_arvore(raiz->campos);
   print_arvore(raiz->esq);
   print_arvore(raiz->dir);
   return OK;
}




/**************************************************************************
ALOCA/LIBERA_NO:
	Aloca e libera nos da arqvore de tipos
***************************************************************************/
SIMBOLO	*aloca_no()
{
SIMBOLO	*p;

   p = (SIMBOLO *) malloc(sizeof(SIMBOLO));
   if (p != NULL)
	      memset(p, 0, sizeof(SIMBOLO));
   return  p;
 }

libera_no(p)
SIMBOLO	*p;
{
   if (p != NULL)
	     free(p);
   return OK;
}


