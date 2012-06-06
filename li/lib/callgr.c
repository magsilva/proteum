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


#include <stdio.h>
#include <lib/gerais.h>
#include "li.h"

int	contcalls;

extern	int	flg_gera;	/* indica se deve gerar li */

extern	FILE	*arqcall;

struct 	tableout ident;		/* token com nome de funcao definida */

char	func_name[TAMLEX];

OSET	cl;

cg_begin_function(s)
char	*s;
{
   strcpy(func_name, s);
   set_new(&cl);
   contcalls = 0;
}

cg_add_call(s)
char *s;
{
   if (  set_inn(&cl, s) < 0)
   {	
   	set_add(&cl, s);
	contcalls++;
   }
}

cg_fim_function()
{
char	*p;
int	col = 0, i;

   if (! flg_gera)
	return;
   fprintf(arqcall, "\n@%s\n", func_name);
   for (p = set_get(&cl, i=0); p != NULL; p = set_get(&cl, ++i))
   {
	while (col % 20)
	{
	   fprintf(arqcall, " ");
	   col++;
	}
	if (col + strlen(p) + 5 >= 80)
	{
	   fprintf(arqcall, "\n");
	   col = 0;
	}
	fprintf(arqcall, "     %s", p);
	col += (strlen(p) + 5);
   }
   fprintf(arqcall, "\n\n");
   set_free(&cl);
}


