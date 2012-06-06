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


/****************************** U-O L B N G **********************************/

#include	<lib/gerais.h>
#include	<muta/lib/muta.h>
#include	<polonesa/polonesa.h>
#include	<li/lib/li.h>
#include	"opmuta.h"



extern	OPERADOR_MUTACAO g_tab_operador[]; 

extern  int     pt;             /* apontador p/ prog LI */

extern  LI_SIMBOLO      sym;    /* simbolo 'lido' */

extern  char    bufmen[];       /* buffer auxiliar global */

extern	char	func_name[];

extern  char    tipo[], ptar[];

extern	int	Seq[];

extern	OSET	set_aux;

extern  EXPRE_POS       expre_pos,
			ex_aux;

static Operador;



u_olng(char *pre, char *pos)
{
 OSET r;
 Operador = U_OLNG;
 set_new_inic(&r, "!\0");
 olbng(pre, pos, OP_2_LOG, OP_1_LOG, &r);
 set_free(&r);
}


u_obng(char *pre, char *pos)
{
 OSET r;
 Operador = U_OBNG;
 set_new_inic(&r, "~\0");
 olbng(pre, pos, OP_2_BIT, OP_1_BIT, &r);
 set_free(&r);
}


olbng(char *pre, char *pos, TIPO_NO troca, TIPO_NO por, OSET *op)
{
char *x, *q, *get_rand(), *set_prim();
int i, j, n, max, seq, nseq;
int u, k, t;
OSET *r, *p = &set_aux;
int param1, param2;



if (g_tab_operador[Operador].percent==0) return;

 r = op;
 u = sym.no;
 max = g_tab_operador[Operador].maximum;
 if (max==0) max--;


 if (expre_pos.topo >= 0)
 {
     expre_completa(&expre_pos);
     ex_aux = expre_pos;


     for (i = 0; i <= ex_aux.topo; i++)
     {		/* procura operador do tipo desejado */
           if (ex_aux.no[i].tipo != troca ) continue;

           set_dup(r, p);
           nseq = set_card(p);
 

	   param1 = i-1;
 	   param2 = expre_pos.no[param1].final - 1;
           nseq = 3 * set_card(p);
           for (n = max,q = get_rand(p, Operador);
                     n != 0 && q != NULL; q = get_rand(p, Operador), n--)
           {
                seq = set_inn(r, q);
		ins_expre_apos(&expre_pos, q, por, param1);

                monta_in_expre(&expre_pos, 0, expre_pos.topo);
                sprintf(bufmen, "%s %s ", pre, expre_pos.buf);
                strcat(bufmen, pos);
                add_descritor(sym.inicio, sym.comprimento, u, bufmen);
                sorteia_grupoU(Operador, Seq[Operador]+seq);

                expre_pos = ex_aux;

	 	seq++;
		ins_expre_apos(&expre_pos, q, por, param2);

                monta_in_expre(&expre_pos, 0, expre_pos.topo);
                sprintf(bufmen, "%s %s ", pre, expre_pos.buf);
                strcat(bufmen, pos);
                add_descritor(sym.inicio, sym.comprimento, u, bufmen);
                sorteia_grupoU(Operador, Seq[Operador]+seq);

                expre_pos = ex_aux;

		seq++;
		ins_expre_apos(&expre_pos, q, por, i);

                monta_in_expre(&expre_pos, 0, expre_pos.topo);
                sprintf(bufmen, "%s %s ", pre, expre_pos.buf);
                strcat(bufmen, pos);
                add_descritor(sym.inicio, sym.comprimento, u, bufmen);
                sorteia_grupoU(Operador, Seq[Operador]+seq);

                expre_pos = ex_aux;

           }
           set_free(p);
           Seq[Operador] += nseq;
        }
  }


}
