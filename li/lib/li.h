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


#ifndef		_li_h_

#define		_li_h_




#include	<lib/gerais.h>



#define		SUFIXO_LI	".nli"	/* extensao de arquivo li */
#define		SUFIXO_FONTE	".c"	/* extensao de arquivo fonte */
#define		SUFIXO_CALL	".cgr"  /* extnsao do arq de call graph */
#define		SUFIXO_FUNC	".fun"  /* arquivo c/ lista de funcoes */
#define		SUFIXO_GFC	".gfc"  /* arquivo c/ grafo de fluxo */

#define		CPP		"%s/proteuIMcpp %s %s" 
			         /* comando de pre-processa
							mento de arquivo C */

#define		EMPTY		0
#define		WLABEL		1
#define		NOEMPTY		2



/*************************************************************************
Defines e estruturas utilizadas pelo analisador lexico
*************************************************************************/


#define FINAL_STATE 1000
#define TAMLEX 255   /* */


struct chave {
              char *pal_chave;
             };

struct tableout {
		 char label[TAMLEX+1];
		 char classe[TAMLEX+1];
		 int  linha;        /* */
                 int  comp;          /* */
		 long inicio;        /* */
                };



/****************************************************************************
Estrutura onde sao colocadas os nomes das funcoes encontradas durante
a analise sintatica de um modulo
****************************************************************************/


typedef struct goto_label {
      char name[TAMLEX+1];
      int id, connect;
      struct goto_label * next;
      } GOTOLABEL;



/*************************************************************************
Definicoes utilizadas para manipular tabelas de simbolos
**************************************************************************/
 
typedef	struct SIMBOLO	/* um simbolo da tabela */
		{
		char	nome[TAMLEX+1];	/* nome do simbolo */
		char	tipo[TAMLEX+11]; /* tipo se for tipo definido */
		struct	{
		  	char	f_auto:1;
			char	f_register:1;
			char	f_static:1;
			char	f_extern:1;
			char	f_typedef:1;
			} storage;
		struct  {
			char	f_void:1;
			char	f_char:1;
			char	f_short:1;
			char	f_int:1;
			char	f_long:1;
			char	f_float:1;
			char	f_double:1;
			char	f_signed:1;
			char	f_unsigned:1;
			char	f_other:1;
			char	f_struct:1;
			char	f_enum:1;
			} specifier;
		struct {
			char	f_const:1;
			char	f_volatile:1;
			} qualifier;
		unsigned char		initialized;
		unsigned char	npar;		/* seq. number, if parameter */
		unsigned char	maxpar;	/* number of parameters, if function */

		char	ptar[20];	/* indicador de pointer/array */
		
		struct SIMBOLO	*campos;  /* apontador para campos estrutura */
		struct SIMBOLO  *pai; /* apontador para pai se for campo */
		struct SIMBOLO	*esq, *dir;
		} SIMBOLO;


			

typedef	struct	{	/* tabela de simbolos */
		  int	topo;
		  SIMBOLO	*vet[50]; /* numero de niveis da tabela */
		} TABSIMBOLO;

enum GOAL	{ PROGRAM, GLOBAL_DECLARATION, FUNC_DECLARATION,
		  PARAM_DECLARATION, BLOCK_DECLARATION, EXPRESSION};


#endif
