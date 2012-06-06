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


/*****************************************************************************
				P T E S T E . H

	Definicoes e estruturas para manipular arquivo de teste.

Autor: Delamaro
Data: 02-09-94
Documentacao: 24-10-94 (Mirian)
*****************************************************************************/


#define		SUFIXO_PTESTE	".PTM" /* extensao padrao de arquivo de teste */


/*----------------- Identificador do arquivo de teste --------------*/

#define		IDENT_PTESTE	"PROTEUM - PROGRAM TEST FILE"


/*----------------- Tipos de teste ----------------------------------*/
#define		T_TESTE		1
#define		T_RESEARCH	2

#define		STR_TESTE	"Test"
#define		STR_RESEARCH	"Research"


/*--------------------- Tipos de anomalias de fluxo de dados -----------------*/
#define		NONE		0
#define		UR		1
#define		DU		2
#define		DD		4

#define		STR_NONE	"<N O  A N O M A L I E S>"
#define		STR_UR		"UR"
#define		STR_DU		"DU"
#define		STR_DD		"DD"


/*--------------------- Indicador de todas-funcoes -------------------*/

#define		STR_ALLFUNC	"<A L L   F U N C T I O N S>"


/*----------------------- Estrutura do arquivo de teste ---------------*/

typedef struct {
	char	nome[NOME_LENGTH+1];	/* nome do teste */
	char	dir[NOME_LENGTH+1]; 	/* diretorio do arquivo fonte, arquivo*/
 					/* executavel e arq. de teste */
	char	fonte[NOME_LENGTH+1];   /* nome do arq. fonte  do teste */
	char	exec[1024];		/* nome do arq. executavel do teste */
	char	comp[3*NOME_LENGTH+1]; 	/* comando de compilacao para criar */
					/* o executavel a partir do fonte*/
	char	fonte_expa[NOME_LENGTH+1];/* arquivo fonte expandido */
	int	tipo;			/* tipo do teste = test ou research */
	int	anomalias;	/* anomalias de fluxo de dados a serem */
				/* procuradas */
	int	todas;		/* flag que dia se todas as funcoes do */
				/*programa serao testadas */
	char	funcoes[1024];	/* buffer que contem o nome das funcoes do */
				/* programa que devem ser testadas */
		} CAB_PTESTE;



