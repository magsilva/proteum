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
				O P M U T A . C

Neste arquivo estao as funcoes que gerenciam as chamadas para as funcoes
que geram os mutantes. Cada operador de mutacao tem uma funcao associada
a ele.

Data: 19-10-94
Autor: Delamaro
**************************************************************************/

#include	<lib/gerais.h>
#include	<li/lib/li.h>
#include	<muta/lib/muta.h>
#include	"opmuta.h"


extern	OPERADOR_MUTACAO	g_tab_operador[];

LI_SIMBOLO *get_list_func(char *f);

char	bufmen[TAM_MAX_EXPR];

char	tipo[TAMLEX+1], ptar[TAMLEX+1],
	tipo1[TAMLEX+1], ptar1[TAMLEX+1];

FILE	*fp_li, *fp_fonte;

OSET	set_aux;

VAR_SET var_set_aux;

/*---------------------------------------------------------------------
Variaveis globais usadas na geracao dos descritores */

int	Quantos,	/* numero de mutantes gerados */
	Seq[NOPERADORES];	/* numero sequencial do mutante p/ 1 op */

long	InicFunc;	/* offset do inicio da funcao */

int	InicNome,	/* offset do nome da funcao */
	TamNome,	/* tamanho do nome da funcao */
	TamDcl,		/* tamanho da declaracao de parametros da funcao */
	TamCab,		/* tamanho do cabecalho da funcao */
	TamFunc;	/* tamanho total da funcao */

char	NameFunc[TAMLEX+1]; /* nome da funcao */

char	buf_descritor[2048]; /* buffer usado para montar o descritor */

int	descr_size;	/* keep track of the descriptor size */

int	g_calling, 
	g_called,
	g_unit,
	g_restrict;

char	*g_funcoes, *g_conexoes;

opmuta(DirLI, ArqLI, DirFonte, ArqFonte, Funcoes, Conexoes, Restrict)
char	DirLI[], ArqLI[], DirFonte[], ArqFonte[], Funcoes[], Conexoes[];
{
int	i, j;
unsigned long	t, fhash(), gera_rand();

   if ((fp_li = abrearq(DirLI, ArqLI, SUFIXO_NLI, 0)) == NULL)
	return 0;

   if ((fp_fonte = abrearq(DirFonte, ArqFonte, "", 0)) == NULL)
   {
	l1:
	fecharq(fp_li);
	return 0;
   }

   if (load_list_func(monta_nome(DirLI, ArqLI, SUFIXO_FUNC)) == ERRO)
   {
	l2:
	fecharq(fp_fonte);
	goto l1;
   }

	

   for(i = 0; i < NOPERADORES; i++)
      {
	/*-------- guarda percentagem de mutantes a serem gerados --------*/
	j = g_tab_operador[i].percent;
 
	/*-------- gera semente para sorteio -----------------------------*/
	if (j > 0)
		  g_tab_operador[i].semente2 =
	          g_tab_operador[i].semente = gera_rand(fhash(ArqFonte, i , j));
	     else g_tab_operador[i].semente2 = g_tab_operador[i].semente = 0;
       }

   g_funcoes = Funcoes;
   g_conexoes = Conexoes;
   g_restrict = Restrict;
   if (tmp_con_create(monta_nome(DirLI, ArqLI, SUFIXO_TMP)) == ERRO)
	goto l2;

   gera_mutantes_fase1();
   gera_mutantes_fase2();

   libera_list_func();
   tmp_con_destroy();

   return Quantos;

}


/************************************************************************
GERA_MUTANTES:
	Rotina que inicia e gerencia a geracao dos mutantes

Autor: Delamaro

************************************************************************/
gera_mutantes_fase1()
{
int	i, j;
char	buf[40];

  inic_mparser();			/* inicializa analisador sintatico */
  posiciona(fp_li, 0l);
  if (carrega_li_glob(fp_li) == ERRO)	/* carrega parte global do prog. */
	goto fim;

  if (analisa_li_glob() == ERRO)	/* analisa parte global do programa */
	goto fim;


   while (carrega_li_func(fp_li) != ERRO)	/* carrega uma funca da LI */
   {
	if (analisa_li_func(&InicFunc, &InicNome, &TamDcl, &TamCab ,
						&TamFunc, NameFunc) == ERRO)
		break;

        g_calling = calling_funcao(g_conexoes, NameFunc);
        g_called = called_funcao(g_conexoes, NULL, NameFunc);
	g_unit = unit_funcao(g_funcoes, NameFunc);

	if (g_calling || g_called || g_unit)
	   gera_fase1();
	
	if (analisa_li_glob() == ERRO)
		break;	
   }
   FimGeraFase1();
   fim:
   fim_parser();
}


gera_mutantes_fase2()
{
int     i, j;
char    buf[40];

  inic_mparser();                       /* inicializa analisador sintatico */
  posiciona(fp_li, 0l);
  if (carrega_li_glob(fp_li) == ERRO)   /* carrega parte global do prog. */
        goto fim;

  if (analisa_li_glob() == ERRO)        /* analisa parte global do programa */
        goto fim;


   Quantos = 0;

   while (carrega_li_func(fp_li) != ERRO)       /* carrega uma funca da LI */
      {
        if (analisa_li_func(&InicFunc, &InicNome, &TamDcl, &TamCab ,
                                                &TamFunc, NameFunc) == ERRO)
                break;

        g_calling = calling_funcao(g_conexoes, NameFunc);
	g_called = called_funcao(g_conexoes, NULL, NameFunc);
	g_unit = unit_funcao(g_funcoes, NameFunc);

	if (g_calling || g_called || g_unit)
	{
	   InicGeraFuncFase2();
           gera_fase2();
	   if (g_unit)
		u_strp(); /* implementa strp ssdl e srsr */
	}

        if (analisa_li_glob() == ERRO)
                break;
      }
   FimGeraFase2();
   fim:
   fim_parser();
}


/****************************************************************************
InicGeraFunc:
	Coloca na saida, dados sobre a funcao que vai ser alterada

Variaveis globais:
InicFunc:	offset do inicio da funcao
InicNome:	offset do inicio do nome da funcao (somado a InicFunc)
TamNome:	tamanho do nome da funcao
TamDcl:		tamanho da declaracao da funcao (nome, parametros)
TamCab:		tamanho do cabecalho da funcao 
TamFunc:	tamanho total da funcao
NameFunc:	nome da funcao
****************************************************************************/
GeraFuncFase1(int evoid, char *param)
{
   printf("%ld\t%d\t%d\t%d\t%d\t%d\t%s %s\n", InicFunc, 
		InicNome, TamDcl, TamCab,TamFunc, evoid, NameFunc, param);
}

InicGeraFuncFase2()
{
   memset(Seq, 0, sizeof(Seq));

}


/***************************************************************************
GO_DESCRITOR_FASE1:
	Monta um dsecritor de  coneccao

***************************************************************************/
go_descritor_fase1(func, offset, seq, compr, no, str)
char	*str;
int	compr, seq, no;
long	offset;
char	*func;
{
long	l;
LI_SIMBOLO	*sym;

   sym = get_list_func(func);
   if (sym == NULL)
	return;

   l = sym->inicio;

   tmp_con_add(l, seq, InicFunc, offset, compr, no, str);

/*   printf("%ld\n%d\n%ld\n%d\n%s\n", l, seq, offset, compr,
						   str); */
}



/************************************************************************
SORTEIA_MUTANTE:
	De acordo com a percentagem de geracao, sorteia o mutante
para ser ou nao gerado 
************************************************************************/
sorteia_grupo2(func, op, seq)
int	op, seq;
char    *func;
{
long    l;
LI_SIMBOLO      *sym;

   if ( called_funcao(g_conexoes, NameFunc, func))
   {
     sym = get_list_func(func);
     if (sym != NULL)
        l = sym->inicio;
     else
        l = -1;

     if ((! (g_restrict && l < 0)) && descr_size > 0  && sorteio(op))
     {
	printf("%ld\t%ld\n", InicFunc, l);
	printf("%d\t%d\t%d\n", 
			op, -1, seq); 
	fwrite(buf_descritor, descr_size, 1, stdout);
	printf("-1\n");
	Quantos++;
     }
   }
   buf_descritor[0] = '\0';
   descr_size = 0;
}

sorteia_grupo1(op, seq)
int     op, seq;
{
long    called, offset, calling;
int	seq1, size, i, j, no;
LI_SIMBOLO      *sym;

   i = tmp_con_first(InicFunc, (long) 0, 0);   
   if (i == ERRO)
	return;
   j = tmp_con_get_nth(i++, &called, &seq1, &calling, 
					&offset, &size, &no, bufmen);
   while (descr_size > 0 && j != ERRO && called == InicFunc)
   {
     if (called != calling && sorteio(op))
     {
        printf("%ld\t%ld\n", calling, InicFunc);
        printf("%d\t%d\t%d\n",
                        op, seq1, seq);
	printf("%d\t%ld\t%d\t%d\n%s\n", CONNECT_DESCR, offset, 
							size, no, bufmen); 
        fwrite(buf_descritor, descr_size, 1, stdout);
        printf("-1\n");
        Quantos++;
     }
     j = tmp_con_get_nth(i++, &called, &seq1, &calling, 
					&offset, &size, &no, bufmen);
   }
   buf_descritor[0] = '\0';
   descr_size = 0;
}

sorteia_grupoU(op, seq)
int	op, seq;
{
long    l;
LI_SIMBOLO      *sym;


     if (descr_size > 0  && sorteio(op))
     {
	printf("%ld\t%ld\n", InicFunc, -1);
	printf("%d\t%d\t%d\n", 
			op, -1, seq); 
	fwrite(buf_descritor, descr_size, 1, stdout);
	printf("-1\n");
	Quantos++;
     }
     buf_descritor[0] = '\0';
     descr_size = 0;
} 


FimGeraFase1()
{
  printf("-1\n");
}


FimGeraFase2()
{
  printf("-1\n");
}


/***************************************************************************
ADD_DESCRITOR:
        Monta um dsecritor de mutacao

***************************************************************************/
add_descritor(offset, compr, no, str)
char    *str;
int     compr, no;
long    offset;
{
int     i;
   i =  descr_size;
   sprintf(&buf_descritor[i], "%d\t%ld\t%d\t%d\n%s\n", OPER_DESCR,
						offset, compr, no, str);
   i += strlen(&buf_descritor[i]);

   buf_descritor[i++] = '\0';
   buf_descritor[i++] = '\n';
   descr_size = i;
}



