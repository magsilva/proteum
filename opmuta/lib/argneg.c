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

extern  EXPRE_POS       expre_pos,
			ex_aux;

static	int	Operador;

static	char	*str;

static	TIPO_NO	tipono;



void ArgNeg(char *pre, char *pos)
{
int     i, j, n;
int     u, k, pinic, pfim, t;

   if (g_tab_operador[Operador].percent == 0)
        return;

   u = sym.no;

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

	     if ((Operador == ARG_ARITH_NEG && e_escalar(tipo, ptar) ) ||
		 e_inteiro(tipo,ptar) )
	     {
		   ins_expre_apos(&expre_pos, str, tipono, pinic);
                   monta_in_expre(&expre_pos, 0, expre_pos.topo);
                   sprintf(bufmen, "%s%s%s", pre, expre_pos.buf, pos);
           	   add_descritor(sym.inicio, sym.comprimento, u, bufmen);
		   sorteia_grupo2(func_name, Operador, ++(Seq[Operador]));

           	   expre_pos = ex_aux;
	      }
              pinic = get_nth_param(&expre_pos, i, ++k);
              pfim = pinic > 0 ? expre_pos.no[pinic].final: -1;
	    }
      }
   }

}


void ArgArithNeg(char *pre, char *pos)
{
   Operador = ARG_ARITH_NEG;
   str = "-";
   tipono = OP_1_ARIT;
   ArgNeg(pre, pos);
}


void ArgLogNeg(char *pre, char *pos)
{
   Operador = ARG_LOG_NEG;
   str = "!";
   tipono = OP_1_LOG;
   ArgNeg(pre, pos);
}

void ArgBitNeg(char *pre, char *pos)
{
   Operador = ARG_BIT_NEG;
   str = "~";
   tipono = OP_1_BIT;
   ArgNeg(pre, pos);
}


