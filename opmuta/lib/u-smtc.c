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



#include	<lib/gerais.h>
#include	<muta/lib/muta.h>
#include	"opmuta.h"


extern 	int	pt;		/* apontador p/ prog LI */

extern	LI_SIMBOLO	sym;	/* simbolo 'lido' */

extern	int	Seq[];

static	Operador;

extern	OPERADOR_MUTACAO g_tab_operador[]; 

extern  char    bufmen[];       /* buffer auxiliar global */

u_smtc(LI_SIMBOLO *sdo , LI_SIMBOLO *scoman, 
	LI_SIMBOLO *swhile, LI_SIMBOLO *scond)
{

    Operador = U_SMTC;
   if (g_tab_operador[Operador].percent != 0)
   {
   add_descritor(sdo->inicio, 0, sdo->no, "{ int _PROTEUM_LOCAL_VAR_ = 1; "  );
   add_descritor(scoman->inicio, 0, scoman->no, " { FALSE_AFTER_N_INTER(2); ");
   add_descritor(swhile->inicio, 0, swhile->no, " } ");
   add_descritor(scond->inicio, 0, scond->no, " } ");
   sorteia_grupoU(Operador, Seq[Operador]);
   Seq[Operador]++;
   }
    Operador = U_SMTT;
   if (g_tab_operador[Operador].percent == 0)
        return;
   add_descritor(sdo->inicio, 0, sdo->no, "{ int _PROTEUM_LOCAL_VAR_ = 1; "  );
   add_descritor(scoman->inicio, 0, scoman->no, " { TRAP_AFTER_N_INTER(2); ");
   add_descritor(swhile->inicio, 0, swhile->no, " } ");
   add_descritor(scond->inicio, 0, scond->no, " } ");
   sorteia_grupoU(Operador, Seq[Operador]);
   Seq[Operador]++;
} 


