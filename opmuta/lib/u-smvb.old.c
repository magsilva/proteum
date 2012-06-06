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

extern  EXPRE_POS       expre_pos,
                        ex_aux;
extern char bufmen[];

static	Operador;

u_smvb(int flag, LI_SIMBOLO *sfor, LI_SIMBOLO *sult, LI_SIMBOLO *sout, LI_SIMBOLO *sfecha, LI_SIMBOLO *scond) 
{
 int comando = flag;
 Operador = U_SMVB;
 if (g_tab_operador[Operador].percent != 0)
 {
   if (comando == 0)  /*comando for ou while*/
   {

     /*coloca um } antes do ultimo comando do loop (move brace up)*/

     add_descritor(sfor->inicio, 0, sfor->no, "{");
     add_descritor(sult->inicio, 0, sult->no, "}");
     sorteia_grupoU(Operador, Seq[Operador]);
     Seq[Operador]++;

     /*trata a insercao de um comando no loop(move brace down)*/

     add_descritor(sfecha->inicio, sfecha->comprimento, sfecha->no, "");
     add_descritor(sout->inicio, sout->comprimento, sout->no, "}");
     sorteia_grupoU(Operador, Seq[Operador]);
     Seq[Operador]++;
    }

    else
     /*comando repeat*/
      {
        add_descritor(sfor->inicio, 0, sfor->no, "{");
        strcpy(bufmen, "} while ");
        carrega_fonte(bufmen[8], &scond);
        strcat(bufmen, "\n");
        add_descritor(sult->inicio, 0, sult->no, bufmen);
        add_descritor(sfecha->inicio, comando, "}");
        sorteia_grupoU(Operador, Seq[Operador]);
        Seq[Operador]++;

        add_descritor(sfecha->inicio, comando, "");
        add_descritor(sout->inicio, 0, bufmen);
        sorteia_grupoU(Operador, Seq[Operador]);
        Seq[Operador]++;       
      }
 }  
}



