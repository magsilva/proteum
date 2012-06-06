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


/*******************************************************************************
                                 U - O I D O . C 
*******************************************************************************/



#include	<lib/gerais.h>
#include	<muta/lib/muta.h>
#include	<polonesa/polonesa.h>
#include	<li/lib/li.h>
#include	"opmuta.h"


extern OPERADOR_MUTACAO g_tab_operador[];

extern	int	pt;		/* apontador p/ prog LI */

extern	LI_SIMBOLO	sym;	/* simbolo 'lido' */

extern	char	bufmen[];	/* buffer auxiliar global */

extern char func_name[];

extern char tipo[], ptar[];

extern int Seq[];

extern OSET set_aux;

extern	EXPRE_POS	expre_pos, ex_aux;

extern	TABSIMBOLO	tab_vars, tab_tipos;
 
static Operador;




u_oido(char *pre, char *pos)
{
 char *q, *get_rand();
 int i, u, n, max, seq, nseq;
 OSET r, *p = &set_aux; 
 int k, k1;

 Operador = U_OIDO;
 if (g_tab_operador[Operador].percent == 0) return;

  
 set_new_inic(&r,"--\0++\0");
 u=sym.no;
 max = g_tab_operador[Operador].maximum; 
 if (max ==0) max--;
 expre_completa(&expre_pos);

  if (expre_pos.topo >=0)
     {
       ex_aux = expre_pos;

       for(i = 0; i<= ex_aux.topo;i++)
 	  {
		k = ex_aux.no[i].tipo;
		if ((k != OP_1_ARIT && k != OP_POS1_ARIT ) ||
		     set_inn(&r, NOME((&ex_aux), i)) < 0)
			     continue;
		k1 = (k == OP_1_ARIT)? OP_POS1_ARIT: OP_1_ARIT;

		nseq = 0;
  		if (i < ex_aux.topo)
		{		
		   seq = 0;  /*indica o numero de sequencia do mutante gerado*/
		   nseq = 1;  /*indico a quantidade de mutantes gerados*/
		   expre_pos.no[i].tipo = k1; /*troca tipo do operador */
                   monta_in_expre(&expre_pos, 0, expre_pos.topo);
                   sprintf(bufmen, "%s %s ", pre, expre_pos.buf);
                   strcat(bufmen, pos);
                   add_descritor(sym.inicio, sym.comprimento, u, bufmen);
                   sorteia_grupoU(Operador, Seq[Operador]+seq);

                   expre_pos = ex_aux;
		  }
               set_dup(&r, p);
               for (n = max, q = get_rand(p, Operador);
                     n != 0 && q != NULL; q = get_rand(p, Operador), n--)
               {
                  seq = nseq + set_inn(&r, q);
		  if (strcmp(NOME((&expre_pos), i), q) == 0)
                  {
                     n++;
                    continue;
                  }

		  expre_pos.no[i].nome = expre_pos.livre;
		  strcpy(&(expre_pos.buf[expre_pos.livre]), q);
		  expre_pos.livre += strlen(q)+1;

                  monta_in_expre(&expre_pos, 0, expre_pos.topo);
                  sprintf(bufmen, "%s %s ", pre, expre_pos.buf);
                  strcat(bufmen, pos);
                  add_descritor(sym.inicio, sym.comprimento, u, bufmen);
                  sorteia_grupoU(Operador, Seq[Operador]+seq);

                  expre_pos = ex_aux;

                }
                nseq += set_card((&r));
                set_free(p);
                Seq[Operador] += nseq;  /*incrementa o numero total de mutantes gerados para determinado Operador*/
	}
    }
    set_free(&r);
}
	







 	


