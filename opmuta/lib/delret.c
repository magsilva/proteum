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

extern int     Seq[];

extern	OPERADOR_MUTACAO g_tab_operador[]; 

extern  LI_SIMBOLO      sym;    /* simbolo 'lido' */

extern  char    bufmen[];       /* buffer auxiliar global */

static	Operador;


DelRetStat()
{
int     u, t;

   Operador = DEL_RET_STAT;
   if (g_tab_operador[Operador].percent == 0)
        return;

   u = sym.no;
   sprintf(bufmen,"IF_NOT_MUTA(");
   t = strlen(bufmen);
   carrega_fonte(&bufmen[t], &sym);
   strcat(bufmen, ")");
   add_descritor(sym.inicio, sym.comprimento, u, bufmen);
   sorteia_grupo1(Operador, ++(Seq[Operador]));
}


