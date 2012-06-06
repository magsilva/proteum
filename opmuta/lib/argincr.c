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

extern	OSET	set_aux;

extern	char	tipo[], ptar[];

extern	int	Seq[];

extern  EXPRE_POS       expre_pos,
			ex_aux;

static	Operador = ARG_INCR_DECR;



OSET *succ_pred_set()
{
static OSET    pc;

   set_new_inic(&pc, "SUCC\0PRED\0");
   return &pc;
}



void ArgIncrDecr(char *pre, char *pos)
{
char    *q, *get_rand();
int     i, j, n, max, nseq, seq;
int     u, k, pinic, pfim, t;
OSET	*r = &set_aux, *p;

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

           pinic = get_nth_param(&expre_pos, i, k=1);
           pfim = pinic > 0 ? expre_pos.no[pinic].final : -1;
           while ( pinic > 0 )
           {
             expre_exec_tipo(&expre_pos, pfim, pinic, tipo, ptar);
	     if (! e_struct(tipo, ptar))
	     {
                p = succ_pred_set(tipo, ptar, &t);
                set_dup(p, r);
                nseq = set_card(p);
                for (n = max,q = get_rand(p, Operador);
                     n != 0 && q != NULL; q = get_rand(p, Operador), n--)
                {
                   seq = set_inn(r, q);
		   ins_expre_apos(&expre_pos, ")", OP_POS2, pinic);
		   ins_expre_apos(&expre_pos, q, VARIAVEL, pfim-1);

                   monta_in_expre(&expre_pos, 0, expre_pos.topo);
		   sprintf(bufmen, "%s%s%s", pre, expre_pos.buf, pos);

                   add_descritor(sym.inicio, sym.comprimento, u, bufmen);
                   sorteia_grupo2(func_name, Operador, Seq[Operador]+seq);

                   expre_pos = ex_aux;
                 }
		set_free(p); set_free(r);
                Seq[Operador] += nseq;
	     }
             pinic = get_nth_param(&expre_pos, i, ++k);
             pfim = pinic > 0 ? expre_pos.no[pinic].final: -1;
           }
      }
   }

}


