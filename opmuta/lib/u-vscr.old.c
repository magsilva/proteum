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

static	Operador;

extern OSET set_aux;

static  VAR_SET *r;



u_vscr(char *pre, char *pos)
{
char    *q, *var_get_rand();
VAR_SET	*p = &var_set_aux;
int	i, j, n, max, seq, nseq;
int	u;
char    *c;
   
  Operador = U_VSCR;
  if (g_tab_operador[Operador].percent==0) return;

  u = sym.no;

  max = g_tab_operador[Operador].maximum; 
  if (max ==0) max--;


  if (expre_pos.topo >= 0)
     {

	expre_completa(&expre_pos);	/* completa expressao */
   	ex_aux = expre_pos;

	for (i = 0; i <= expre_pos.topo; i++)
	   {		/* procura referencia do tipo desejado */
		if (expre_pos.no[i].tipo != CAMPO)
		         continue;

                var_set_dup(r, p);
                nseq = var_set_card(p);		

		j = expre_pos.no[i-1].final;

		/*calcula tipo */
		expre_exec_tipo(&expre_pos, j, i-1, tipo, ptar); 
		strcpy(bufmen, tipo);
		get_tipo_campo(bufmen, NOME((&expre_pos), i) , tipo,ptar);

		var_campo(bufmen, tipo, ptar, set_campo);

		for (n = max, q = get_rand(p, Operador);q != NULL && n != 0;
                           q = get_rand(p, Operador),n--)
		   {	/* troca pelas outras variaveis */ 

			if (strcmp(NOME((&expre_pos), i), q) == 0)
                        {
                            n++;
		            continue;
                         }

			expre_substitui(&expre_pos, i, q, CAMPO);

			if (expre_valida(&expre_pos))
			{
			monta_in_expre(&expre_pos, 0, expre_pos.topo); /* monta infixa */
                        sprintf(bufmen, "%s %s ", pre, expre_pos.buf);

			strcpy(bufmen, pre);
			strcat(bufmen, expre_pos.buf);
			strcat(bufmen, pos);
			add_descritor(sym.inicio, sym.comprimento, u, bufmen);

  	                sorteia_grupoU(Operador, Seq[Operador]+seq);

			}

			expre_pos = ex_aux;
		    }
		      var_set_free(p);
	              Seq[Operador] += nseq;

            }

      }

}


