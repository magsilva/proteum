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


#include <stdio.h>
#include <lib/gerais.h>
#include <polonesa/polonesa.h>
#include <string.h>
#include "li.h" 


extern	int	nivel;		/* nivel de aninhamento */

extern	int	flg_gera;	/* indica se deve gerar li */

extern	int	loc,
		contcalls,
		nnodes;


extern  long	comp_token,
		comp_ant,
		lin_token;

extern	FILE	*arqli,
		*arqfun;

extern	SIMBOLO *aloca_no(), *cria_arvore();


struct 	tableout ident;		/* token com nome de funcao definida */

extern	TABSIMBOLO	tab_vars, tab_tipos;

char ret[255];

char	nome_final[TAMLEX+1];

extern GOTOLABEL *lista_resul;


insert_simbol_function(def_simbolo)
SIMBOLO	*def_simbolo;
{
SIMBOLO	*s;
int	n;


   muda_ptar(def_simbolo->ptar);
   find_simbolo(def_simbolo->nome, &tab_vars, &n, &s);

   if (n == nivel)
        replace_simbolo(s, def_simbolo);
   else
     	ins_simbolo(def_simbolo, &tab_vars, 0, &s);

   if (def_simbolo->campos)
	ins_simbolo(def_simbolo->campos, &tab_vars, 1, &s);
}

li_begin_function(def_simbolo)
SIMBOLO	*def_simbolo;
{
   loc = 0;
   if (flg_gera)
   {
	fprintf(arqli, "@%s\t%d\t%ld\t%d\t%d\n", def_simbolo->nome, nnodes,
				ident.inicio, ident.comp, ident.linha);
        fprintf(arqfun, "%s\t%ld\t%d\t%d\n", def_simbolo->nome,
                                ident.inicio, ident.comp, ident.linha);
   }

   comp_token = comp_ant;
   strcpy(nome_final, def_simbolo->nome);
   grava_li(1, "$DCL");
   calcula_posicao();
}

li_fim_function()
{
   insert_list(nome_final, loc, contcalls, &lista_resul);
/*   loc = 0; */
}



insert_simbol_struct(ds)
SIMBOLO	*ds;
{
SIMBOLO	*p;
int	n;

   find_simbolo(ds->nome, &tab_tipos, &n, &p);
   if ( p == NULL)
   {
	ins_simbolo(ds, &tab_tipos, nivel, &p);
    }
}
  

insert_simbol_enum(ds)
SIMBOLO	*ds;
{
SIMBOLO	*p;
int	n;

   find_simbolo(ds->nome, &tab_tipos, &n, &p);
   if ( p == NULL)
   {
	ins_simbolo(ds, &tab_tipos, nivel, &p);
    }
}
 

insert_simbol_variable(ds)
SIMBOLO	*ds;
{
SIMBOLO	s;

   if (ds->nome[0] == '\0')
	return;

   muda_ptar(ds->ptar);
   if (ds->storage.f_typedef)
	ins_simbolo(ds, &tab_tipos, nivel, &s);
   else
	ins_simbolo(ds, &tab_vars, nivel, &s);
}



insert_simbol_parameter(ds)
SIMBOLO *ds;
{
SIMBOLO *s;
int	n;

   if (ds->nome[0] == '\0')
        return;

   muda_ptar(ds->ptar);
   find_simbolo(ds->nome, &tab_vars, &n, &s);

   if (n == nivel)
	replace_simbolo(s, ds); 
   else
        ins_simbolo(ds, &tab_vars, nivel, &s);
}



insert_field(ds, f)
SIMBOLO	*ds, *f;
{
SIMBOLO	s, *field;

   field = aloca_no();
   *field = *f;
   field->pai = ds;

   if (field->nome[0] == '\0')
	return;

   muda_ptar(field->ptar);
   if (ds->campos == NULL)
	ds->campos = cria_arvore(field);
   else
	ins_arvore(field, ds->campos);
}

insert_parameter(ds, f)
SIMBOLO *ds, *f;
{
SIMBOLO s, *field;

   field = aloca_no();
   *field = *f;
   field->pai = ds;
   field->npar = (ds->maxpar)++;

   muda_ptar(field->ptar);
   if (ds->campos == NULL)
        ds->campos = cria_arvore(field);
   else
        ins_arvore(field, ds->campos);
}



muda_ptar(s)
char	*s;
{
int	i = 0;

   ret[0] = 0;
   rec_ptar(s, &i);
   for (i = strlen(ret)-1 ; i >= 0; i--)
	*s++ = ret[i]; 
   *s = '\0';
/*   printf("\n%s", ret); */
}


rec_ptar(char *s, int *i)
{
int n = 0;


   while (s[*i] == '*')
   {
        (*i)++;
        n++;
   }
   rec_dir(s, i);
   while(n-- > 0)
       strcat(ret, "*");
}

rec_dir(char *s, int *i)
{
char    d[] = {0, 0};

   if (s[*i] == '\0')
        return;
   if (s[*i] == '{')
   {
        (*i)++;
        rec_ptar(s, i);
        (*i)++;
   }
   while (s[*i] == '(' || s[*i] == '[')
   {
        d[0] = s[*i];
        (*i)++;
        strcat(ret, d);
   }
}




type_of_assigment(s)
char	*s;
{

   if (strcmp(s, "=") == 0)
	  return OP_ASS_PLAIN;
 
   if (	strcmp(s, "+=") == 0 || strcmp(s, "-=") == 0 ||
	strcmp(s, "*=") == 0 || strcmp(s, "/=") == 0 ||
	strcmp(s, "%=") == 0 )
	  return OP_ASS_ARIT;

   if (	strcmp(s, "|=") == 0 || strcmp(s, "&=") == 0 ||
	strcmp(s, "^=") == 0)
	  return OP_ASS_BIT;

   if (strcmp(s, ">>=") == 0 || strcmp(s, "<<=") == 0)
	  return OP_ASS_SHIFT;

   return -1;
}


type_of_unary(s)
char	*s;
{

   if (strcmp(s, "+") == 0 || strcmp(s, "-") == 0)
	return OP_1_ARIT;

   if (strcmp(s, "&") == 0)
	return OP_1_REF;

   if (strcmp(s, "*") == 0)
	return OP_1_DEREF;

   if (strcmp(s, "~") == 0)
	return OP_1_BIT;

   if (strcmp(s, "!") == 0)
	return OP_1_LOG;

   return -1;
}



cast_operator(ds, s)
SIMBOLO *ds;
char	*s;
{

   tipo_to_str(ds, s);
   muda_ptar(ds->ptar);
   strcpy(&(s[strlen(s)]), ds->ptar);
/*   ptar_to_str(ds->ptar, &(s[strlen(s)])); */
}

ptar_to_str(p, s)
char    *s, *p;
{
char    *x;
static  char temp[200];

   x = &(p[strlen(p)-1]);
   if ( x >= p)
        strcat(s++, " ");
   while (x >= p)
   {
        if (*x ==  '(')
           strcat(s, "()");
        else
        if (*x ==  '[')
           strcat(s, "[]");
        else
        {
           sprintf(temp, "(*%s)", s);
           strcpy(s, temp);
        }
        x--;
   }
}


tipo_to_str(ds, s)
SIMBOLO *ds;
char *s;
{
int	i;

   s[0] = '\0';

   if (ds->qualifier.f_const)
   	strcat(s, "const ");
   if (ds->qualifier.f_volatile)
	strcat(s, "volatile ");

/*   if (ds->storage.f_auto)
	strcat(s, "auto ");
   if (ds->storage.f_static)
	strcat(s, "static ");
   if (ds->storage.f_extern)
	strcat(s, "extern ");
   if (ds->storage.f_typedef)
	strcat(s, "typedef ");
   if (ds->storage.f_register)
	strcat(s, "register "); */

   if (ds->specifier.f_signed)
	strcat(s, "signed ");
   if (ds->specifier.f_unsigned)
	strcat(s, "unsigned ");
   if (ds->specifier.f_long)
	strcat(s, "long ");
   if (ds->specifier.f_short)
	strcat(s, "short ");
   if (ds->specifier.f_void)
	strcat(s, "void ");
   if (ds->specifier.f_char)
	strcat(s, "char ");
   if (ds->specifier.f_int)
	strcat(s, "int ");
   if (ds->specifier.f_float)
	strcat(s, "float ");
   if (ds->specifier.f_double)
	strcat(s, "double ");

   if (ds->specifier.f_other || ds->specifier.f_struct || ds->specifier.f_enum)
	strcat(s, ds->tipo);

   if (s[i = strlen(s)-1] == ' ')
	s[i] = '\0';
}

