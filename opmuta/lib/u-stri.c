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
                                 U - S T R I . C
	Modulo que implementa o operadore de mutacao de armadilha em comandos if (UTRAP_ON_TRUE e UTRAP_ON_FALSE).
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


u_stri( LI_SIMBOLO *scond)
{
    
   Operador = U_STRI;
   if (g_tab_operador[Operador].percent != 0)
   {
     add_descritor(scond->inicio, 0, scond->no, "(UTRAP_ON_TRUE(");
     add_descritor(scond->inicio+ scond->comprimento, 0, scond->no, "))"  );
     sorteia_grupoU(Operador, Seq[Operador]);
     Seq[Operador]++;

     add_descritor(scond->inicio, 0, scond->no, "(UTRAP_ON_FALSE(");
     add_descritor(scond->inicio+scond->comprimento, 0, scond->no, "))"  );
     sorteia_grupoU(Operador, Seq[Operador]);
     Seq[Operador]++;

   }
 } 
