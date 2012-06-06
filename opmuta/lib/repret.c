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

int	nret;

LI_SIMBOLO	vet[100];


init_RepRetStat()
{
int	i;

   ex_aux = expre_pos;
   if (nret == 0)
   {
	vet[nret] = sym;
	vet[nret++].linha = conta_local(&ex_aux);
   }
   else
   {
	for (i = 0; i < nret; i++)
        {
            carrega_fonte(bufmen, &vet[i]);
            gera_expre(&bufmen[6]);
            if (compara_expre(&ex_aux, &expre_pos))
                 break;
        }
        if (i >= nret)
        {
             vet[nret] = sym;
             vet[nret++].linha = conta_local(&ex_aux);
        }
    }
    expre_pos = ex_aux;
}


RepRetStat()
{
int     i, u, t, max, seq, nseq;
char    *q, *get_rand();
OSET    *r, *p, *monta_pset();

   Operador = REP_RET_STAT;
   if (g_tab_operador[Operador].percent == 0)
        return;
   max = g_tab_operador[Operador].maximum;
   if (max == 0) max--;

   u = sym.no;
   ex_aux = expre_pos;
   p = monta_pset(nret);
   nseq = nret;

   for (q = get_rand(p, Operador); 
	max != 0 && q != NULL; q = get_rand(p, Operador), max--)
   {
	seq = i = atoi(q);
	carrega_fonte(bufmen, &vet[i]);
	gera_expre(&bufmen[6]);
	if (vet[i].linha != conta_local(&expre_pos) ||
	    compara_expre(&ex_aux,&expre_pos))
	{
		max++;
		continue;
	}

   	sprintf(bufmen,"IF_MUTA2(");
        t = strlen(bufmen);
        carrega_fonte(&bufmen[t], &vet[i]);
        strcat(bufmen, ",");
        t = strlen(bufmen);
        carrega_fonte(&bufmen[t], &sym);
        strcat(bufmen, ")");
        add_descritor(sym.inicio, sym.comprimento, u, bufmen);
        sorteia_grupo1(Operador, Seq[Operador]+seq);
   }
   set_free(p);
   Seq[Operador] += nseq;
}


conta_local(e)
EXPRE_POS       *e;
{
int     i, j;

   for (i= j = 0; i <= e->topo; i++)
   {
        if (e->no[i].tipo == VARIAVEL)
              j += e_local(NOME(e,i));
   }
   return j;
}

