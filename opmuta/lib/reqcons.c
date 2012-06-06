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


#include        <lib/gerais.h>
#include        <muta/lib/muta.h>
#include        <polonesa/polonesa.h>
#include        "opmuta.h"

extern  char    tipo[], ptar[];

extern  EXPRE_POS       expre_pos,
                        ex_aux;

extern		SIMBOLO	*cur_par();

extern		TABSIMBOLO	tab_vars, tab_tipos;


char	*int_const = "0\0001\0(-1)\0__MAXINT__\0__MININT__\0";

char	*uint_const = "0\0001\0__MAXUINT__\0";

char	*long_const = "0\0001\0(-1)\0__MAXLONG__\0__MINLONG__\0";

char	*ulong_const = "0\0001\0__MAXULONG__\0";

char	*char_const = "'\\0'\0'\\1'\0((char)(-1))\0__MAXCHAR__\0__MINCHAR__\0";

char	*float_const = "0.0\0001.0\0(-1.0)\0(10E10)\0(10E-10)\0(-10E10)\0(-10E-10)\0";

char	*pointer_const = "\0";

OSET	pc;

VAR_SET	setP,
	setPF,
	setG,
	setL,
	setE,
	setGE,
	setLP;

OSET	setCint,
	setCfloat,
	setCstring;

static	SIMBOLO	*fakeP;

OSET	*get_req_set(char *tipo, char *ptar, TIPO_NO *k)
{

   if (e_pointer(tipo, ptar) || e_array(tipo, ptar))
   {
	*k = SUB_EXPRE;
	set_new_inic(&pc, pointer_const);
   }
   else

   if (e_float(tipo, ptar))
   {
	*k = CONST_FLOAT;
        set_new_inic(&pc, float_const);
   }
   else

   if (e_char(tipo, ptar))
   {
	*k = CONST_CHAR;
        set_new_inic(&pc, char_const);
   }
   else

   if (e_long(tipo, ptar))
   {
	*k = CONST_INT;
	if (e_unsigned(tipo, ptar))
           set_new_inic(&pc, ulong_const);
	else
           set_new_inic(&pc, long_const);
   }
   else

   if (e_int(tipo, ptar))
   {
	*k = CONST_INT;
        if (e_unsigned(tipo, ptar))
           set_new_inic(&pc, uint_const);
        else
           set_new_inic(&pc, int_const);
   }
   else
        set_new_inic(&pc, "\0");


   return &pc;
}


OSET    *get_const_set(char *tipo, char *ptar, TIPO_NO *k)
{

   if (e_char(tipo, "") && (strcmp(ptar, "*") == 0 || strcmp(ptar, "[") == 0))
   {
	*k = STRING;
	return &setCstring;
   }

   if (e_float(tipo, ptar))
   {
	*k = CONST_FLOAT;
	return &setCfloat;
   }

   if (e_inteiro(tipo, ptar))
   {
	*k = CONST_INT;
	return &setCint;
   }
   
   set_new(&pc);
   return &pc;
}


OSET *get_param_set()
{
int	i;
OSET	*p = &pc;
SIMBOLO	*q;

   set_new(p);
   for (i = 0, q = cur_par(0); q != NULL; q = cur_par(++i))
   {
	set_add(p, q->nome);
   }
   return p;
}

/**************************************************************************
INIT_COMPUTE_SET:
	This function initializes (empties) the sets P, G, L, E, C.
**************************************************************************/
void	init_compute_set()
{
   var_set_new(&setP);
   fakeP = NULL;
   var_set_new(&setPF);

   var_set_new(&setG);
   var_set_new(&setL);
   var_set_new(&setE);
   var_set_new(&setGE);
   set_new(&setCint);
   set_new(&setCfloat);
   set_new(&setCstring);
}


void	lib_fakes(SIMBOLO *x)
{
   if (x == NULL)
	return;
   lib_fakes(x->dir);
   free(x);
}

SIMBOLO	*insert_fake(SIMBOLO **p, SIMBOLO *s)
{
SIMBOLO	*q, *aloca_no();
int	i;

   q = aloca_no();
   if (q == NULL)
	return q;
   *q = *s;
   i = strlen(q->ptar);
   if (i > 0)
	q->ptar[i-1] = '\0';
   q->storage.f_typedef = TRUE; 	/* indicates it's a fake simbolo */
   q->dir = *p;
   *p = q;
   return q;
}


void	lib_sets()
{
   var_set_free(&setP);
   if (fakeP != NULL)
   {
	lib_fakes(fakeP);
	fakeP = NULL;
   }
   var_set_free(&setPF);

   var_set_free(&setG);
   var_set_free(&setL);
   var_set_free(&setE);
   var_set_free(&setGE);
   set_free(&setCint);
   set_free(&setCfloat);
   set_free(&setCstring);
}

print_sets()
{
SIMBOLO *p;
int i;
char	*q;

  fprintf(stderr, "\nGlobals: ");
  for (p = var_set_get(&setG, i=0); p != NULL; p = var_set_get(&setG, ++i))
  {
	fprintf(stderr, "  %s", p->nome);
  }

  fprintf(stderr, "\nParameters: ");
  for (p = var_set_get(&setP, i=0); p != NULL; p = var_set_get(&setP, ++i))
  {
        fprintf(stderr, "  %s", p->nome);
  }

  fprintf(stderr, "\nParameters (no fake): ");
  for (p = var_set_get(&setPF, i=0); p != NULL; p = var_set_get(&setPF, ++i))
  {
        fprintf(stderr, "  %s", p->nome);
  }


  fprintf(stderr, "\nExterns: ");
  for (p = var_set_get(&setE, i=0); p != NULL; p = var_set_get(&setE, ++i))
  {
        fprintf(stderr, "  %s", p->nome);
  }

  fprintf(stderr, "\nGlobal + Externs: ");
  for (p = var_set_get(&setGE, i=0); p != NULL; p = var_set_get(&setGE, ++i))
  {
        fprintf(stderr, "  %s", p->nome);
  }

  fprintf(stderr, "\nLocals: ");
  for (p = var_set_get(&setL, i=0); p != NULL; p = var_set_get(&setL, ++i))
  {
        fprintf(stderr, "  %s", p->nome);
  }

  fprintf(stderr, "\nConsts int: ");
  for (q = set_get(&setCint, i=0); q != NULL; q = set_get(&setCint, ++i))
  {
        fprintf(stderr, "  %s", q);
  }
  fprintf(stderr, "\nConsts float: ");
  for (q = set_get(&setCfloat, i=0); q != NULL; q = set_get(&setCfloat, ++i))
  {
        fprintf(stderr, "  %s", q);
  }
  fprintf(stderr, "\nConsts string: ");
  for (q = set_get(&setCstring, i=0); q != NULL; q = set_get(&setCstring, ++i))
  {
        fprintf(stderr, "  %s", q);
  }

}




/************************************************************************
COMPUTE_SETP:
	This function builds the set of formal parameters of the current
function being analyzed.
************************************************************************/
compute_setP()
{
int	i;
SIMBOLO *q;

   for (i = 0, q = cur_par(0); q != NULL; q = cur_par(++i))
   {
        if (var_set_add(&setP, q) == ERRO)
		break;
        if (var_set_add(&setPF, q) == ERRO)
		break;
	if (e_pointer(q->tipo, q->ptar) || e_array(q->tipo, q->ptar))
	{
	   q = insert_fake(&fakeP, q);
	   if (q == NULL)
		break;
	   if (var_set_add(&setP, q) == ERRO)
		break;
	}
   }
}


/************************************************************************
COMPUTE_SETE:
        This function builds the set of global variables of the current
function being analyzed.
************************************************************************/
compute_setE()
{
int     i;
SIMBOLO *q;

   var_set_new(&setE);
   add_sim_set(&setE, tab_vars.vet[0]); 
   add_sim_set(&setGE, tab_vars.vet[0]); 
}


add_sim_set(VAR_SET *s, SIMBOLO *p)
{
   if (p == NULL)
	return;

   if ( ! e_funcao(p->tipo, p->ptar) && (! p->storage.f_extern) &&
	(var_set_inn(s, p->nome) < 0))
   {
	if (var_set_add(s, p) == ERRO)
	   return;
   }
   add_sim_set(s, p->esq);
   add_sim_set(s, p->dir);
}


/************************************************************************
COMPUTE_SETL:
        This function builds the set of local variables of the current
function being analyzed.
************************************************************************/
compute_setL()
{
extern	int	flg_dcl;
int	i;
SIMBOLO *q;

   if (! flg_dcl)
	return;
   var_set_new(&setL);
   var_set_new(&setLP);

   for (i = tab_vars.topo; i > 1; i--)
   {
	add_sim_set(&setL, tab_vars.vet[i]);
	add_sim_set(&setLP, tab_vars.vet[i]);
   }
   for (i = 0; i < var_set_card((&setPF)); i++)
   {
	q = var_set_get(&setPF,i);
	if (  var_set_inn(&setLP,q) < 0 )
	    var_set_add(&setLP, q);
   } 
}


compute_setGC()
{
int	i, j;
SIMBOLO *p;

       for (i = 0; i <= expre_pos.topo; i++)
       {            /* procura referencia do tipo desejado */

	   if (EH_CONST(expre_pos.no[i].tipo))
	   {
		expre_exec_tipo(&expre_pos, i, i, tipo, ptar);
		if (e_inteiro(tipo, ptar) && 
		    set_inn(&setCint, NOME((&expre_pos), i)) < 0)
		{
		   set_add(&setCint, NOME((&expre_pos), i));
		}
		else
		if (e_float(tipo, ptar) && 
                    set_inn(&setCfloat, NOME((&expre_pos), i)) < 0)
		{
                   set_add(&setCfloat, NOME((&expre_pos), i));
		}
		else
		if ( expre_pos.no[i].tipo == STRING && 
                    set_inn(&setCstring, NOME((&expre_pos), i)) < 0)
		{
		   set_add(&setCstring, NOME((&expre_pos), i));
		}
	   }
	   else
	   
	   if (expre_pos.no[i].tipo == VARIAVEL && 
	       e_ext(NOME((&expre_pos), i)))
	   {
		j = var_set_inn(&setE, NOME((&expre_pos), i));
		p = var_set_get(&setE, j);
		var_set_del(&setE, j);
		var_set_add(&setG, p);
	   }
	}
}


compute_setField(char *estruct, char *tipo, char *ptar, VAR_SET *sc)
{
int n;
SIMBOLO *p;

   var_set_new(sc);
   find_simbolo(estruct, &tab_tipos, &n, &p);
   if (p == NULL)
	return;
   add_sim_set(sc, p->campos);
}

