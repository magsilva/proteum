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
                                 U - S S W M . C
	Modulo que implementa o operadore de mutacao de armadilha em comandos case (UTRAP_ON_CASE).
	Tanto a implementacao quanto funcionamento sao semelhantes ao operador da Proteum1.4.1.

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


u_sswm(LI_SIMBOLO *scase, int cont)
{
char *p;

   Operador = U_SSWM;
   if (cont > 1)
   {
      	p = label_muta();
   	strcpy(bufmen, "goto ");
   	strcat(bufmen, p);
   	strcat(bufmen, "; ");
 
   	strcpy(&bufmen[40], p);
   	strcat(&bufmen[40], ": ; ");
    }

   if (g_tab_operador[Operador].percent != 0)
   { 
     if (cont > 1)
	add_descritor(scase->inicio, 0, scase->no, bufmen);

     add_descritor(scase->inicio+scase->comprimento, 0, 
				scase->no, " UTRAP_ON_CASE(); ");
     if (cont > 1)
         add_descritor(scase->inicio+scase->comprimento, 0,
				scase->no, &bufmen[40]);

     sorteia_grupoU(Operador, Seq[Operador]);
     Seq[Operador]++;
   }
}
