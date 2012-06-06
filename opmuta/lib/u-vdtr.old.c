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


extern	OPERADOR_MUTACAO g_tab_operador[]; 

extern  int     pt;             /* apontador p/ prog LI */

extern  LI_SIMBOLO      sym;    /* simbolo 'lido' */

extern  char    bufmen[];       /* buffer auxiliar global */

extern  char    tipo[], ptar[];

extern	int	Seq[];

extern	VAR_SET	var_set_aux;

extern  EXPRE_POS       expre_pos,
			ex_aux;
EXPRE_POS		ex_aux1;

static	Operador;

extern OSET set_aux;

static  VAR_SET *r;

extern	char	*g_flgg;


u_vdtr(char *pre, char *pos)
{
char	*p, *q, tipo[100], ptar[100]; 
int	i, j, fim, max;
int	u, n, seq, nseq;
OSET    r, x;

  Operador = U_VDTR;
  set_new_inic(&r, "TRAP_ON_POSITIVE\0TRAP_ON_NEGATIVE\0TRAP_ON_ZERO\0");

  if (g_tab_operador[Operador].percent==0) return;

  u = sym.no;

  max = g_tab_operador[Operador].maximum; 
  if (max ==0) max--;


   if (expre_pos.topo >= 0)
     {
	expre_completa(&expre_pos);
	ex_aux1 = expre_pos;

	for (i = 0; i <= expre_pos.topo; i++)
	   {		/* procura referencia do tipo desejado */
		if ( ! expre_e_referencia(&expre_pos, i) ||
		     ! expre_e_escalar(&expre_pos, i, tipo, ptar))
			     continue;

	        if (! e_parametro(g_flgg, &expre_pos, i))
		         continue;
 		


		fim = expre_pos.no[i].final;

		ins_expre_apos(&expre_pos, ")", OP_POS2, i);
		ins_expre_apos(&expre_pos, p, VARIAVEL, fim-1);
		expre_completa(&expre_pos);

		if (! expre_valida(&expre_pos))
			     continue;

		ex_aux = expre_pos;
                nseq = set_card(p);

                for (n = max, q = get_rand(p, Operador); 
		q != NULL && n != 0;	q = get_rand(p, Operador),n--)


		   {	
	         	if (! expre_valida(&expre_pos))
			{    
                           n++; 
 			   continue;
			}

                        seq = set_inn(&r, q);

			expre_substitui(&expre_pos, fim, q, VARIAVEL);

			monta_in_expre(&expre_pos, 0, expre_pos.topo); /* monta infixa */
			strcpy(bufmen, pre);
			strcat(bufmen, expre_pos.buf);
                        sprintf(bufmen, "%s %s ",pre, expre_pos.buf);
			strcat(bufmen, pos);
			add_descritor(sym.inicio, sym.comprimento, u, bufmen);
      	                sorteia_grupoU(Operador, Seq[Operador] + seq);
			expre_pos = ex_aux;
		    }
                    var_ser_free(p);
                    Seq[Operador]+=seq;
            }
      }

}
