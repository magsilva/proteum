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

extern  char    tipo[], ptar[];

extern	int	Seq[];

extern	OSET	set_aux;

extern  EXPRE_POS       expre_pos,
			ex_aux;

static	Operador;

extern	VAR_SET	setP;


void DirVarIncrDecr(char *pre, char *pos)
{
char	*q, *get_rand();
int     i, j, n, max, seq, nseq;
int     u, k, t;
OSET	*p = &set_aux, r;

   Operador = DIR_VAR_INCR_DECR;
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

           j = root_of_ref(&expre_pos, i);
           if ( j < 0 || expre_pos.no[j].tipo != VARIAVEL || 
		! e_interface(NOME((&expre_pos), j)) )
               continue;

	   set_new_inic(&r, "++\0--\0");
	   set_dup(&r, p);
	   nseq = set_card(p);		
	   for (n = max,q = get_rand(p, Operador); 
		     n != 0 && q != NULL; q = get_rand(p, Operador), n--)
	   {
	   	seq = set_inn(&r, q);
		ins_expre_apos(&expre_pos, q, OP_1_ARIT, i);
                if (! expre_valida(&expre_pos))
                {
                   nseq = 0;
                   expre_pos = ex_aux;
                   break;
                }

           	monta_in_expre(&expre_pos, 0, expre_pos.topo); 
                sprintf(bufmen, "%s IF_MUTA(%s, ", pre, expre_pos.buf);
                expre_pos = ex_aux;
                monta_in_expre(&expre_pos, 0, expre_pos.topo);
		strcat(bufmen, expre_pos.buf);
		strcat(bufmen, ")");
		strcat(bufmen, pos);
           	add_descritor(sym.inicio, sym.comprimento, u, bufmen);
		sorteia_grupo1(Operador, Seq[Operador]+seq);

           	expre_pos = ex_aux;
	   }
	   set_free(p); set_free(&r);
	   Seq[Operador] += nseq;
	}
   }

}



void IndVarIncrDecr(char *pre, char *pos, int ret)
{
char    *q, *get_rand();
int     i, j, n, max, seq, nseq;
int     u, k, t;
OSET    *p = &set_aux, r;

   Operador = IND_VAR_INCR_DECR;
   if (g_tab_operador[Operador].percent == 0)
        return;

   u = sym.no;
   max = g_tab_operador[Operador].maximum;
   if (max == 0)
        max--;
   for (i = 0; (!ret) && i <= expre_pos.topo; i++)
   {
        if (expre_pos.no[i].tipo == VARIAVEL &&
                e_interface(NOME((&expre_pos), i)))
                break;
   }
   if (i > expre_pos.topo)
        return;

   if (expre_pos.topo >= 0)
   {

       expre_completa(&expre_pos);     /* completa expressao */
       ex_aux = expre_pos;

       for (i = 0; i <= expre_pos.topo; i++)
       {            /* procura referencia do tipo desejado */

           j = root_of_ref(&expre_pos, i);
           if ( j < 0 || ( ! EH_CONST(expre_pos.no[j].tipo) &&
                (expre_pos.no[j].tipo != VARIAVEL ||
                 e_interface(NOME((&expre_pos), j)))) )
               continue;

	   if (EH_CONST(expre_pos.no[j].tipo))
	   {
		set_new_inic(&r, "SUCC\0PRED\0");
	   }
	   else
	   {
           	set_new_inic(&r, "++\0--\0");
	   }
           set_dup(&r, p);
           nseq = set_card(p);
           for (n = max,q = get_rand(p, Operador);
                     n != 0 && q != NULL; q = get_rand(p, Operador), n--)
           {
                seq = set_inn(&r, q);
		if (EH_CONST(expre_pos.no[j].tipo))
		{
		   ins_expre_apos(&expre_pos, ")", OP_POS2, j);
                   ins_expre_apos(&expre_pos, q, VARIAVEL, j-1);
		}
		else
		{
                   ins_expre_apos(&expre_pos, q, OP_1_ARIT, i);
		}

                if (! expre_valida(&expre_pos))
                {
                   nseq = 0;
                   expre_pos = ex_aux;
                   break;
                }

                monta_in_expre(&expre_pos, 0, expre_pos.topo);
                sprintf(bufmen, "%s IF_MUTA(%s, ", pre, expre_pos.buf);
                expre_pos = ex_aux;
                monta_in_expre(&expre_pos, 0, expre_pos.topo);
                strcat(bufmen, expre_pos.buf);
                strcat(bufmen, ")");
                strcat(bufmen, pos);
                add_descritor(sym.inicio, sym.comprimento, u, bufmen);
                sorteia_grupo1(Operador, Seq[Operador]+seq);

                expre_pos = ex_aux;
           }
           set_free(p); set_free(&r);
           Seq[Operador] += nseq;
        }
   }

}

