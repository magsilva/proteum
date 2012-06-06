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


/**************************************************************************
				U - S T R P . C

	Modulo que implementa o operador de mutacao de armadilha em  
comandos. (trap on statement execution).
	A principio estao sendo tratados apenas os comandos da LI e 
as declaracoes nao estao. Assim, declaracoes como

int	i = 0;

ainda nao estao sendo mutadas.

Parametros:

**************************************************************************/


#include	<lib/gerais.h>
#include	<muta/lib/muta.h>
#include	<polonesa/polonesa.h>
#include	<li/lib/li.h>
#include	"opmuta.h"


extern OPERADOR_MUTACAO g_tab_operador[];

extern	int	pt;		/* apontador p/ prog LI */

extern	int	flg_erro,	/* flag de erro */
		flg_dcl;	/* indica declaracao */

extern	TABSIMBOLO	tab_vars, tab_tipos;

extern	int	nret;

extern	int	lastnode;

extern	LI_SIMBOLO	sym;	/* simbolo 'lido' */

extern	char	bufmen[];	/* buffer auxiliar global */

extern	EXPRE_POS	expre_pos;

extern int Seq[];

extern	LI_SIMBOLO	vet[100];


EXPRE_POS	ex_aux;

extern	int	nivel;

int max;

 
static int Operador, Operador2, Operador3;

u_strp()
{
   flg_dcl = TRUE;
   init_compute_set();
   Operador = U_STRP;
   Operador2 = U_SSDL;
   Operador3 = U_SRSR;
   if (g_tab_operador[Operador].percent == 0 &&
       g_tab_operador[Operador2].percent == 0 &&
       g_tab_operador[Operador3].percent == 0 ) return;
   max = g_tab_operador[Operador3].maximum;
   lastnode = -1;
   label_new();
   if (func_inic() == ERRO)
   {
	erro:
	label_release();
	msg("Sintatic Error in LI Program");
	return ERRO;
   }

   strp_comando();
   func_fim();
   if (flg_erro)
	     goto erro;
   label_release();
   return OK;
}


strp_comando()
{

   if (e_abre(sym))
	     strp_comp();
	else
   if (e_sequencial(sym))
	     strp_seq(0, OP_3);
	else
   if (e_if(sym))
	     strp_if();
	else
   if (e_case(sym))
	     strp_case();	
	else
   if (e_for(sym))
	     strp_for();	
	else
   if (e_while(sym))
	     strp_while();	
	else
   if (e_repeat(sym))
	     strp_repeat();	
	else
   if (e_goto(sym))
	     strp_goto();	
	else
   if (e_break(sym))
	     strp_break();	
	else
   if (e_continue(sym))
	     strp_continue();	
	else
   if (e_return(sym))
	     strp_return();
	else
   if (e_label(sym))	
	     {
		ins_label_list(&sym);
   		sym = *(pli_getsym(++pt));
		strp_comando();
	     }
	else
   if (e_dcl(sym))
	     func_dcl();
	else func_erro();

}


strp_comp()	/* comando composto */
{
LI_SIMBOLO	abrechave, s, s1;
int	u, pt1;
long	r;

   abrechave = sym;
   pt1 = label_size();
   pt++;
   nivel++;
   sym = *(pli_getsym(pt));
   while (! e_fecha(sym) && ! pli_fim_func(pt))
      {
	strp_comando();
      }
   if (! e_fecha(sym))
	     func_erro();
   esvazia_tab(&tab_vars, nivel);  /*libera todos simbolos definidos no nivel */
   esvazia_tab(&tab_tipos, nivel);
   flg_dcl = TRUE;
 
   r = abrechave.inicio+abrechave.comprimento;
   u = abrechave.no;

   /* GERA STRP */
   add_descritor(r, sym.inicio - 1 - r, u, "UTRAP_ON_STAT();");
   /* recoloca os labels de volta */
   relabel(pt1, r, u);
   sorteia_grupoU(Operador, Seq[Operador]++);

   /* GERA SSDL */
   add_descritor(r, sym.inicio - 1 - r, u, "");
   /* recoloca os labels de volta */
   relabel(pt1, r, u);
   sorteia_grupoU(Operador2, Seq[Operador2]++);
   
   /* GERA SRSR */
   gera_ret(pt1, r, sym.inicio, u, NULL);


   nivel--;
   pt++;
   sym = *(pli_getsym(pt));
}


strp_seq()
{
int	u;

   u = sym.no;
   add_descritor(sym.inicio, sym.comprimento, u,"UTRAP_ON_STAT();");
   sorteia_grupoU(Operador, Seq[Operador]++);

   if ( sym.comprimento > 1)
   {
	u = sym.no;
   	add_descritor(sym.inicio, sym.comprimento, u,";");
   	sorteia_grupoU(Operador2, Seq[Operador2]++);
   }
   gera_ret(-1, sym.inicio, sym.inicio+sym.comprimento,u, NULL);

   pt++;
   sym = *(pli_getsym(pt));
}

strp_if()
{
LI_SIMBOLO	sif;
int	u, pt1;

   sif = sym;
   sym = *(pli_getsym(++pt));
   pt1 = label_size();

   if ( ! e_cond(sym))
	return func_erro();
  
   sym = *(pli_getsym(++pt));
   strp_comando();

   if ( e_else(sym))
	     {
		sym = *(pli_getsym(++pt));
		strp_comando();
	      }

   u = sif.no;
   add_descritor(sif.inicio, sym.inicio-sif.inicio, u, "UTRAP_ON_STAT();");
   relabel(pt1, sif.inicio, u);
   sorteia_grupoU(Operador, Seq[Operador]++);

   add_descritor(sif.inicio, sym.inicio-sif.inicio, u, ";");
   relabel(pt1, sif.inicio, u);
   sorteia_grupoU(Operador2, Seq[Operador2]++);

   gera_ret(pt1, sif.inicio, sym.inicio, u, NULL);
}

strp_case()
{
LI_SIMBOLO	scase;
long	r;
int	u, pt1;

   scase = sym;
   pt1 = label_size();

   sym = *(pli_getsym(++pt));
   if (! e_cond(sym))
	return func_erro();

   sym = *(pli_getsym(++pt));
   if ( ! e_abre(sym))
	     return func_erro();

   sym = *(pli_getsym(++pt));
   while ( e_rotc(sym))
     {
	sym = *(pli_getsym(++pt));
	while (! e_rotc(sym) && ! e_fecha(sym) && ! pli_fim_func(pt))
	   {
		strp_comando();
	    }
	if (pli_fim_func(pt))
	        return func_erro();
     }

   r = sym.inicio+sym.comprimento;
   u = scase.no;
   add_descritor(scase.inicio,  r - scase.inicio, u, "UTRAP_ON_STAT();");
   relabel(pt1, scase.inicio, u);
   sorteia_grupoU(Operador, Seq[Operador]++);

   add_descritor(scase.inicio,  r - scase.inicio, u, ";");
   relabel(pt1, scase.inicio, u);
   sorteia_grupoU(Operador2, Seq[Operador2]++);

   gera_ret(pt1, scase.inicio, r, u, NULL);


   sym = *(pli_getsym(++pt));
}


strp_for()
{
LI_SIMBOLO	sfor;
long	r;
int	u, pt1;
      
   sfor = sym;
   sym = *(pli_getsym(++pt));
   pt1 = label_size();
   
   if ( ! e_sequencial(sym))
	return func_erro();
   sym = *(pli_getsym(++pt));

   if (! e_cond(sym) )
	return func_erro();

   sym = *(pli_getsym(++pt));   
   if ( ! e_sequencial(sym))
	return func_erro();
   sym = *(pli_getsym(++pt));

   strp_comando();

   u = sfor.no;
   add_descritor(sfor.inicio, sym.inicio-sfor.inicio, u, "UTRAP_ON_STAT();");
   relabel(pt1, sfor.inicio, u);
   sorteia_grupoU(Operador, Seq[Operador]++);

   add_descritor(sfor.inicio, sym.inicio-sfor.inicio, u, ";");
   relabel(pt1, sfor.inicio, u);
   sorteia_grupoU(Operador2, Seq[Operador2]++);

   gera_ret(pt1, sfor.inicio, sym.inicio, u, NULL);

}



strp_while()
{
LI_SIMBOLO	swhile;
int	u, pt1;

   swhile = sym;
   sym = *(pli_getsym(++pt));
   pt1 = label_size();

   if ( ! e_cond(sym))
	return func_erro();
  
   sym = *(pli_getsym(++pt));
   strp_comando();


   u = swhile.no;
   add_descritor(swhile.inicio, sym.inicio-swhile.inicio, u, "UTRAP_ON_STAT();");
   relabel(pt1, swhile.inicio, u);
   sorteia_grupoU(Operador, Seq[Operador]++);

   add_descritor(swhile.inicio, sym.inicio-swhile.inicio, u, ";");
   relabel(pt1, swhile.inicio, u);
   sorteia_grupoU(Operador2, Seq[Operador2]++);

   gera_ret(pt1, swhile.inicio, sym.inicio, u, NULL);

}


strp_repeat()
{
LI_SIMBOLO	srepeat;
int	u, pt1;
long	r;

   srepeat = sym;
   sym = *(pli_getsym(++pt));
   pt1 = label_size();

   strp_comando();

   if ( ! e_until(sym))
	return func_erro();

   sym = *(pli_getsym(++pt));
   if (! e_cond(sym))
	return func_erro();

   r = sym.inicio+sym.comprimento;
   u = srepeat.no;
   add_descritor(srepeat.inicio, r - srepeat.inicio, u, "UTRAP_ON_STAT();");
   relabel(pt1, srepeat.inicio, u);
   sorteia_grupoU(Operador, Seq[Operador]++);

   add_descritor(srepeat.inicio, r - srepeat.inicio, u, ";");
   relabel(pt1, srepeat.inicio, u);
   sorteia_grupoU(Operador2, Seq[Operador2]++);

   gera_ret(pt1, srepeat.inicio, r, u, NULL);


   sym = *(pli_getsym(++pt));
}


strp_goto()
{
LI_SIMBOLO	sgoto;
int	u;
long	r;

   sgoto = sym;
   sym = *(pli_getsym(++pt));

   if (! e_label(sym))
	return func_erro();

   r = sym.inicio+sym.comprimento;
   u = sgoto.no;
   add_descritor(sgoto.inicio, r - sgoto.inicio, u, "UTRAP_ON_STAT();");
   sorteia_grupoU(Operador, Seq[Operador]++);

   add_descritor(sgoto.inicio, r - sgoto.inicio, u, ";");
   sorteia_grupoU(Operador2, Seq[Operador2]++);

   gera_ret(-1, sgoto.inicio, r, u, NULL);


   sym = *(pli_getsym(++pt));
}

 
strp_break()
{
int	u;

   u = sym.no;
   add_descritor(sym.inicio,  sym.comprimento,	u, "UTRAP_ON_STAT();");
   sorteia_grupoU(Operador, Seq[Operador]++);

   add_descritor(sym.inicio,  sym.comprimento,	u, ";");
   sorteia_grupoU(Operador2, Seq[Operador2]++);

   gera_ret(-1, sym.inicio, sym.inicio+sym.comprimento, u, NULL);


   sym = *(pli_getsym(++pt));
}

strp_continue()
{
   strp_break();
}


strp_return()
{
int	u;

   u = sym.no;
   add_descritor(sym.inicio,  sym.comprimento,	u, "UTRAP_ON_STAT();");
   sorteia_grupoU(Operador, Seq[Operador]++);

   add_descritor(sym.inicio,  sym.comprimento,	u, ";");
   sorteia_grupoU(Operador2, Seq[Operador2]++);

   carrega_fonte(bufmen, &sym);
   gera_expre(&bufmen[6]);
   ex_aux = expre_pos;
   gera_ret(-1, sym.inicio, sym.inicio+sym.comprimento, sym.no, &ex_aux);
   sym = *(pli_getsym(++pt));
}



gera_ret(int pt1, int inicio, int final, int no, EXPRE_POS *expret)
{
int     i, n, max, seq, nseq;
char    *q, *get_rand();
OSET    *p, *monta_pset();

   if (g_tab_operador[Operador3].percent == 0)
        return;
   n = max;
   if (n == 0) n--;
   p = monta_pset(nret);
   nseq = nret;

   if (nseq == 0)
   {
	seq = 0;
	if (pt1 >= 0)
	   relabel(pt1,inicio, no);
        add_descritor(inicio, final-inicio, no, "return;");
	sorteia_grupoU(Operador3,Seq[Operador3]+seq); 
	nseq++;
   }
   else
   {
   compute_setL();
   for (q = get_rand(p, Operador3); 
	n != 0 && q != NULL; q = get_rand(p, Operador3), n--)
   {
	seq = i = atoi(q);
	carrega_fonte(bufmen, &vet[i]);
	gera_expre(&bufmen[6]);
	if (vet[i].linha != conta_local(&expre_pos) )
	{
		n++;
		continue;
	}

	if (expret != NULL )     /* is a return statement to replace */
	{

	    if (compara_expre(expret,&expre_pos))
	    {
		n++;
		continue;
	    }
	}


	if (pt1 >= 0)
	   relabel(pt1,inicio,no);
        add_descritor(inicio, final-inicio, no, bufmen);
        sorteia_grupoU(Operador3, Seq[Operador3]+seq);
   }
   }
   set_free(p);
   Seq[Operador3] += nseq;
}

