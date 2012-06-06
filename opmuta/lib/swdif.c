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


extern	OPERADOR_MUTACAO g_tab_operador[]; 

extern  int     pt;             /* apontador p/ prog LI */

extern  LI_SIMBOLO      sym;    /* simbolo 'lido' */

extern  char    bufmen[];       /* buffer auxiliar global */

extern	char	func_name[];

extern  char    tipo[], ptar[],
		tipo1[],ptar1[];

extern	int	Seq[];

extern  EXPRE_POS       expre_pos,
			ex_aux;

static	Operador = SW_DIF_ARG;

short int	gone[1024],
		ngone;

char	aux_buf[1024];

void SwitchDifArg(char *pre, char *pos)
{
char    *q, *get_rand();
int     i, j, n, max, seq, nseq, pmax, l;
int     u, k, pinic, pfim, pinic1, pfim1, t;
OSET	*r, *p, *monta_pset();

   if (g_tab_operador[Operador].percent == 0)
        return;

   u = sym.no;
   max = g_tab_operador[Operador].maximum;
   if (max == 0)
	max--;

   if (expre_pos.topo >= 0)
   {

       expre_completa(&expre_pos);     /* completa expressao */
       ex_aux = expre_pos;

       for (i = 0; i <= expre_pos.topo; i++)
       {            /* procura referencia do tipo desejado */

           if (expre_pos.no[i].tipo != OP_POS2 ||
               strcmp(NOME((&expre_pos), i), ")") != 0)
                   continue;

           j = expre_pos.no[i].final;
           if (expre_pos.no[j].tipo != VARIAVEL)
               continue;

           strcpy(func_name, NOME((&expre_pos), j));
           if (get_tipo_varr(func_name, tipo, ptar) != ERRO)
                continue;
	   pmax = get_max_param(&expre_pos, i);
	   memset(gone, 0, sizeof(gone));
	   ngone = 0;

	   for (k = 0; k < pmax; k++)
	   {
	        nseq = pmax;
		p = monta_pset(pmax);
		pinic = get_nth_param(&expre_pos, i, k+1);
		pfim = expre_pos.no[pinic].final;
		expre_exec_tipo(&expre_pos, pfim, pinic, tipo, ptar);

	        for (n = max, q = get_rand(p, Operador);
                     n != 0 && q != NULL; q = get_rand(p, Operador), n--)
		{
		   l = atoi(q);
		   if (l == k)
		   {
			n++;
			continue;
		   }
		   if (already_gone(k, l, pmax))
			continue;
		   pinic1 = get_nth_param(&expre_pos, i, l+1);
		   pfim1 = expre_pos.no[pinic1].final;
		   expre_exec_tipo(&expre_pos, pfim1, pinic1, tipo1, ptar1);
		   if ( compara_tipos(tipo, ptar, tipo1, ptar1) )
		   {
			n++;
			continue;
		   }
		   expre_change_places(&expre_pos, pinic, pinic1);
		   gone[ngone++] = k * pmax + l;
		   seq = l;
                   monta_in_expre(&expre_pos, 0, expre_pos.topo);
                   sprintf(bufmen, "%s%s%s", pre, expre_pos.buf, pos);
                   add_descritor(sym.inicio, sym.comprimento, u, bufmen);
                   sorteia_grupo2(func_name, Operador, Seq[Operador]+seq);

                   expre_pos = ex_aux;
		}
		set_free(p);
                Seq[Operador] += nseq;
	   }

      }
   }

}


