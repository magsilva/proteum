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


/*****************************************************************************
                                U-SWDD e U-SDWD

Operadores que implementam respectivamente a troca de comando while por do-while, e do-while por while.

******************************************************************************/

#include	<lib/gerais.h>
#include	<muta/lib/muta.h>
#include	"opmuta.h"


extern 	int	pt;		/* apontador p/ prog LI */

extern	LI_SIMBOLO	sym;	/* simbolo 'lido' */

extern	int	Seq[];

static	Operador;

extern	OPERADOR_MUTACAO g_tab_operador[]; 

extern  char    bufmen[];       /* buffer auxiliar global */

u_sdwd(LI_SIMBOLO *sdo , LI_SIMBOLO *scoman, 
	LI_SIMBOLO *swhile, LI_SIMBOLO *scond)
{
int i;

    Operador = U_SDWD;
   if (g_tab_operador[Operador].percent == 0)
        return;
   add_descritor(sdo->inicio, sdo->comprimento, sdo->no, "while");
   carrega_fonte(bufmen, scond);	/* carrega fonte da condicao */
   i = strlen(bufmen);		/* tira a ; do final */
   while (i >= 0 && bufmen[i] != ';')
	i--;
   if (i >=0)
	     bufmen[i] = ' ';

   add_descritor(scoman->inicio, 0, scoman->no, bufmen);
   add_descritor(swhile->inicio, swhile->comprimento, swhile->no, "");
   add_descritor(scond->inicio, scond->comprimento, scond->no, "");
    sorteia_grupoU(Operador, Seq[Operador]);
    Seq[Operador]++;
} 


u_swdd(LI_SIMBOLO *swhile , LI_SIMBOLO *scond, LI_SIMBOLO *scoman)
{

   Operador = U_SWDD;
   if (g_tab_operador[Operador].percent == 0)
        return;
   add_descritor(swhile->inicio, swhile->comprimento, swhile->no, "do");
   add_descritor(scond->inicio, scond->comprimento, scond->no, "");
   add_descritor(scoman->inicio, 0, scoman->no, "while");
   carrega_fonte(bufmen, scond);
   strcat(bufmen, ";\n");
   add_descritor(scoman->inicio, 0, scoman->no, bufmen);
    sorteia_grupoU(Operador, Seq[Operador]);
    Seq[Operador]++;
} 
