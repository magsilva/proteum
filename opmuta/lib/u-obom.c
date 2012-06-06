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


u_oaaa(char *pre, char *pos)
{
OSET r;
   Operador = U_OAAA;
   set_new_inic(&r, "*=\0/=\0%=\0+=\0-=\0");
   u_obom(pre, pos, OP_ASS_ARIT, OP_ASS_ARIT, &r);
   set_free(&r);
}

u_oaan(char *pre, char *pos)
{
OSET r;
   Operador = U_OAAN;
   set_new_inic(&r, "+\0-\0*\0/\0%\0");
   u_obom(pre, pos, OP_2_ARIT, OP_2_ARIT, &r);
   set_free(&r);
}

u_oaba(char *pre, char *pos)
{
OSET r;
   Operador = U_OABA;
   set_new_inic(&r, "&=\0^=\0|=\0");
   u_obom(pre, pos, OP_ASS_ARIT, OP_ASS_BIT, &r);
   set_free(&r);
}


u_oabn(char *pre, char *pos)
{
OSET r;
   Operador = U_OABN;
   set_new_inic(&r, "|\0&\0^\0");
   u_obom(pre, pos, OP_2_ARIT, OP_2_BIT, &r);
   set_free(&r);
}

u_oaea(char *pre, char *pos)
{
OSET r;
   Operador = U_OAEA;
   set_new_inic(&r, "=\0");
   u_obom(pre, pos, OP_ASS_ARIT, OP_ASS_PLAIN, &r);
   set_free(&r);
}

u_oaln(char *pre, char *pos)
{
OSET r;
   Operador = U_OALN;
   set_new_inic(&r, "||\0&&\0");
   u_obom(pre, pos, OP_2_ARIT, OP_2_LOG, &r);
   set_free(&r);
}

u_oarn(char *pre, char *pos)
{
OSET r;
   Operador = U_OARN;
   set_new_inic(&r, "<\0>\0<=\0>=\0==\0!=\0");
   u_obom(pre, pos, OP_2_ARIT, OP_2_RELA, &r);
   set_free(&r);
}

u_oasa(char *pre, char *pos)
{
OSET r;
   Operador = U_OASA;
   set_new_inic(&r, "<<=\0>>=\0");
   u_obom(pre, pos, OP_ASS_ARIT, OP_ASS_SHIFT, &r);
   set_free(&r);
}

u_oasn(char *pre, char *pos)
{
OSET r;
   Operador = U_OASN;
   set_new_inic(&r, "<<\0>>\0");
   u_obom(pre, pos, OP_2_ARIT, OP_2_SHIFT, &r);
   set_free(&r);
}

u_obaa(char *pre, char *pos)
{
OSET r;
   Operador = U_OBAA;
   set_new_inic(&r, "*=\0/=\0%=\0+=\0-=\0");
   u_obom(pre, pos, OP_ASS_BIT, OP_ASS_ARIT, &r);
   set_free(&r);
}

u_oban(char *pre, char *pos)
{
OSET r;
   Operador = U_OBAN;
   set_new_inic(&r, "+\0-\0*\0/\0%\0");
   u_obom(pre, pos, OP_2_BIT, OP_2_ARIT, &r);
   set_free(&r);
}

u_obba(char *pre, char *pos)
{
OSET r;
   Operador = U_OBBA;
   set_new_inic(&r, "&=\0^=\0|=\0");
   u_obom(pre, pos, OP_ASS_BIT, OP_ASS_BIT, &r);
   set_free(&r);
}

u_obbn(char *pre, char *pos)
{
OSET r;
   Operador = U_OBBN;
   set_new_inic(&r, "|\0&\0^\0");
   u_obom(pre, pos, OP_2_BIT, OP_2_BIT, &r);
   set_free(&r);
}

u_obea(char *pre, char *pos)
{
OSET r;
   Operador = U_OBEA;
   set_new_inic(&r, "=\0");
   u_obom(pre, pos, OP_ASS_BIT, OP_ASS_PLAIN, &r);
   set_free(&r);
}

u_obln(char *pre, char *pos)
{
OSET r;
   Operador = U_OBLN;
   set_new_inic(&r, "||\0&&\0");
   u_obom(pre, pos, OP_2_BIT, OP_2_LOG, &r);
   set_free(&r);
}

u_obrn(char *pre, char *pos)
{
OSET r;
   Operador = U_OBRN;
   set_new_inic(&r, "<\0>\0<=\0>=\0==\0!=\0");
   u_obom(pre, pos, OP_2_BIT, OP_2_RELA, &r);
   set_free(&r);
}

u_obsa(char *pre, char *pos)
{
OSET r;
   Operador = U_OBSA;
   set_new_inic(&r, "<<=\0>>=\0");
   u_obom(pre, pos, OP_2_BIT, OP_ASS_SHIFT, &r);
   set_free(&r);
}

u_obsn(char *pre, char *pos)
{
OSET r;
   Operador = U_OBSN;
   set_new_inic(&r, "<<\0>>\0");
   u_obom(pre, pos, OP_2_BIT, OP_2_SHIFT, &r);
   set_free(&r);
}

u_oeaa(char *pre, char *pos)
{
OSET r;
   Operador = U_OEAA;
   set_new_inic(&r, "*=\0/=\0%=\0+=\0-=\0");
   u_obom(pre, pos, OP_ASS_PLAIN, OP_ASS_ARIT, &r);
   set_free(&r);
}

u_oeba(char *pre, char *pos)
{
OSET r;
   Operador = U_OEBA;
   set_new_inic(&r, "&=\0^=\0|=\0");
   u_obom(pre, pos, OP_ASS_PLAIN, OP_ASS_BIT, &r);
   set_free(&r);
}

u_oesa(char *pre, char *pos)
{
OSET r;
   Operador = U_OESA;
   set_new_inic(&r, "<<=\0>>=\0");
   u_obom(pre, pos, OP_ASS_PLAIN, OP_ASS_SHIFT, &r);
   set_free(&r);
}

u_olan(char *pre, char *pos)
{
OSET r;
   Operador = U_OLAN;
   set_new_inic(&r, "+\0-\0*\0/\0%\0");
   u_obom(pre, pos, OP_2_LOG, OP_2_ARIT, &r);
   set_free(&r);
}

u_olbn(char *pre, char *pos)
{
OSET r;
   Operador = U_OLBN;
   set_new_inic(&r, "|\0&\0^\0");
   u_obom(pre, pos, OP_2_LOG, OP_2_BIT, &r);
   set_free(&r);
}

u_olln(char *pre, char *pos)
{
OSET r;
   Operador = U_OLLN;
   set_new_inic(&r, "||\0&&\0");
   u_obom(pre, pos, OP_2_LOG, OP_2_LOG, &r);
   set_free(&r);
}

u_olrn(char *pre, char *pos)
{
OSET r;
   Operador = U_OLRN;
   set_new_inic(&r, "<\0>\0<=\0>=\0==\0!=\0");
   u_obom(pre, pos, OP_2_LOG, OP_2_RELA, &r);
   set_free(&r);
}

u_olsn(char *pre, char *pos)
{
OSET r;
   Operador = U_OLSN;
   set_new_inic(&r, "<<\0>>\0");
   u_obom(pre, pos, OP_2_LOG, OP_2_SHIFT, &r);
   set_free(&r);
}

u_oran(char *pre, char *pos)
{
OSET r;
   Operador = U_ORAN;
   set_new_inic(&r, "+\0-\0*\0/\0%\0");
   u_obom(pre, pos, OP_2_RELA, OP_2_ARIT, &r);
   set_free(&r);
}

u_orbn(char *pre, char *pos)
{
OSET r;
   Operador = U_ORBN;
   set_new_inic(&r, "|\0&\0^\0");
   u_obom(pre, pos, OP_2_RELA, OP_2_BIT, &r);
   set_free(&r);
}

u_orln(char *pre, char *pos)
{
OSET r;
   Operador = U_ORLN;
   set_new_inic(&r, "||\0&&\0");
   u_obom(pre, pos, OP_2_RELA, OP_2_LOG, &r);
   set_free(&r);
}

u_orrn(char *pre, char *pos)
{
OSET r;
   Operador = U_ORRN;
   set_new_inic(&r, "<\0>\0<=\0>=\0==\0!=\0");
   u_obom(pre, pos, OP_2_RELA, OP_2_RELA, &r);
   set_free(&r);
}

u_orsn(char *pre, char *pos)
{
OSET r;
   Operador = U_ORSN;
   set_new_inic(&r, "<<\0>>\0");
   u_obom(pre, pos, OP_2_RELA, OP_2_SHIFT, &r);
   set_free(&r);
}

u_osaa(char *pre, char *pos)
{
OSET r;
   Operador = U_OSAA;
   set_new_inic(&r, "*=\0/=\0%=\0+=\0-=\0");
   u_obom(pre, pos, OP_ASS_SHIFT, OP_ASS_ARIT, &r);
   set_free(&r);
}

u_osan(char *pre, char *pos)
{
OSET r;
   Operador = U_OSAN;
   set_new_inic(&r, "*=\0/=\0%=\0+=\0-=\0");
   u_obom(pre, pos, OP_2_SHIFT, OP_2_ARIT, &r);
   set_free(&r);
}

u_osba(char *pre, char *pos)
{
OSET r;
   Operador = U_OSBA;
   set_new_inic(&r, "&=\0^=\0|=\0");
   u_obom(pre, pos, OP_ASS_SHIFT, OP_ASS_BIT, &r);
   set_free(&r);
}


u_osbn(char *pre, char *pos)
{
OSET r;
   Operador = U_OSBN;
   set_new_inic(&r, "|\0&\0^\0");
   u_obom(pre, pos, OP_2_SHIFT, OP_2_BIT, &r);
   set_free(&r);
}

u_osea(char *pre, char *pos)
{
OSET r;
   Operador = U_OSEA;
   set_new_inic(&r, "=\0");
   u_obom(pre, pos, OP_ASS_SHIFT, OP_ASS_PLAIN, &r);
   set_free(&r);
}

u_osln(char *pre, char *pos)
{
OSET r;
   Operador = U_OSLN;
   set_new_inic(&r, "||\0&&\0");
   u_obom(pre, pos, OP_2_SHIFT, OP_2_LOG, &r);
   set_free(&r);
}

u_osrn(char *pre, char *pos)
{
OSET r;
   Operador = U_OSRN;
   set_new_inic(&r, "<\0>\0<=\0>=\0==\0!=\0");
   u_obom(pre, pos, OP_2_SHIFT, OP_2_RELA, &r);
   set_free(&r);
}

u_ossa(char *pre, char *pos)
{
OSET r;
   Operador = U_OSSA;
   set_new_inic(&r, "<<=\0>>=\0");
   u_obom(pre, pos, OP_ASS_SHIFT, OP_ASS_SHIFT, &r);
   set_free(&r);
}

u_ossn(char *pre, char *pos)
{
OSET r;
   Operador = U_OSSN;
   set_new_inic(&r, "<<\0>>\0");
   u_obom(pre, pos, OP_2_SHIFT, OP_2_SHIFT, &r);
   set_free(&r);
}


u_obom(char *pre, char *pos, TIPO_NO troca, TIPO_NO por, OSET *op)
{
char    *q, *get_rand();
int     i, j, n, max, seq, nseq;
int     u, k, t;
OSET    *r, *p = &set_aux, *get_const_set();

   if (g_tab_operador[Operador].percent == 0)
        return;

   r = op;
   u = sym.no;
   max = g_tab_operador[Operador].maximum;
   if (max == 0)
        max--;

   if (expre_pos.topo >= 0)
   {

       ex_aux = expre_pos;

       for (i = 0; i <= expre_pos.topo; i++)
       {            /* procura referencia do tipo desejado */

           if (expre_pos.no[i].tipo != troca)   
               continue;

           set_dup(r, p);
           nseq = set_card(p);
           for (n = max,q = get_rand(p, Operador);
                     n != 0 && q != NULL; q = get_rand(p, Operador), n--)
           {
                seq = set_inn(r, q);
		if (expre_pos.no[i].tipo == por && 
		       strcmp(NOME((&expre_pos), i), q) == 0)
                {
                   n++;
                   continue;
                }

		expre_pos.no[i].tipo = por;
		expre_pos.no[i].nome = expre_pos.livre;
		strcpy(&(expre_pos.buf[expre_pos.livre]), q);
		expre_pos.livre += strlen(q)+1;

                if (! expre_valida(&expre_pos))
                {
                   n++;
                   expre_pos = ex_aux;
                   continue;
                }

                monta_in_expre(&expre_pos, 0, expre_pos.topo);
                sprintf(bufmen, "%s %s ", pre, expre_pos.buf);
                strcat(bufmen, pos);
                add_descritor(sym.inicio, sym.comprimento, u, bufmen);
                sorteia_grupoU(Operador, Seq[Operador]+seq);

                expre_pos = ex_aux;

           }
           set_free(p);
           Seq[Operador] += nseq;
        }
   }

}


