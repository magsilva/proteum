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

static	char	*str;

static	TIPO_NO	tipono;

extern	VAR_SET	setP;


void DirVarNeg(char *pre, char *pos)
{
int     i, j;
int     u;

   if (g_tab_operador[Operador].percent == 0)
        return;

   u = sym.no;

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

	   expre_exec_tipo(&expre_pos, expre_pos.no[i].final, i, tipo, ptar);
	   if ((Operador == DIR_VAR_ARITH_NEG && e_escalar(tipo, ptar) ) ||
                 e_inteiro(tipo,ptar) )

	   {
	     ins_expre_apos(&expre_pos, str, tipono, i);
             if (expre_valida(&expre_pos))
             {
           	monta_in_expre(&expre_pos, 0, expre_pos.topo); 
                sprintf(bufmen, "%s IF_MUTA(%s, ", pre, expre_pos.buf);
                expre_pos = ex_aux;
                monta_in_expre(&expre_pos, 0, expre_pos.topo);
		strcat(bufmen, expre_pos.buf);
		strcat(bufmen, ")");
		strcat(bufmen, pos);
           	add_descritor(sym.inicio, sym.comprimento, u, bufmen);
		sorteia_grupo1(Operador, ++(Seq[Operador]));

	     }
             expre_pos = ex_aux;
	   }
	}
   }

}



void DirVarArithNeg(char *pre, char *pos)
{
   Operador = DIR_VAR_ARITH_NEG;
   str = "-";
   tipono = OP_1_ARIT;
   DirVarNeg(pre, pos);
}


void DirVarLogNeg(char *pre, char *pos)
{
   Operador = DIR_VAR_LOG_NEG;
   str = "!";
   tipono = OP_1_LOG;
   DirVarNeg(pre, pos);
}

void DirVarBitNeg(char *pre, char *pos)
{
   Operador = DIR_VAR_BIT_NEG;
   str = "~";
   tipono = OP_1_BIT;
   DirVarNeg(pre, pos);
}


void IndVarNeg(char *pre, char *pos, int ret)
{
int     i, j, u;

   if (g_tab_operador[Operador].percent == 0)
        return;

   u = sym.no;
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
           expre_exec_tipo(&expre_pos, expre_pos.no[i].final, i, tipo, ptar);
           if ((Operador == IND_VAR_ARITH_NEG && e_escalar(tipo, ptar) ) ||
                 e_inteiro(tipo,ptar) )

           {
             ins_expre_apos(&expre_pos, str, tipono, i);
             if (expre_valida(&expre_pos))
             {
                monta_in_expre(&expre_pos, 0, expre_pos.topo);
                sprintf(bufmen, "%s IF_MUTA(%s, ", pre, expre_pos.buf);
                expre_pos = ex_aux;
                monta_in_expre(&expre_pos, 0, expre_pos.topo);
                strcat(bufmen, expre_pos.buf);
                strcat(bufmen, ")");
                strcat(bufmen, pos);
                add_descritor(sym.inicio, sym.comprimento, u, bufmen);
                sorteia_grupo1(Operador, ++(Seq[Operador]));

             }
             expre_pos = ex_aux;
           }
        }
   }

}



void IndVarArithNeg(char *pre, char *pos, int ret)
{
   Operador = IND_VAR_ARITH_NEG;
   str = "-";
   tipono = OP_1_ARIT;
   IndVarNeg(pre, pos, ret);
}


void IndVarLogNeg(char *pre, char *pos, int ret)
{
   Operador = IND_VAR_LOG_NEG;
   str = "!";
   tipono = OP_1_LOG;
   IndVarNeg(pre, pos, ret);
}

void IndVarBitNeg(char *pre, char *pos, int ret)
{
   Operador = IND_VAR_BIT_NEG;
   str = "~";
   tipono = OP_1_BIT;
   IndVarNeg(pre, pos, ret);
}



