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

extern	char	func_name[];

extern  char    tipo[], ptar[];

extern	int	Seq[];

extern	OSET	set_aux;

extern  EXPRE_POS       expre_pos,
ex_aux;

static	Operador;


u_vvdl(char *pre, char *pos)
{
    char    *q, *get_rand();
    int     i, j, n, max;
    int     u, k, t;

    Operador = U_VVDL;
    if (g_tab_operador[Operador].percentage == 0.0)
        return;

    u = sym.no;
    max = g_tab_operador[Operador].maximum;
    if (max == 0)
        max--;

    if (expre_pos.topo >= 0)
    {

        ex_aux = expre_pos;

        for (i = 0; i <= expre_pos.topo; i++)
        {
            /* procura referencia do tipo desejado */

            if ( ! eh_operador_binario(expre_pos.no[i].tipo) )   // procura operadores binarios nao atribuicao
                continue;
            expre_completa(&expre_pos);

            if ( expre_e_referencia(&expre_pos, i-1) )
            {
                    // op unario apenas remove o operador e a variavel
                    expre_pos.no[i].tipo = NOP;
                    expre_substitui(&expre_pos, i-1, "", NOP);
                    expre_reload_NOP(&expre_pos, i); // move os NOPS para baixo e os operandos para cima

//               expre_completa(&expre_pos);
                    if ( expre_valida(&expre_pos) )
                    {
                        monta_in_expre(&expre_pos, 0, expre_pos.topo);
                        sprintf(bufmen, "%s %s ", pre, expre_pos.buf);
                        strcat(bufmen, pos);
                        add_descritor(sym.inicio, sym.comprimento, u, bufmen);
                        sorteia_grupoU(Operador, Seq[Operador]++);
                    }
                    expre_pos = ex_aux;
                    expre_completa(&expre_pos);
            }

            k = expre_pos.no[i-1].final - 1;
            if ( expre_e_referencia(&expre_pos, k) )
            {// operador binario verifica se o segundo operando é VARIAVEL
                    expre_pos.no[i].tipo = NOP;
                    expre_substitui(&expre_pos, k, "", NOP);
                    expre_reload_NOP(&expre_pos, k); // move os NOPS para baixo e os operandos para cima
                    expre_reload_NOP(&expre_pos, i); // move os NOPS para baixo e os operandos para cima
//               expre_completa(&expre_pos);
                    if ( expre_valida(&expre_pos) )
                    {
                        monta_in_expre(&expre_pos, 0, expre_pos.topo);
                        sprintf(bufmen, "%s %s ", pre, expre_pos.buf);
                        strcat(bufmen, pos);
                        add_descritor(sym.inicio, sym.comprimento, u, bufmen);
                        sorteia_grupoU(Operador, Seq[Operador]++);
                        expre_pos = ex_aux;
                        expre_completa(&expre_pos);
                    }
            }
            expre_pos = ex_aux;
        }
    }
}





