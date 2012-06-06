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


#include <stdio.h>
#include <string.h>

#include "li.h"

/* Declaracao de Variaveis Globais */

struct chave tab_chave[] = { /* tabela de palavras chave */
#include	"chave.h"
 };


static FILE * arqfonte;       /* contem ponteiro para arquivo fonte */
int ch;                /* ultimo caracter lido */
struct tableout saida;        /* ultimo token lido */

int   linha;          /* linha do caracter lido */
long  offset;         /* posicao do caracter lido */
long  ini_token;      /* posicao do 1o.token do item sintatico */
long  comp_token;     /* comprimento do item sintatico */
long  comp_ant;	      /* guarda ultimo comprimento */
long  lin_token;      /* linha do 1o.token do item sintatico */

static int	tamanho; /* tamanho do simbolo lido */
		


static	char	buf_dev[10];	/* pilha de caracterees devolvidos */
static	int	topo_dev;	/* topo desta pilha */

extern	TABSIMBOLO	tab_tipos, tab_vars;
static  SIMBOLO	*onde;

extern int	loc;

char	*bufyy;
int	ap_char;

/*******************************************************************/
/* int low_yylex()                                                 */
/* Autor: Marcio Delamaro 					   */
/* Versao: 2.0                                                     */
/*                                                                 */
/* Funcao: e' um analisador lexico generico dirigido               */
/*                                                                 */
/* Entradas: nenhuma                                               */
/*                                                                 */
/* Saida:    inteiro identificandor do tipo do token               */
/*                                                                 */
/*******************************************************************/

void low_yylex()
{
int	i,aninha,base;	/* i -- contador auxiliar para manipular string
			   aninha -- conta numero de aninhamentos de comentarios
			   base -- guarda a base de um numero lido 
				(octal, decimal, hexadecimal */
int	nivel;

char	c;	/* c -- variavel auxiliar para guardar qual o proximo caractere a ser procurado
			   num comentario aninhado */

void	pegaseqesc();




s1:
while (isspace(ch))	/* elimina brancos */
   peg_ch();

tamanho = 0;
saida.inicio = offset;
saida.linha = linha;
saida.label[0] = saida.classe[0] = '\0';

/*--------------------------------------------------------------
Verifica se eh fim de arquivo
---------------------------------------------------------------*/
if (ch == EOF)
{
   strcpy(saida.classe, "EOF");
}
else

/* ---------------------------------------------------------------
Se caractere for letra entao eh identificador ou palavra reservada
-----------------------------------------------------------------*/
if (isalpha(ch) || ch == '_')
{
   do {
	saida.label[tamanho++] = ch; /* vai lendo caractere */
	peg_ch();
      } while ((isalpha(ch) || isdigit(ch) || ch == '_') && tamanho < TAMLEX);
   saida.label[tamanho] = '\0';
   if (pesq_tab(saida.label) != 0)	/* verifica se eh simbolo reservado*/
	strcpy(saida.classe, saida.label);
   else
   {	/* verifica se eh tipo definido pelo usuario ou se identificador */
	find_simbolo(saida.label, &tab_tipos, &nivel, &onde);
	if (nivel >= 0)
	   strcpy(saida.classe, "TIPDEF");
	else
	   strcpy(saida.classe, "IDENT");
   }
}
else

/*------------------------------------------------------------------
se caractere eh digito, simbolo eh numero
--------------------------------------------------------------------*/
if (isdigit(ch))
{
strcpy(saida.classe, "INT_CONST");
   if (ch == '0')
   {
	base = 8;
	saida.label[tamanho++] = ch;
	peg_ch();
	if (ch == 'x' || ch == 'X')
	{
	   saida.label[tamanho++] = ch;
	   peg_ch();
	   base = 16;
	   if (! isxdigit(ch))
		goto erro;
	}
   }
   else
      base = 10;

   while (edigito(base, ch))	/* vai lendo caractere enquanto eh numero */
   {
	saida.label[tamanho++] = ch;
	peg_ch();
   }

   if (isdigit(ch))
   {			/* verifica se existe digit decimal dentro de constante
			   octal */
	do {
	      peg_ch();
	   } while (isdigit(ch));
	goto erro;
   }
   else 
   if (ch == '.' && base != 16)
	goto s2;
   else
   if (ch == 'l' || ch == 'L' || ch == 'u' || ch == 'U' )
   {
	saida.label[tamanho++] = ch;
	peg_ch();
   }
   
}
else

/*----------------------------------------------------------------------
Pega constante ponto flutuante
-----------------------------------------------------------------------*/
if (ch == '.')
{
   s2:
   strcpy(saida.classe, "FLOAT_CONST");
   saida.label[tamanho++] = ch;
   peg_ch();

   if ( tamanho == 1 && !isdigit(ch))
	goto s3;

   while (isdigit(ch))
   {
	saida.label[tamanho++] = ch;
	peg_ch();
   }

   if (ch == 'e' || ch == 'E')
   {
      saida.label[tamanho++] = ch;
      peg_ch();
      if (ch == '+' || ch == '-')
      {
	  saida.label[tamanho++] = ch;
	  peg_ch();
       }
      while (isdigit(ch))
      {
	  saida.label[tamanho++] = ch;
	  peg_ch();
      }
   }
   if (ch == 'f' || ch == 'F' )
   {
	saida.label[tamanho++] = ch;
	peg_ch();
   }

}
else

/*---------------------------------------------------------------------
pega constante do tipo 'a' ou '\n' ou '\567'
----------------------------------------------------------------------*/
if (ch == '\'')
{
   strcpy(saida.classe, "CHAR_CONST");
   saida.label[tamanho++] = ch;
   peg_ch();

   if (ch == '\\')	/* eh sequencia de escape tipo \n ou \567 */
   {
	saida.label[tamanho++] = ch;
	pegaseqesc();
   }
   else
	if (ch == '\'')	/* nao podem vir dois apostrofos seguidos */
	   goto erro;
	else
	{
	   saida.label[tamanho++] = ch;
	   peg_ch();
	}

   if (ch != '\'')	/* tem que vir um fecha apostrofo */
	goto erro;
   else
   {
	saida.label[tamanho++] = ch;
	peg_ch();
   }
}
else

/*----------------------------------------------------------------
se caractere lido eh " entao le string
-----------------------------------------------------------------*/
if (ch == '"')
{
   strcpy(saida.classe, "STRING");

   saida.label[tamanho++] = ch;
   peg_ch();
   while (ch != '"' && tamanho < TAMLEX - 1)
   {
	saida.label[tamanho++] = ch;
	if (ch == '\\')	/* eh uma sequencia de escape dentro do string */
        {
	    pegaseqesc();
	}
	else
	   peg_ch();
   }
   if (tamanho < TAMLEX - 1)
   {
	saida.label[tamanho++] = ch;
	peg_ch();
    }
    else goto erro;

}
else

/*---------------------------------------------------------------------
se caractere for um / PODE ser um comentario
----------------------------------------------------------------------*/
if (ch == '/')
{
   peg_ch();
   if (ch != '*')	/* nao eh abre comantario */
   {
	dev_ch();
	ch = '/';
	goto s3;
   }
   aninha = 1;
   do {			/* consome tudo entre comentario */
	peg_ch();
	do {
		while (ch != '*' && ch != '/')
		{
		   peg_ch();
		   if (ch == EOF)
			goto erro;
		}

		if (ch == '/')
		   c = '*';
		else
		   c = '/';
		peg_ch();
	   } while (ch != c);
	if (ch == '/')
	   aninha--;
	else
	   aninha++;
      } while (aninha > 0);
   peg_ch();
   goto s1;	/* volta para ler proximo item lexico */
}
else

/*--------------------------------------------------------------------------
Tenta diretiva se aparece #
-----------------------------------------------------------------------*/

if (ch == '#')
{
   saida.label[tamanho++] = ch;
   strcpy(saida.classe, "DIRETIVA");
   peg_ch();
}

else
/*----------------------------------------------------------------
se nao eh nada, tenta simbolo reservado
----------------------------------------------------------------*/
{
   s3:
   saida.label[tamanho++] = ch;
   peg_ch();

   while (ch != EOF && ! isspace(ch) && ! isalpha(ch) && ch != '_' &&
	  ! isdigit(ch) && tamanho < 3)
   {
	saida.label[tamanho++] = ch;
	peg_ch();
   }
   saida.label[tamanho] = '\0';
   do
   {
	i = pesq_tab(saida.label);
	if (i <= 0 && tamanho-- > 1)
	{		/* se nao achou, diminui o tamanho do simbolo e procura*/
	   dev_ch();
	   ch = saida.label[tamanho];
	   saida.label[tamanho] = '\0';
	}
    } while (i <= 0 && tamanho > 0);
    if (i > 0)	/* achou simbolo */
	strcpy(saida.classe, saida.label);
    else
    {		/* nao achou simbolo */
    	erro:
	tamanho = 0;
	strcpy(saida.classe, "FALHE");
    }
}

saida.label[tamanho] = '\0';
saida.comp = tamanho;
calcula_posicao();
/* printf("\n%s %s", saida.label, saida.classe);   */

}

/*******************************************************************/
/* int yylex()                                                     */
/* Autor: Marcio Delamaro                                          */
/* Versao:                                                         */
/* Data: 07/02/94                                                  */
/*                                                                 */
/* Funcao: e' um analisador lexico que trata das diretivas de      */
/* pre-processamento                                               */
/*                                                                 */
/* Entradas: nenhuma                                               */
/*                                                                 */
/* Saida:    inteiro identificandor do tipo do token               */
/*                                                                 */
/* Funcoes Chamadas: low_yylex				 	   */
/*                                                                 */
/*                                                                 */
/*******************************************************************/
void yylex()
{
static int	linha_atual = -1;

   low_yylex();
   if (saida.linha > linha_atual)
	loc++;
   linha_atual = saida.linha;		/* guarda linha corrente */
   while (strcmp(saida.classe, "DIRETIVA") == 0)
   {
	do {
		low_yylex();	/* consome todos os tokens da mesma linha */
	   } while (! (saida.linha > linha_atual));
	linha_atual = saida.linha;
   }

   /* printf("\nClasse: %s  Label: %s   Linha: %d", saida.classe, saida.label,
						 saida.linha); */
}



/**********************************************************************/
/* void	pegaseqesc()                                                 */
/* Autor: Marcio Delamaro                                             */
/* Versao: 1.0                                                        */
/* Data: 11/03/94                                                     */
/*                                                                    */
/* Funcao: 						              */
/*	Rotina auxiliar do yylex(). Pega sequencia de escape, do tipo */
/* \n \r ou do tipo \456                         		      */
/*                                                                    */
/* Entradas: nenhuma                                                  */
/*                                                                    */
/* Saida: nenhuma                                                     */
/*                                                                    */
/* Variaveis Externas: ch,			                      */
/*                                                                    */
/**********************************************************************/
void pegaseqesc()
{
int	i;

peg_ch();			/* le proximo caractere */
if (edigito(8,ch))
{	/* se for um digito, pega valor numerico */
   i = 0;
   do {
	saida.label[tamanho++] = ch;
	i++;
	peg_ch();		/* pega proximo caractere */
     } while (edigito(8,ch) && i < 3);	/* repete enquanto for digito e numero 
					de digitos <= 3 */
}
else 
{
   saida.label[tamanho++] = ch;
   peg_ch();
}


}

/**********************************************************************/
/* int	edigito(base, dig)                                            */
/* Autor: Marcio Delamaro                                             */
/* Versao: 1.0                                                        */
/* Data: 10/03/94                                                     */
/*                                                                    */
/* Funcao:                                                            */
/*	verifica se um caractere eh digito na base 10, 8 ou 16	      */
/*                                                                    */
/* Entradas:                                                          */
/*	base: base (10, 8 ou 16)				      */
/*	dig:  caractere a ser analisado				      */
/*                                                                    */
/* Saida: 0 (nao eh um digito) ou 1 (eh digito)			      */
/*                                                                    */
/* Variaveis Externas:                                                */
/*                                                                    */
/**********************************************************************/
edigito(base, dig)
int	base, dig;
{
   switch(base)
   {
	case 10:
	   return isdigit(dig);
	case 16:
	   return isxdigit(dig);
	case 8:
	   return (isdigit(dig) && dig < '8');
	default:
	   return 0;
   }
}





/**********************************************************************/
/* int peg_ch()                                                       */
/* Autor: Marcos L. Chaim                                             */
/* Versao: 1.0                                                        */
/* Data: 18/09/89                                                     */
/*                                                                    */
/* Funcao: pega o proximo caracter a ser analisado pelo AL.           */
/*                                                                    */
/* Entradas: nenhuma                                                  */
/*                                                                    */
/* Saida: nenhuma                                                     */
/*                                                                    */
/* Variaveis Externas: ch, comprimemto, arqfonte.                     */
/*                                                                    */
/**********************************************************************/

int peg_ch()
{

 if (arqfonte == NULL)
 {			/* analise estah sendo feita na memoria */
    mpeg_ch();
 }
 else
 {
      if (topo_dev < 0)
         ch = getc(arqfonte);
      else
	 ch = buf_dev[topo_dev--];
 }

 if (ch == '\n')         /* incrementa numero da linha */
    ++linha;

  ++offset;               /* incrementa deslocamento */
 return ch;
}


/**********************************************************************/
/* void dev_ch()                                                      */
/* Autor: Marcos L. Chaim                                             */
/* Versao: 1.0                                                        */
/* Data: 18/09/89                                                     */
/*                                                                    */
/* Funcao: devolve o ultimo caracter analisado pelo AL.               */
/*                                                                    */
/* Entradas: nenhuma                                                  */
/*                                                                    */
/* Saida: nenhuma                                                     */
/*                                                                    */
/* Variaveis Externas: comprimemto, arqfonte, ch.                     */
/*                                                                    */
/**********************************************************************/

dev_ch()
{

 if (arqfonte == NULL)
 {
    mdev_ch();
 }
 else
 {
    buf_dev[++topo_dev] = ch;
 }

 --offset;               /* */

 if (ch == '\n')
	linha--;

return ch;
}


/**********************************************************************/
/* void mpeg_ch()                                                     */
/* Autor: Delamaro	                                              */
/* Versao: 1.0                                                        */
/* Data: 18/09/89                                                     */
/*                                                                    */
/* Funcao: pega o proximo caracter a ser analisado pelo AL.           */
/*                                                                    */
/* Entradas: nenhuma                                                  */
/*                                                                    */
/* Saida: nenhuma                                                     */
/*                                                                    */
/* Variaveis Externas: 	buffy, ap_char		.                     */
/*                                                                    */
/**********************************************************************/

mpeg_ch()
{
   ch = bufyy[ap_char++];
   return (ch == '\0') ? ch = EOF: ch;
}

/**********************************************************************/
/* void mdev_ch()                                                     */
/* Autor: Delamro	                                              */
/* Versao: 1.0                                                        */
/* Data: 18/09/89                                                     */
/*                                                                    */
/* Funcao: devolve o ultimo caracter analisado pelo AL.               */
/*                                                                    */
/* Entradas: nenhuma                                                  */
/*                                                                    */
/* Saida: nenhuma                                                     */
/*                                                                    */
/* Variaveis Externas:  ch, ap_char	        	              */
/*                                                                    */
/**********************************************************************/

mdev_ch()
{
   ap_char--;
   return ch;
}




/************************************************************************
INIC_YYLEX
	Faz inicializacao do analizador lexico, lendo de um arquivo

PARAMETROS:
	DIR:	diretorio do arquivo a ser analisado
	ARQ:	nome do arquivo a ser analisado

*************************************************************************/
inic_yylex(dir, arq)
char	*dir, *arq;
{
FILE	*abrearq();

  linha = 1;          /* linha do caracter lido pelo A.L. */
  offset = 0;        /* posicao do caracter lido pelo A.L. */
  ini_token = 0;     /* posicao do 1o.token do item sintatico */
  comp_token = 0;     /* comprimento do item sintatico */
  lin_token = 0;      /* linha do 1o.token do item sintatico */
  topo_dev = -1;	/* topo da pilha de caracteres devolvidos */
  ch = ' ';

  arqfonte = abrearq(dir, arq, "", 0);

  if (arqfonte == NULL)
  {
	return ERRO;
  }

  return OK;
}


/************************************************************************
INIC_MYYLEX
	Faz inicializacao do analizador lexico, analisando buffer em
memoria

PARAMETROS:
	BUF:	endereco do buffer a ser analisado

*************************************************************************/

inic_myylex(buf)
char	*buf;
{
 linha = 1;          /* linha do caracter lido pelo A.L. */
 offset = 0;        /* posicao do caracter lido pelo A.L. */
 ini_token = 0;     /* posicao do 1o.token do item sintatico */
 comp_token = 0;     /* comprimento do item sintatico */
 lin_token = 0;      /* linha do 1o.token do item sintatico */
 bufyy = buf;		/* guarda endereco do buffer */
 arqfonte = NULL;
 ch = ' ';
 ap_char = 0;
 return OK;
}




fim_analex()
{
int	i;

   if (arqfonte != NULL)
      fecharq(arqfonte);
   return OK;
}

/**********************************************************************/
/* int pesq_tab(char *, int)                                          */
/* Autor: Marcos L. Chaim                                             */
/* Versao: 1.1                                                        */
/* Data: 19/09/89                                                     */
/*                                                                    */
/* Entradas: apontador para a palavra a ser pesquisada e o numero de  */
/*           elementos da tabela de palavras reservadas.              */
/*                                                                    */
/* Saida:    inteiro indicando verdadeiro ou falso.                   */
/*                                                                    */
/* Variaveis Globais: tab_chave,n_chave;                              */
/*                                                                    */
/**********************************************************************/

int pesq_tab(pal)
char *pal;
{
 int n_chaves = LEN(tab_chave);
 int inicio,fim,meio,cond;
 inicio=0;
 fim=n_chaves-1;
 while(inicio<=fim)
     {
      meio=(inicio+fim)/2;
      if((cond=strcmp(pal,tab_chave[meio].pal_chave))<0)
      fim=meio-1;
      else if(cond>0)
           inicio=meio+1;
           else
           return(1);
     }
 return(0);
}



/**********************************************************************
FORCE_EOF:
	In case of error, makes input pointer to point at the
endo of the input.

**********************************************************************/
force_eof()
{

   if (arqfonte == NULL)
     ap_char = strlen(bufyy);
   else
     posifim(arqfonte);
}

/****************************************************************
REANALEX:
	Verifies whether a identifier is a variable, a enum constant
or none.
*****************************************************************/

reanalex()
{
int	nivel;

   find_simbolo(saida.label, &tab_vars, &nivel, &onde);
   if (nivel >= 0)
   {
	strcpy(saida.classe, "VARIABLE");
	return;
   }
   find_enum_const(saida.label, &tab_tipos, &nivel, &onde);
   if (nivel >= 0)
   {
	strcpy(saida.classe, "ENUM_CONST");
   }
}


/******************************************************************
BACK_TRACK:
	Return the input pointer to a specifieid point.
 
Parameters:
	struct tableout last: token to return to.
*****************************************************************/
back_track(last)
struct tableout	*last;
{

   offset = last->inicio - 1;
   linha = last->linha;

   if ( arqfonte == NULL)
	ap_char = offset;
   else
	posiciona(arqfonte, offset);

   topo_dev = -1;
   peg_ch();
   yylex();
}

