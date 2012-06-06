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

LI_SIMBOLO ultimo;

u_smvb(int flag, LI_SIMBOLO *sfor, LI_SIMBOLO *sult, LI_SIMBOLO *sout, LI_SIMBOLO *sfecha, LI_SIMBOLO *scond)
{
 int i;
  Operador = U_SMVB;
 if (g_tab_operador[Operador].percent == 0)
     return;
   if ( ! e_fecha((*sfecha) ))
   	return;
   if (flag == 0)  /*comando for ou while*/
   {


     if ( e_sequencial((*sult)) )
     {

     /*coloca um } antes do ultimo comando do loop (move brace up)*/

        add_descritor(sult->inicio, 0, sult->no, " } ");
        add_descritor(sfecha->inicio, sfecha->comprimento, sfecha->no, "");
        sorteia_grupoU(Operador, Seq[Operador]);
        Seq[Operador]++;
     }

     if ( e_sequencial((*sout)) )
     {
     /*trata a insercao de um comando no loop(move brace down)*/
           add_descritor(sfecha->inicio, sfecha->comprimento, sfecha->no, "");
           add_descritor(sout->inicio +sout->comprimento, 0, sout->no, "}");
           sorteia_grupoU(Operador, Seq[Operador]);
           Seq[Operador]++;
      }
    }


    else
    {
     /*comando repeat*/
        strcpy(bufmen, " } while ");
	carrega_fonte(&(bufmen[9]), scond);
        strcat(bufmen, "\n");

        if ( e_sequencial((*sult)) )
        {

     /*coloca um } antes do ultimo comando do loop (move brace up)*/

           add_descritor(sult->inicio, 0, sult->no, bufmen);
           add_descritor(sfecha->inicio, sout->inicio - sfecha->inicio, sfecha->no, "");
           sorteia_grupoU(Operador, Seq[Operador]);
           Seq[Operador]++;
        }

        if ( e_sequencial((*sout)) )
        {
        /*trata a insercao de um comando no loop(move brace down)*/
           add_descritor(sfecha->inicio, sout->inicio - sfecha->inicio, sfecha->no, "");
           add_descritor(sout->inicio +sout->comprimento, 0, sout->no, bufmen);
           sorteia_grupoU(Operador, Seq[Operador]);
           Seq[Operador]++;
         }


      }

}



