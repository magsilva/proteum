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


/******************************************************************************
				    U-OIPM
                  Indirection Operator Precedence Mutation

       Implementa os operadores de mutacao de precedencia de indirecao

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

extern int Seq[];

extern	EXPRE_POS	expre_pos, ex_aux;

EXPRE_POS	ex_aux2;

extern	TABSIMBOLO	tab_vars, tab_tipos;

static Operador;


u_oipm(char *pre, char *pos)
{
 int i, u, n, max, seq, nseq;
 OSET r; 
 struct no  k ;

 Operador = U_OIPM;

 if (g_tab_operador[Operador].percent == 0) return;

 set_new_inic(&r, "++\0--\0");

 u=sym.no;

 max = g_tab_operador[Operador].maximum; 
 if (max ==0) max--;
 if (expre_pos.topo >= 0)
  {
    expre_completa(&expre_pos);

    ex_aux = expre_pos;

    for (i = 0; i <= expre_pos.topo; i++) 
        /* procura referencia do tipo desejado */
    {
      if ( expre_pos.no[i].tipo != OP_POS1_ARIT  ||  
          set_inn(&r, NOME((&expre_pos), i)) < 0)
                    continue;

      nseq = 1;
      seq = 0; 
      n = max;
      while (expre_pos.no[i+1].tipo == OP_1_DEREF && n != 0)
      {
        k = expre_pos.no[i];
      	expre_pos.no[i] = expre_pos.no[i+1];
      	expre_pos.no[i+1] = k;
		

      	if (expre_valida(&expre_pos))
      	{
	    ex_aux2 = expre_pos;
       	    monta_in_expre(&expre_pos, 0, expre_pos.topo); /* monta infixa */
            sprintf(bufmen, "%s %s ",pre, expre_pos.buf);
            strcat(bufmen, pos);
            add_descritor(sym.inicio, sym.comprimento, u, bufmen);	 
            sorteia_grupoU(Operador, Seq[Operador] + seq);
	    seq++; nseq++;
	    expre_pos = ex_aux2;

	    if (--n == 0)
		break;

	    expre_pos.no[i+1].tipo = OP_1_ARIT;
       	    monta_in_expre(&expre_pos, 0, expre_pos.topo); /* monta infixa */
            sprintf(bufmen, "%s %s ",pre, expre_pos.buf);
            strcat(bufmen, pos);
            add_descritor(sym.inicio, sym.comprimento, u, bufmen);	 
            sorteia_grupoU(Operador, Seq[Operador] + seq);
	    seq++; nseq++;
	    expre_pos = ex_aux2;
	    n--;
         }
	i++;
      }

      expre_pos = ex_aux;
      Seq[Operador] += nseq;
     }
      
   }
}











