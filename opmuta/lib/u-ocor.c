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


/************************************************************************
				U - O C O R . C
	Implementa operador de mutacao sobre operados cast

***************************************************************************/

#include	<lib/gerais.h>
#include	<muta/lib/muta.h>
#include	<polonesa/polonesa.h>
#include	<li/lib/li.h>
#include	"opmuta.h"


extern OPERADOR_MUTACAO g_tab_operador[];

extern	int	pt;		/* apontador p/ prog LI */

extern	LI_SIMBOLO	sym;	/* simbolo 'lido' */

extern	char	bufmen[];	/* buffer auxiliar global */

extern	char	func_name[];

extern  char tipo[], ptar[];

extern int Seq[];

extern OSET set_aux;

extern	EXPRE_POS	expre_pos, ex_aux;

extern	TABSIMBOLO	tab_vars, tab_tipos;

static Operador;


static	TIPO_NO	 troca, por;



u_ocor(char *pre, char *pos)
{

 char *q, *get_rand();
 int i, u, n, j, max, seq, nseq;
 OSET r, x, *p = &set_aux; 
 
 seq = 0;
 troca = OP_1_CAST;
 por = OP_1_CAST;

 Operador = U_OCOR;
 if (g_tab_operador[Operador].percent==0) return;

  set_new_inic(&r,"char\0unsigned char\0int\0float\0unsigned\0unsigned int\0short\0short int\0long\0long int\0unsigned long\0unsigned long int\0double\0long float\0");

   set_new_inic(&x, "char\0unsigned char\0int\0float\0unsigned\0short\0long\0unsigned long\0double\0");

 
 u = sym.no;
 max = g_tab_operador[Operador].maximum; 
 if (max ==0) max--;
 

   if (expre_pos.topo >= 0)
     {
       ex_aux = expre_pos;

       for (i = 0; i <= ex_aux.topo; i++)
         {		/* procura operador do tipo desejado */
	   if (ex_aux.no[i].tipo != troca ||
		 set_inn(&r, NOME((&ex_aux), i)) < 0) 
			continue;

 
           set_dup(&x, p);
           nseq = set_card(p);
           

           for (n = max, q = get_rand(p, Operador); 
		q != NULL && n != 0;	q = get_rand(p, Operador),n--)
            {	/* troca pelos outros operadores */

               	if (expre_pos.no[i].tipo == por && 
                    strcmp(NOME((&expre_pos), i), q) == 0)
		{
			n++;
		 	continue;
		}
		seq = set_inn(&x, q);

		expre_pos.no[i].tipo = por;
		expre_pos.no[i].nome = expre_pos.livre;
		strcpy(&(expre_pos.buf[expre_pos.livre]), q);
		expre_pos.livre += strlen(q)+1;
 
		if (expre_valida(&expre_pos))
		{
		  monta_in_expre(&expre_pos, 0, expre_pos.topo); /* monta infixa */
		  strcpy(bufmen, pre);
		  strcat(bufmen, expre_pos.buf);
                  sprintf(bufmen, "%s %s ",pre, expre_pos.buf);
		  strcat(bufmen, pos);
		  add_descritor(sym.inicio, sym.comprimento, u, bufmen);
  		  sorteia_grupoU(Operador, Seq[Operador] + seq);
	         }
		 else
		   n++;
		 expre_pos = ex_aux;
            }
	    set_free(p); 
            Seq[Operador] += nseq;
           }
	set_free(&r); set_free(&x);

      }
}
