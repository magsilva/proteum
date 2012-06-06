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
#include        <li/lib/li.h>
#include        "opmuta.h"

extern int     Seq[];

extern	OPERADOR_MUTACAO g_tab_operador[]; 

extern  LI_SIMBOLO      sym;    /* simbolo 'lido' */

extern  char    bufmen[];       /* buffer auxiliar global */

extern  EXPRE_POS       expre_pos,
                        ex_aux;

static	Operador;

int	nlabel;

LI_SIMBOLO	lvet[100];


init_sglr()
{
   lvet[nlabel++] = sym;
}


u_sglr()
{
int     i, u, t, max, seq, nseq;
char    *q, *get_rand();
OSET    *p, *monta_pset();

   Operador = U_SGLR;
   if (g_tab_operador[Operador].percent == 0)
        return;
   max = g_tab_operador[Operador].maximum;
   if (max == 0) max--;

   u = sym.no;
   ex_aux = expre_pos;
   p = monta_pset(nlabel);
   nseq = nlabel;

   for (q = get_rand(p, Operador); 
	max != 0 && q != NULL; q = get_rand(p, Operador), max--)
   {
	seq = i = atoi(q);
	carrega_fonte(bufmen, &lvet[i]);
	bufmen[strlen(bufmen)-1] = '\0';
	if (strcmp(bufmen, sym.simbolo) == 0)
	{
	   max++;
	   continue;
	}
        strcat(bufmen, ";");
        add_descritor(sym.inicio, sym.comprimento, u, bufmen);
        sorteia_grupoU(Operador, Seq[Operador]+seq);
   }
   set_free(p);
   Seq[Operador] += nseq;
}

