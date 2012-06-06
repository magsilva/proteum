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


#include            <lib/gerais.h>
#include            <btree/btree.h>
 
#include            "muta.h"


int add_descr_func(h, inic, name, param, nome, dcl, cab, fim, e_void)
HAND_MUTANTE	*h;
long	inic, fim;
int	nome, dcl, cab, e_void;
char    name[], param[];
{
long l;
int  s;
DESCR_FUNCAO df;

   df.inic_func = inic;
   df.inic_nome = nome;
   df.tam_dcl = dcl;
   df.tam_cab = cab;
   df.tam_func = fim;
   df.e_void = e_void;
   strcpy(df.name, name);
   strcpy(df.param, param);

   if (BTREE_insert_data(h->btfun, &df, &df, sizeof(df)) < 0)
   {
      if (btree_errno != DUP_KEY)
      {
      l1:
	 msg("Error creating function descriptor");
	 return ERRO;
      }

      if (BTREE_rewrite_data(h->btfun, &df, &df) < 0)
	goto l1;

   }

   return OK;
}


int get_descr_func_l(h, inic, name, param, nome, dcl, cab, fim, e_void)
HAND_MUTANTE	*h;
long	inic, *fim; /* fim era inteiro foi mudado para long */
int	*nome, *dcl, *cab, *e_void;
char    name[], param[];
{
DESCR_FUNCAO df;
int s;

   if (BTREE_get_data(h->btfun, &inic, &df, &s) < 0)
   {
       msg("Invalid function descriptor");
       return ERRO;
   }
 
   *nome = df.inic_nome;
   *dcl = df.tam_dcl;
   *cab = df.tam_cab;
   *fim = df.tam_func;
   *e_void = df.e_void;
   if (name != NULL)
	strcpy(name, df.name);
   if (param != NULL)
        strcpy(param, df.param);
   return OK;
}



int cmp_func(x, y)
void *x, *y;
{
long l, m;

   l = * (long *) x;
   m = * (long *) y;

   return ((l < m) ? -1: ((l > m)? 1 : 0));
}

void print_func()
{
}





