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


/**************************************************************************
				S B R C. C

	Modulo que implementa o operador de mutacao de troca de continue por
break e break por continue.

Parametros:

**************************************************************************/

#include	<lib/gerais.h>
#include	<muta/lib/muta.h>
#include	"opmuta.h"


extern 	int	pt;		/* apontador p/ prog LI */

extern	LI_SIMBOLO	sym;	/* simbolo 'lido' */

extern	int	Seq[];

static	Operador;

extern	OPERADOR_MUTACAO g_tab_operador[]; 

 

u_scrb()
{
    Operador = U_SCRB;
   if (g_tab_operador[Operador].percent == 0)
        return;
    add_descritor(sym.inicio, sym.comprimento, sym.no, "break;");
    sorteia_grupoU(Operador, Seq[Operador]);
    Seq[Operador]++;
} 

u_sbrc()
{
    Operador = U_SBRC;
   if (g_tab_operador[Operador].percent == 0)
        return;
    add_descritor(sym.inicio, sym.comprimento, sym.no, "continue;");
    sorteia_grupoU(Operador, Seq[Operador]);
    Seq[Operador]++;
} 

