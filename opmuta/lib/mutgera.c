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


/*#######################################################################
			M U T G E R A . C

	Este modulo gerencia a geracao de mutantes e possui alguma 
funcoes auxiliares para geracao dos mutantes

Data de criacao: 06/05/93
########################################################################*/


#include	<lib/gerais.h> 
#include	<li/lib/li.h>
#include	<muta/lib/muta.h>
#include	"opmuta.h"


int	pt_label;

extern	char	bufmen[];

static  OSET set_label;

static  char buf[TAMLEX+2];

extern	int	Quantos, nivel;

extern  TABSIMBOLO      tab_vars, tab_tipos;

extern	VAR_SET		setL,
			setE,
			setG,
			setP;

int     pt;             /* apontador p/ prog LI */

int     flg_erro,       /* flag de erro */
        flg_dcl;        /* indica declaracao */

LI_SIMBOLO	sym;


static	LI_SIMBOLO	prog_li[MAX_LIN_LI], /* local para montar programa li */
			symli;


static	int	fim_glob,
		inic_func, fim_func;


/**************************************************************************
CARREGA_LI_GLOB:
	Carrega parte global do programa LI

AUtor: Delamaro
**************************************************************************/
carrega_li_glob(fp_li)
FILE	*fp_li;
{
unsigned long	t;
char	c;

   fim_func = fim_glob = 0;

   do {
	if (fim_glob >= MAX_LIN_LI)
	           {
			msg("Program so Big");
			return ERRO;
		   }
	t = ftell(fp_li);
	if (fscanf(fp_li, "%s %d %ld %d %d", symli.simbolo, &symli.no,
				&symli.inicio, &symli.comprimento, &symli.linha)
	    != 5)
		     symli.simbolo[0] = '\0';

	prog_li[fim_glob] = symli;

	c = symli.simbolo[0];
	if ( c == '\0' || c == '@')
	{
	        posiciona(fp_li, t);
	 }
  	else 
	{
		if ( ! (e_par(symli) && symli.inicio == 0))
						/*se for 0 eh } falso */
		   fim_glob++;
	}

       } while (c != '\0' & c != '@');
   return OK;
}


	
/**************************************************************************
CARREGA_LI_func:
	Carrega parte do programa LI

AUtor: Delamaro
**************************************************************************/
carrega_li_func(fp_li)
FILE	*fp_li;
{
unsigned long	t;
char	c;

   t = ftell(fp_li);
   fim_func = inic_func = 0;
   if (fscanf(fp_li, "%s %d %ld %d %d",symli.simbolo, &symli.no,&symli.inicio,
					&symli.comprimento, &symli.linha) != 5)
	     symli.simbolo[0] = '\0';

   if (symli.simbolo[0] != '@')
	     return ERRO;
   prog_li[fim_func++] = symli;

   do {
   	t = ftell(fp_li);
	if (fim_func >= MAX_LIN_LI)
	           {
			msg("Program so Big");
			return ERRO;
		   }
	if (fscanf(fp_li, "%s %d %ld %d %d", symli.simbolo, &symli.no,
				&symli.inicio, &symli.comprimento, &symli.linha)
	    != 5)
		     symli.simbolo[0] = '\0';

	prog_li[fim_func] = symli;

	c = symli.simbolo[0];
	if ( c == '\0' || c == '@')
	{
	        posiciona(fp_li, t);
	}
	else
	{
	     if (! (e_par(symli) && symli.inicio == 0)) 
						 /* se for 0 eh } falso */
		 fim_func++;
	}

       }  while (c != '\0' & c != '@');
   fim_glob = fim_func;

   while (e_dcl(prog_li[fim_func-1]))
	fim_func--;

   if (fim_func == inic_func || ! e_fecha(prog_li[fim_func-1]))
	     return ERRO;
   return OK;
}


/***************************************************************************
PLI_XXXXX:
	Estas funcoes manipulam o programa li na memoria, evitando que 
as funcoes que gerar os mutantes tenham que acessar variaveis globais
**************************************************************************/

pli_inic_glob()
{
   return fim_func;	/*inicio da area global */
}


pli_fim_glob(i)
int	i;
{
   return i >= fim_glob; 	/* verifica se e' fim da parte global */
}

pli_inic_func()
{
   return inic_func;	/*inicio da area global */
}


pli_fim_func(i)
int	i;
{
   return i >= fim_func; 	/* verifica se e' fim da funcao */
}


LI_SIMBOLO *pli_getsym(i)		/* retorna um simbolo da li */
{

   return &prog_li[i];
}



calling_funcao(g_funcoes, f)
char	g_funcoes[], f[];
{
char	*p, *q, *from_buf();
int	i;

   inic_from_buf(g_funcoes);
   p = from_buf(g_funcoes);
   if (p == NULL)	/* se nao tem nada no buffer, assume TODAS FUNCOES */
	return TRUE;

   while ( p != NULL)
      {
	strcpy(buf, p);
   	q = strchr(buf, '$');
	*q = '\0';
	if (strcmp(buf, "-all") == 0 || strcmp(buf, f) == 0)
	    return TRUE;
   	p = from_buf(g_funcoes);
      }
  return FALSE;
}


called_funcao(g_funcoes, f, f1)
char    g_funcoes[], f[], f1[];
{
char    *q, *p, *from_buf();
int     i;

   inic_from_buf(g_funcoes);
   p = from_buf(g_funcoes);
   if (p == NULL)       /* se nao tem nada no buffer, assume TODAS FUNCOES */
        return TRUE;

   while ( p != NULL)
      {
        strcpy(buf, p);
        q = strchr(buf, '$');
        *q = '\0';

        if ((f == NULL) || (strcmp(buf, "-all") == 0) || (strcmp(buf, f) == 0))
	{
           if (strcmp(q+1, "-all") == 0 || strcmp(q+1, f1) == 0)
              return TRUE;
	}
        p = from_buf(g_funcoes);
      }
  return FALSE;
}


unit_funcao(g_funcoes, f1)
char    g_funcoes[], f1[];
{
{
char	*p, *from_buf();

   inic_from_buf(g_funcoes);
   p = from_buf(g_funcoes);
   if (p == NULL)	/* se nao tem nada no buffer, assume TODAS FUNCOES */
	return TRUE;

   while ( p != NULL)
      {
	if (!strcmp(p, f1))
	    return TRUE;
   	p = from_buf(g_funcoes);
      }
  return FALSE;
}

}

/***********************************************************************
Funcoes gerais usadas por vario operadores 
***********************************************************************/

/**************************************************************************
CARREGA_FONTE:
	Carrega um trecho do fonte na memoria.

Parametros:
	buf: buffer para carregar;
	sym: simbolo da LI

Autor: Delamaro

**************************************************************************/
carrega_fonte(buf, sym)
char	buf[];
LI_SIMBOLO	*sym;
{
extern	FILE	*fp_fonte;

   buf[0] = '\0';
   if (sym->comprimento == 0)
	return OK;

   if (sym->comprimento > TAM_MAX_EXPR)
   {
	 msg("Statment too long");
	 fprintf(stderr, "Line: %d size: %d\n" , sym->linha, sym->comprimento);
	 buf[0] = '\0';
	 return ERRO;
   }
   posiciona(fp_fonte, sym->inicio-1);
   learq(fp_fonte, buf, sym->comprimento);
   buf[sym->comprimento] = 0;

   return OK;
}


/**********************************************************************
FUNC_DCL:
	This function analyses a declaration inside a function. It's
called by most mutation operators to deal a $DECL li command.
************************************************************************/
func_dcl()
{
int	i;

   carrega_fonte(bufmen, &sym);
   switch (nivel)
   {
	case 0:
	   i = mparser(FUNC_DECLARATION, bufmen);
	   break;
	case 1:
           i = mparser(PARAM_DECLARATION, bufmen);
           break;
        default:
           i = mparser(BLOCK_DECLARATION, bufmen);
           break;
   }
   if (i == 0)
	return func_erro();
   flg_dcl = TRUE;
   sym = *(pli_getsym(++pt));
}



/**********************************************************************
FUNC_ERRO:
        This function deal with an error in the li program
************************************************************************/
func_erro()
{
   pt = MAX_LIN_LI;
   flg_erro = TRUE;
}



/**************************************************************************
FUNC_INIC:
	This function analyzes the begining of a function. It's used
by most of the operators.
*************************************************************************/
func_inic()
{

   pt = pli_inic_func();        /* aponta inicio da funcao LI */
   sym = *(pli_getsym(pt));     /* pega primeiro simbolo */
   if (e_func(sym))
              sym = *(pli_getsym(++pt));
        else return ERRO;

 if (e_dcl(sym))        /* declaracao da funcao */
             {
                nivel = 0;
                func_dcl();
              }
        else return ERRO;

   nivel = 1;

   while (! pli_fim_func(pt) && ! e_abre(sym))
        {
           if (e_dcl(sym))
                     func_dcl();
                else return ERRO;
        }

   if (! e_abre(sym))
                return ERRO;

   flg_erro = FALSE;
   return OK;
}


func_fim()
{
   esvazia_tab(&tab_tipos, 1);
   esvazia_tab(&tab_vars, 1);
}



/**************************************************************************
GET_RAND:
	Randomly selects an element from a set.

Parameters:
OSET *s:	The set.
int  op:	The mutation operators

**************************************************************************/
char *get_rand(OSET *s, int op)
{
int	i;
static char	buf[TAMLEX+1];

   if (set_card(s) <= 0)
	return NULL;

   i = op_rand_int(op, set_card(s));
   strcpy(buf, set_get(s, i));
   set_del(s, i);
   return buf;
}

SIMBOLO	*var_get_rand(VAR_SET *s, int op)
{
int     i;
SIMBOLO *p;

   if (var_set_card(s) <= 0)
        return NULL;

   i = op_rand_int(op, var_set_card(s));
   p = var_set_get(s, i);
   var_set_del(s, i);
   return p;
}



/**************************************************************************
E_GLOBAL:
E_LOCAL:
	Verify if a simbol is a global/local variable (functions are
not included in the global/local set)
**************************************************************************/
int	e_local(char *x)
{
   return (var_set_inn(&setL, x) >= 0);
}


int	e_ext(char *x)
{
   return (var_set_inn(&setE, x) >= 0 && ! e_local(x));
}

int	e_interface(char *x)
{
   return (( var_set_inn(&setP, x) >= 0 || var_set_inn(&setG, x) >= 0 ) && 
	   ! e_local(x));
}



/************************************************************************
FUNCTIONS TAKE CARE OF LABELS
************************************************************************/

label_new()
{
   set_new(&set_label);
}

label_release()
{
   set_free(&set_label);
}


label_size()
{
   return set_card((&set_label));
}

ins_label_list(sym)
LI_SIMBOLO	*sym;
{
char	buf[sizeof(sym->simbolo)+2];

   sprintf(buf, "%s:\n", sym->simbolo);
   set_add(&set_label, buf);
}


relabel(pt1, offset, no)
int	pt1;
long offset;
int no;
{
char	*p;
int	i;
static	char buf[TAMLEX+5];

   if (pt1 < set_card((&set_label)) )
   {
      for (i = pt1, p = set_get(&set_label,i); p != NULL; 
            p  = set_get(&set_label,++i))
      {
	   strcpy(buf, p);
	   strcat(buf, " ;");
           add_descritor(offset, 0, no, buf);
      }
   }
}


char *label_muta()
{
   return "__LABEL_MUTA__";
}
