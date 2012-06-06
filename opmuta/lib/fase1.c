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
#include	<polonesa/polonesa.h>
#include	<li/lib/li.h>
#include	"opmuta.h"


extern	int	pt;		/* apontador p/ prog LI */

extern	int	flg_erro,	/* flag de erro */
		flg_dcl;	/* indica declaracao */

extern	LI_SIMBOLO	sym;	/* simbolo 'lido' */

extern	char	bufmen[];	/* buffer auxiliar global */

extern	EXPRE_POS	expre_pos;

EXPRE_POS	ex_aux;

extern	int	nivel;

extern	char	*g_conexoes, NameFunc[];

extern	TABSIMBOLO	tab_vars, tab_tipos;

extern	char	tipo[], ptar[];

static	char	func_name[TAMLEX+1];

static	int	cont;



gera_fase1()
{
OSET	*p, *get_param_set();
char	*q;
int	i, ehvoid;

   cont = 1;
   if (func_inic() == ERRO)
   {
	erro:
	   msg("Sintatic Error in LI Program");
	   return ERRO;
   }
   if (get_tipo_varr(NameFunc, tipo, ptar) == ERRO)
	goto erro;
   i = strlen(ptar);
   if (ptar[i-1] != '(')
	goto erro;

   ptar[i-1] = '\0';

   ehvoid = e_void(tipo, ptar);
   p = get_param_set();
   i = 0;
   q = set_get(p, i);
   sprintf(bufmen, "(%s", q? q: "");
   while ((q = set_get(p, ++i)) != NULL)
   {
	strcat(bufmen, ", ");
	strcat(bufmen, q);
   }
   strcat(bufmen, ")");
   set_free(p);
   GeraFuncFase1(ehvoid, bufmen);

   fase1_comando();
   if (flg_erro)
	     goto erro;
   func_fim();
   return OK;
}




fase1_comando()
{

   if (e_abre(sym))
	     fase1_comp();
	else
   if (e_sequencial(sym))
	     fase1_seq();
	else
   if (e_if(sym))
	     fase1_if();
	else
   if (e_case(sym))
	     fase1_case();	
	else
   if (e_for(sym))
	     fase1_for();	
	else
   if (e_while(sym))
	     fase1_while();	
	else
   if (e_repeat(sym))
	     fase1_repeat();	
	else
   if (e_goto(sym))
	     fase1_goto();	
	else
   if (e_break(sym))
	     fase1_break();	
	else
   if (e_continue(sym))
	     fase1_continue();	
	else
   if (e_return(sym))
	     fase1_return();
	else
   if (e_label(sym))	
	     {
   		sym = *(pli_getsym(++pt));
		fase1_comando();
	     }
	else
   if (e_dcl(sym))
	     func_dcl();
	else func_erro();

}



/************************************************************************
FASE1_XXX:
	Trata cada um dos possiveis comandos da LI

***********************************************************************/

fase1_comp()	/* trata comando composto */
{

   pt++;
   nivel++;
   sym = *(pli_getsym(pt));
   while (! e_fecha(sym) && ! pli_fim_func(pt))
      {
	fase1_comando();
      }
   if (! e_fecha(sym))
	     func_erro();
   esvazia_tab(&tab_vars, nivel);  /*libera todos simbolos definidos no nivel */
   esvazia_tab(&tab_tipos, nivel);
   flg_dcl = TRUE;
   nivel--;
   pt++;
   sym = *(pli_getsym(pt));
}







fase1_if()
{

   sym = *(pli_getsym(++pt));

   if ( ! e_cond(sym))
	return func_erro();
   fase1_seq1();
  
   fase1_comando();

   if ( e_else(sym))
	     {
		sym = *(pli_getsym(++pt));
		fase1_comando();
	      }

}


fase1_case()
{

   sym = *(pli_getsym(++pt));
   if (! e_cond(sym))
	return func_erro();
   fase1_seq1();

   if ( ! e_abre(sym))
	     return func_erro();

   sym = *(pli_getsym(++pt));
   while ( e_rotc(sym))
     {
	sym = *(pli_getsym(++pt));
	while (! e_rotc(sym) && ! e_fecha(sym) && ! pli_fim_func(pt))
	   {
		fase1_comando();
	    }
	if (pli_fim_func(pt))
	        return func_erro();
     }

   sym = *(pli_getsym(++pt));
}


fase1_for()
{
      
   sym = *(pli_getsym(++pt));
   
   if ( ! e_sequencial(sym))
	return func_erro();
   fase1_seq(); 

   if (! e_cond(sym) )
	return func_erro();
   fase1_seq();

   if ( ! e_sequencial(sym))
	return func_erro();
   fase1_seq1(); 

   fase1_comando();

 }



fase1_while()
{
   sym = *(pli_getsym(++pt));

   if ( ! e_cond(sym))
	return func_erro();
   fase1_seq1();
  
   fase1_comando();


}


fase1_repeat()
{
   sym = *(pli_getsym(++pt));

   fase1_comando();

   if ( ! e_until(sym))
	return func_erro();
   sym = *(pli_getsym(++pt));

   if (! e_cond(sym))
	return func_erro();
   fase1_seq();
}


fase1_goto()
{
   sym = *(pli_getsym(++pt));

   if (! e_label(sym))
	return func_erro();

   sym = *(pli_getsym(++pt));
}


fase1_break()
{

   sym = *(pli_getsym(++pt));
}

fase1_continue()
{
   fase1_break();
}


fase1_seq1()
{
   if (carrega_fonte(bufmen, &sym) == ERRO)
             return func_erro();

   gera_expre(bufmen);  /* monta expressao posfixa */
   fase1_apply("(",")");
   sym = *(pli_getsym(++pt));
}


fase1_apply(pre, pos)
char *pre, *pos;
{
char	*p; 
int	i, j;
int	u;


   u = sym.no;

   if (expre_pos.topo >= 0)
     {

	expre_completa(&expre_pos);	/* completa expressao */
   	ex_aux = expre_pos;
	
	for (i = 0; i <= expre_pos.topo; i++)
	   {		/* procura referencia do tipo desejado */

		if (expre_pos.no[i].tipo != OP_POS2 ||
		    strcmp(NOME((&expre_pos), i), ")") != 0)
			continue;

		j = expre_pos.no[i].final;
		if (expre_pos.no[j].tipo != VARIAVEL)
			continue;

		strcpy(func_name, NOME((&expre_pos), j));
		if (! called_funcao(g_conexoes, NameFunc, func_name))
			continue;

		ins_expre_apos(&expre_pos, ")", OP_POS2, i);
		if (i == expre_pos.topo-1)
		   ins_expre_apos(&expre_pos,"PREPARE_MUTA",VARIAVEL, j-1);
		else
		   ins_expre_apos(&expre_pos,"PREPARE2_MUTA", VARIAVEL, j-1);


	 	 monta_in_expre(&expre_pos, 0, expre_pos.topo); /* monta infixa */
                 sprintf(bufmen, "%s%s%s", pre, expre_pos.buf, pos);


		 go_descritor_fase1(func_name, sym.inicio, cont, 
						sym.comprimento, u, bufmen);

		 expre_pos = ex_aux;
		 cont++;
            }
      }

}





fase1_seq()
{


   if (carrega_fonte(bufmen, &sym) == ERRO)
 	     return func_erro();

   gera_expre(bufmen);	/* monta expressao posfixa */
   fase1_apply("", ";");

   sym = *(pli_getsym(++pt));
}







fase1_return()
{


   if (carrega_fonte(bufmen, &sym) == ERRO)
 	     return func_erro();

   gera_expre(&bufmen[6]);	/* monta expressao posfixa */
   fase1_apply("return ", ";");
   sym = *(pli_getsym(++pt));
}

