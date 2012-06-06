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


#include 	<stdio.h>



#include	"li.h"


int	flg_gera;	/* flag que indica se deve ser gerada LI ou nao */

int	error_sintatic; /* flag shows if there was a sintatic error in parser */


FILE	*arqli,		/* arquivo onde deve ser gravada LI */
	*arqfun, 	/* list of functions */
	*arqgfc, 	/* file with the control flow graphs and def use data */
	*arqcall;	/* arquivo onde gravar CALL-GRAPH */

TABSIMBOLO	tab_vars,   /* tabelas de simbolos (tipos e variaveis) */
		tab_tipos;



/*------------------- Variaveis externas ------------------------------*/

GOTOLABEL	*lista_resul,	/* lista das funcoes de um modulo */
		*aux_lista;

extern	int	linha;		/* linha corrente do A.L. */



/***************************************************************************
INIC_PARSER
	Faz a inicializacao do analisador sintatico.

PARAMETROS:
	DIR:	Diretorio onde esta o arquivo fonte
	ARQ:	nome do arquivo a ser procurado (NULL se eh para analisar
		string que estah na memoria)
	BUF:	endereco do string a ser analisado (NULL se eh para analisar
		de arquivo)
	DIRLI:	diretorio onde criar arquivo LI
	ARLI:	nome do arquivo onde deve gravar a LI (NULL se nao deve
		gerar LI)
***************************************************************************/
inic_parser(dir, arq, buf, dirli, arli, arcall)
char	*arq, *buf, *dirli, *arli, *arcall;
{

   flg_gera = (arli != NULL);
   if (arq == NULL)
   {
      inic_myylex(buf);
   }
   else
   {
      if (inic_yylex(dir, arq) == ERRO)
	 return ERRO;
   }

   if (flg_gera)
   {
      arqli = criarq(dirli, arli, SUFIXO_LI);
      arqcall = criarq(dirli, arli, SUFIXO_CALL);
      arqfun = criarq(dirli, arli, SUFIXO_FUNC);
      arqgfc = criarq(dirli, arli, SUFIXO_GFC);

      if (arqli == NULL || arqcall == NULL || arqfun == NULL || arqgfc == NULL)
      {
	 fecharq(arqfun);
	 fecharq(arqcall);
	 fecharq(arqli);
	 fecharq(arqgfc);
	 fim_analex();
	 return ERRO;
      }
   }


   yylex();
   return OK;
}


fim_parser()
{
   fim_analex();
   if (arqli != NULL)
   {
      fecharq(arqli);
      fecharq(arqcall);
      fecharq(arqfun);
      fecharq(arqgfc);
   }
   arqli = NULL;
   arqcall = NULL;
   arqfun = NULL;
   arqgfc = NULL;

/* libera resultado criado pelas rotinas semanticas */

   clear_list(lista_resul);
   lista_resul = NULL;		/* libera lista com nome das funcoes que
				   pertencem ao arquivo */

   esvazia_tab(&tab_tipos, 0);	/*esvazia tabelas de simbolos */
   esvazia_tab(&tab_vars, 0);
   return OK;
}

/******************************************************************
PARSER:
	Analisa um programa, uma declaracao ou uma expressao

Parameters:
	enum GOAL goal:	name of the structure to be analissed 

********************************************************************/
parser(goal)
enum GOAL goal;
{

   error_sintatic = FALSE;
   switch (goal)
   {
	case PROGRAM:
	    translation_unit();
	    break;
	case GLOBAL_DECLARATION:
	     global_declaration();
	     break;
 	case FUNC_DECLARATION:
	     global_function();
	     break;
	case PARAM_DECLARATION:
	     function_definition();
	     break;
	case BLOCK_DECLARATION:
	     block_declaration();
	     break;
	case EXPRESSION:
	     expression();
	     break;
        default:
	     return FALSE;
     }

     return ! error_sintatic;
}



/*******************************************************************
INIC_GET_FUNC:
	Faz analise sintatica do modulo para pegar nome das funcoes
do modulo.
Parametros:
	dir: diretorio onde esta o fonte e os includes
	fonte: nome do fonte
	prepro: nome do arquivo preprocessado

*********************************************************************/
inic_get_func(dir, fonte, prepro)
char	*dir, *fonte, *prepro;
{
int	i;
char *bufexp;
char	*monta_nome();

 bufexp = fonte;
 if (prepro != NULL)
   if (cria_arq_cpp(dir, fonte, prepro) == ERRO)
	return ERRO;
	else bufexp = prepro;


 if (inic_parser(dir, bufexp, NULL, NULL, NULL) == ERRO) /*inicializa parser */
    return ERRO;


 if ( parser(PROGRAM) == TRUE )
   {
	aux_lista = lista_resul;
/*	unlink(monta_nome(dir, bufexp, "")); */
	return OK;
   }
     else
   {
/*	unlink(monta_nome(dir, bufexp, "")); */
	msg("Sintatic Error in Source File");
	fim_parser();
	return ERRO;
   }

}

/**************************************************************************
GET_FUNC:
	devolve o nome de uma funcao encontrada no fonte
Autor: Delamaro
***************************************************************************/

char	*get_func()
{
char	*p;

   if (aux_lista == NULL)
   {
	fim_parser();
	return NULL;
   }
   p = aux_lista->name;
   aux_lista = aux_lista->next;
   return p;
}


char	*get_func_loc(x, y)
int	*x, *y;
{
char	*p;

   if (aux_lista == NULL)
   {
	fim_parser();
	return NULL;
   }
   p = aux_lista->name;
   *x = aux_lista->id;
   *y = aux_lista->connect;
   aux_lista = aux_lista->next;
   return p;
}

/*******************************************************************
CRIA_LI:
	Faz analise sintatica do modulo e cria arquivo LI.
Parametros:
	dir: diretorio onde esta o fonte e os includes
	fonte: nome do fonte
	prepro: nome do arquivo criado pelo pre-processador
	nome: nome do arquivo li a criar

*********************************************************************/
cria_li(dir, fonte, prepro, nome)
char	*dir, *fonte, *nome, *prepro;
{
int	i;
char	buf[NOME_LENGTH+10];
char	*arqanalise = fonte;


 if (prepro != NULL )
   if ( cria_arq_cpp(dir, fonte, prepro) == ERRO)
	return ERRO;
	else arqanalise = prepro;


 if (inic_parser(dir, arqanalise, NULL, dir, nome) == ERRO)
   return ERRO;

 if (parser(PROGRAM) == TRUE)
   {
	aux_lista = lista_resul;
/*	fim_parser(); */
	return OK;
   }
 else
     {
	sprintf(buf, "Sintatic Error at Line %d ", linha);
	msg(buf);
	fim_parser();
	return ERRO;
     }

}

/*******************************************************************
MPARSER:
	Faz analise sintatica de um buffer
Parametros:
	enum GOAL goal:     objetivo a ser alcancado na analise
	char buf: 	buffer a ser analisado

*********************************************************************/
mparser(goal, buf)
enum GOAL goal;
char	*buf;
{
   inic_myylex(buf);
   yylex();
   return parser(goal);
}

inic_mparser()
{
   flg_gera = FALSE;
   arqli = arqcall = arqfun = arqgfc = NULL;
}

get_func_type(buf, tipo, ptar)
char	*buf, *tipo, *ptar;
{
SIMBOLO	ds;

   memset(&ds, 0, sizeof(ds));
   inic_myylex(buf);
   yylex();
   declaration_specifiers(&ds);

   tipo_to_str(&ds, tipo);
   strcpy(ptar, ds.ptar);
   return OK;
}



/**************************************************************************
CRIA_ARQ_CPP:
	Execute pre-processor to expand source file

Parameters:
	dir: directory were is the source file
	source: name of the source file
	name: name of the program pre-processed
**************************************************************************/
cria_arq_cpp(dir, source, name)
char	dir[], source[], name[];
{
char	*monta_nome();
static char bufaux[3*(NOME_LENGTH+1)], bufaux2[3*(NOME_LENGTH+1)];
char	aux[10], *p;

   unlink(p = monta_nome(dir, name, ""));
   strcpy(bufaux2, p);
   sprintf(bufaux, CPP, getenv(STR_DIR_HOME), monta_nome(dir, source, ""), bufaux2);

   get_str_exec(bufaux, aux, 0);

   if (! tem_arq(dir, name, ""))
   {
	msg("Error Executing C Pre-processor");
	return ERRO;
   }
   return OK;
}


   
/**************************************************************************
INSERT_LIST:
CLEAR_LIST:
	Insere/libera lista com as funcoes do modulo analisado
***************************************************************************/
insert_list(description, id, contconect, start)
char * description;
int id, contconect;
GOTOLABEL ** start;
{

 GOTOLABEL * name_aux, * new;



 new = (GOTOLABEL *) malloc(sizeof(GOTOLABEL));
 if(new == (GOTOLABEL *) NULL)
     {
	msg("Memory Overflow");
	return ;
      }

 /* Insere informacao */

 strcpy(new->name,description);
 new->id = id;
 new->connect = contconect;
 new->next = NULL;

 /* posiciona no final da lista */

 if(*start != NULL)
     {
     for(name_aux = *start; name_aux->next != NULL; name_aux = name_aux->next);
     name_aux->next = new;
     }
 else
     *start = new;
 return;
}




clear_list(list)
GOTOLABEL * list;
{
if(list != NULL)
	{
	clear_list(list->next);
	free(list);
	}
}



