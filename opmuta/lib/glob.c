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


/************************************************************************
				G L O B . C
	Faz analise da parte global da LI.
***************************************************************************/


#include	<lib/gerais.h>
#include	"opmuta.h"
#include	<li/lib/li.h>

extern	int	pt;		/* apontador p/ prog LI */

extern	int	flg_erro,	/* flag de erro */
		flg_dcl;	/* indica declaracao */


extern	LI_SIMBOLO	sym;	/* simbolo 'lido' */

extern	char	bufmen[];	/* buffer auxiliar global */

extern	int	nivel;

extern	TABSIMBOLO	tab_vars, tab_tipos;


analisa_li_glob()
{
   pt = pli_inic_glob();	/* aponta inicio da  LI */
   sym = *(pli_getsym(pt));	/* pega primeiro simbolo */
   nivel = 0;
   while (e_dcl(sym))
   {
	glob_dcl();
   }

   if (! pli_fim_glob(pt))
	return ERRO;

   if (flg_erro)
	return ERRO;

   return OK;
}





glob_dcl()
{
   carrega_fonte(bufmen, &sym);
   if (! mparser(GLOBAL_DECLARATION, bufmen) )
	 return func_erro();
   flg_dcl = TRUE;
   sym = *(pli_getsym(++pt));
}
   


 
glob_erro()
{
  pt = MAX_LIN_LI;
  flg_erro = TRUE;
}


analisa_li_func(inic, name, dcl_size, cab_size, func_size, nome)
long	*inic;
int	*name, *dcl_size, *cab_size, *func_size;
char	nome[];
{
int	pt;
LI_SIMBOLO	sym;
DWORD	k;

   pt = pli_inic_func();

   sym = *(pli_getsym(pt++));
   strcpy(nome, &sym.simbolo[1]);
   k = sym.inicio;

   sym = *(pli_getsym(pt++));
   *inic = sym.inicio;
   *name = k - sym.inicio;
   *dcl_size = sym.comprimento;
   k = sym.inicio;

   while (e_dcl(sym))
     sym = *(pli_getsym(pt++));

   *cab_size = sym.inicio - *inic - *dcl_size;


   while (! pli_fim_func(pt++));
   sym = *(pli_getsym(pt -= 2));

   *func_size = sym.inicio + sym.comprimento - k;
   *inic += *name;
}


