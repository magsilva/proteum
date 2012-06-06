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

int	lastnode;

static	Operador;

extern	EXPRE_POS expre_pos;


CovAllNode(char *pre, char *pos)
{
int     u, t;

   Operador = COV_ALL_NODE;
   if (g_tab_operador[Operador].percent == 0)
        return;

   u = sym.no;
   if (u == lastnode)
	return;
   if (expre_pos.topo < 0)
	return;

   lastnode = u;
   monta_in_expre(&expre_pos, 0, expre_pos.topo);
   sprintf(bufmen, "%s TRAP(%s) %s", pre, expre_pos.buf, pos);
   add_descritor(sym.inicio, sym.comprimento, u, bufmen);
   sorteia_grupo1(Operador, ++(Seq[Operador]));
}

CovAllNode_stat()
{
int     u;
int 	i;

   Operador = COV_ALL_NODE;
   if (g_tab_operador[Operador].percent == 0)
        return;

   u = sym.no;
   if (u == lastnode)
        return;

   lastnode = u;
   strcpy(bufmen, "TRAP_ON_STAT(");
   i = strlen(bufmen);
   if (carrega_fonte(&bufmen[i], &sym) == ERRO)
 	     return;
   strcat(bufmen, ")");
   add_descritor(sym.inicio, sym.comprimento, u, bufmen);
   sorteia_grupo1(Operador, ++(Seq[Operador]));
}

CovAllNode_goto(LI_SIMBOLO *s)
{
int     u, t, i;

   Operador = COV_ALL_NODE;
   if (g_tab_operador[Operador].percent == 0)
        return;

   u = sym.no;
   if (u == lastnode)
        return;

   lastnode = u;
   strcpy(bufmen, "TRAP_ON_STAT(");
   i = strlen(bufmen);
   if (carrega_fonte(&bufmen[i], s) == ERRO)
 	     return;
   i = strlen(bufmen);
   bufmen[i++] = ' ';
   if (carrega_fonte(&bufmen[i], &sym) == ERRO)
 	     return;
   strcat(bufmen, ")");
   t = (sym.inicio + sym.comprimento) - s->inicio;
   add_descritor(s->inicio, t, u, bufmen);
   sorteia_grupo1(Operador, ++(Seq[Operador]));
}



CovAllEdge(char *pre, char *pos)
{
int	u, t;

   Operador = COV_ALL_EDGE;
   if (g_tab_operador[Operador].percent == 0)
        return;

   u = sym.no;
   if (sym.comprimento == 0)
	return;
   if (expre_pos.topo < 0)
        return;

   monta_in_expre(&expre_pos, 0, expre_pos.topo);
   sprintf(bufmen, "%s TRAP_ON_TRUE(%s) %s", pre, expre_pos.buf, pos);
   add_descritor(sym.inicio, sym.comprimento, u, bufmen);
   sorteia_grupo1(Operador, ++(Seq[Operador]));

   sprintf(bufmen, "%s TRAP_ON_FALSE(%s) %s", pre, expre_pos.buf, pos);
   add_descritor(sym.inicio, sym.comprimento, u, bufmen);
   sorteia_grupo1(Operador, ++(Seq[Operador]));
}


CovAllEdgeSw(int tembreak)
{
int	u, t;

   Operador = COV_ALL_EDGE;
   if (g_tab_operador[Operador].percent == 0)
        return;

   u = sym.no;

   if (! tembreak)
   {
	add_descritor(sym.inicio, 0, u, "goto __PROTEUM__LABEL__;");
   }
   add_descritor(sym.inicio+sym.comprimento, 0, u, " TRAP_ON_CASE(); ");
   
   sorteia_grupo1(Operador, ++(Seq[Operador]));
}


