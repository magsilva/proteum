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


/*******************************************************************
			 P G E R A I S . C
	Rotinas comuns usadas por varios operadores de mutacao.

*******************************************************************/

#include	<stdio.h>

#include	"polonesa.h"
#include	<lib/gerais.h>
#include	<li/lib/li.h>



static	OSET	set_campo;

char	scbuf[2048];


static	char	*ptipo, *pptar;	



SIMBOLO	*grupo_var[100];

extern	TABSIMBOLO	tab_vars, tab_tipos;

void	(*proc_visit)();



/*************************************************************************
E_ESCALAR:
	Verifica se tipo de variavel e' escalar 
**************************************************************************/
e_escalar(tipo, ptar)
char	*tipo, *ptar;
{
   return (e_char(tipo, ptar) || e_float(tipo, ptar) || e_int(tipo, ptar));
}




/*************************************************************************
E_INTEIRO:
	Verifica se tipo de variavel e' inteira 
**************************************************************************/
e_inteiro(tipo, ptar)
char	*tipo, *ptar;
{

    return e_char(tipo, ptar) || e_int(tipo, ptar);
}



/*************************************************************************
E_ARRAY:
	Verifica se tipo de variavel e' array 
**************************************************************************/
e_array(tipo, ptar)
char	*tipo, *ptar;
{
SIMBOLO	*p;
int	n, i = strlen(ptar);

   if (i > 0)
   	     return ptar[i-1] == '[';

   find_simbolo(tipo, &tab_tipos, &n, &p);
   if (p == NULL)
	     return FALSE;
   return e_array(p->tipo, p->ptar);
}




/*************************************************************************
E_FUNC:
	Verifica se tipo de variavel e' funcao
**************************************************************************/
e_funcao(tipo, ptar)
char	*tipo, *ptar;
{
SIMBOLO	*p;
int	n, i = strlen(ptar);

   if (i > 0)
   	     return ptar[i-1] == '(';

   find_simbolo(tipo, &tab_tipos, &n, &p);
   if (p == NULL)
	     return FALSE;
   return e_funcao(p->tipo, p->ptar);
}




/*************************************************************************
E_POINTER:
	Verifica se tipo de variavel e' pointer 
**************************************************************************/
e_pointer(tipo, ptar)
char	*tipo, *ptar;
{
SIMBOLO	*p;
int	n, i = strlen(ptar);

   if (i > 0)
   	     return ptar[i-1] == '*';

   find_simbolo(tipo, &tab_tipos, &n, &p);
   if (p == NULL)
	     return FALSE;
   return e_pointer(p->tipo, p->ptar);
}


/**************************************************************************
GET_VAR_TIPO:
	Devolve tipo de uma variavel;
***************************************************************************/
get_tipo_varr(var, tipo, ptar)
char	*var, *tipo, *ptar;
{
SIMBOLO	*p;
int	n;

   *ptar = '\0';
   *tipo = '\0';
   find_simbolo(var, &tab_vars, &n, &p);
   if (p == NULL)
	     {
		strcpy(tipo, "int");
		strcpy(ptar, "(");
		return ERRO;
	      }
   return monta_tipo(p, tipo, ptar);
}

get_tipo_sim(var, tipo, ptar)
char    *var, *tipo, *ptar;
{
SIMBOLO *p;
int     n;

   *ptar = '\0';
   *tipo = '\0';
   find_simbolo(var, &tab_vars, &n, &p);
   if (p == NULL)
             {
                strcpy(tipo, "int");
                strcpy(ptar, "(");
                return ERRO;
              }
   strcpy(tipo, p->tipo);
   strcpy(ptar, p->ptar);
   return OK;
}


monta_tipo(p, tipo, ptar)
char	*tipo, *ptar;
SIMBOLO	*p;
{
int	n;
SIMBOLO	*q;

   if (! p->specifier.f_other)
   {
	strcpy(tipo, p->tipo);
	strcpy(ptar, p->ptar);
	return OK;
   }

   find_simbolo(p->tipo, &tab_tipos, &n, &q);
   if (q == NULL || monta_tipo(q, tipo, ptar) == ERRO)
	     return ERRO;
   strcat(ptar, p->ptar);
   return OK;
}


/**************************************************************************
GET_TIPO_CAMPO:
	Devolve o tipo de um campo de estrutura.
***************************************************************************/
get_tipo_campo(stru, campo, tipo, ptar)
char	*stru, *campo, *tipo, *ptar;
{
int	n;
SIMBOLO	*p, *q, *find_arvore();

   *ptar = '\0';
   *tipo = '\0';
   find_simbolo(stru, &tab_tipos, &n, &p);
   if (p == NULL || (q = find_arvore(campo, p->campos)) == NULL)
	return ERRO;
   return monta_tipo(q, tipo, ptar);
}



/****************************************************************************
CUR_PAR:
	Returns the nth parameter of the currently being analyzed function

Parameter:
int	n:	The number of the parameter
****************************************************************************/
SIMBOLO *cur_par_aux(SIMBOLO *p, int n)
{
SIMBOLO *q;

   if (p == NULL)
	return p;
   if (p->npar == n)
	return p;
   q = cur_par_aux(p->esq, n);
   if (q == NULL)
	return cur_par_aux(p->dir,n);
   else
	return q;
}

SIMBOLO *cur_par(int n)
{
   return cur_par_aux(tab_vars.vet[1], n);
}



/****************************************************************************
COMPARA_TIPOS:
	Compara se tipos passados como parametros sao equivalentes

****************************************************************************/
compara_tipos(tipo1, ptar1, tipo2, ptar2)
char	*tipo1, *tipo2, *ptar1, *ptar2;
{
static	char	t1[100], p1[100],
		t2[100], p2[100];
SIMBOLO	aux;
int	i,j;

   memset(&aux, 0, sizeof(aux));
   strcpy(aux.tipo, tipo1);
   strcpy(aux.ptar, ptar1);
   monta_tipo(&aux, t1, p1);

   strcpy(aux.tipo, tipo2);
   strcpy(aux.ptar, ptar2);
   monta_tipo(&aux, t2, p2);

   i = strlen(p1);
   j = strlen(p2);

   if (i != j)
	return FALSE;

   while (--i >= 0 && --j >=0)
   {
	if (p1[i] == p2[j])
	   continue;
	if (p1[i] == '*' && p2[j] == '[')
	   continue;
        if (p2[i] == '*' && p1[j] == '[')
           continue;
	return FALSE;
   }

   if (strcmp(t1, t2) == 0)
	     return TRUE;

   if (e_float(t1, p1) && e_float(t2, p2))
	     return TRUE;

   if (e_inteiro(t1, p1) && e_inteiro(t2, p2))
	     return TRUE;

   return FALSE;

}

compara_tipos_sub(tipo1, ptar1, tipo2, ptar2)
char    *tipo1, *tipo2, *ptar1, *ptar2;
{
static  char    t1[100], p1[100],
                t2[100], p2[100];
SIMBOLO aux;
int     i,j;

   memset(&aux, 0, sizeof(aux));
   strcpy(aux.tipo, tipo1);
   strcpy(aux.ptar, ptar1);
   monta_tipo(&aux, t1, p1);

   strcpy(aux.tipo, tipo2);
   strcpy(aux.ptar, ptar2);
   monta_tipo(&aux, t2, p2);

   i = strlen(p1);
   j = strlen(p2);

   if (i != j)
        return FALSE;

   while (--i >= 0 && --j >=0)
   {
        if (p1[i] == p2[j])
           continue;
        if (p1[i] == '*' && p2[j] == '[')
           continue;
        if (p2[i] == '*' && p1[j] == '[')
           continue;
        return FALSE;
   }

   if (strcmp(t1, t2) == 0)
             return TRUE;
   return FALSE;
}


/************************************************************************
E_SOMETHING:
	Verify whether a type is a something 
*************************************************************************/
e_something(sm, tipo, ptar)
char	*sm, *tipo, *ptar;
{
int     n;
SIMBOLO *p;

   if (*ptar != '\0')
	return FALSE;

   if (acha_palavra(sm, tipo) >= 0)
	return TRUE;

   find_simbolo(tipo, &tab_tipos, &n, &p);
   if (p == NULL)
	return FALSE;

   return e_something(sm, p->tipo, p->ptar);
}


e_char(tipo, ptar)
char	*tipo, *ptar;
{
   return e_something("char", tipo, ptar);
}

e_int(tipo, ptar)
char    *tipo, *ptar;
{
   if (e_something("int", tipo, ptar) || e_something("enum", tipo, ptar))
	return TRUE;

   if (*ptar != '\0')
	return FALSE;

   return (! (e_char(tipo, ptar) || e_float(tipo, ptar) ||
			e_void(tipo, ptar) || e_struct(tipo, ptar)));

}

e_float(tipo, ptar)
char    *tipo, *ptar;
{
   return e_something("float", tipo, ptar) || e_something("double", tipo, ptar);
}

e_struct(tipo, ptar)
char    *tipo, *ptar;
{
   return e_something("struct", tipo, ptar) || e_something("union", tipo, ptar);
}


e_void(tipo, ptar)
char    *tipo, *ptar;
{
   return e_something("void", tipo, ptar);
}

e_long(tipo, ptar)
char    *tipo, *ptar;
{
   return e_int(tipo, ptar) && e_something("long", tipo, ptar);
}

e_unsigned(tipo, ptar)
char    *tipo, *ptar;
{
   return e_something("unsigned", tipo, ptar);
}

