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
 				G E R A I S . H 
 
	Definicoes usadas por varios programas

Autor: Delamaro
Data: 30/09/94 

*************************************************************************/


#ifndef		_gerais_h_

#define		_gerais_h_

#include	<stdio.h>
#include	<unistd.h>
#include 	<stdlib.h>
#include	<string.h>

FILE	*popen();	/* define popen if stdio.h doesn't */


/* -------- Definicoes de constantes e macros gerais -----------*/

#ifndef		TRUE
#   define		TRUE	1
#   define		FALSE	0

#   define		OK	0
#   define		ERRO	-1

#else
#   define		XOK	0
#   define		ERRO    -1
#endif


#define		LEN(x)		sizeof(x)/sizeof(x[0])	/* numero de elementos
							de um vetor */

#define		MINI(x,y)	(x<=y)?x:y

#define		MAXI(x,y)	(x>=y)?x:y

/*--------------------- Constantes usadas por alguns modulos -----------*/

#define		SUFIXO_TMP	".TMP"

#define		SUFIXO_INS	"_inst.c"

#define		SUFIXO_PP	"_pp.c"

#define		STR_STDINPUT	"input"

#define         STR_STDOUTPUT	"output"

#define         STR_STDERROR	"error"


#define		IDENT_TMP	"PROTEUM/IM - TEMPORARY FILE"

#define		STR_DIR_HOME	"PROTEUMIMHOME"

/*			G E R A I S.H

/*-------------------- Constantes utilizadas por gerais.c ---------*/

#define		NOME_LENGTH	128	/* tamanho maximo do nome de um arquivo
					   ou diretorio */


#define		OFFSET0		64	/* tamanho do cabecalho de identifica
					   cao colocado nos arquivos de 
					    trabalho do proteum */

#define		MAXSET		512

typedef	struct {
		short int	n;
		short int	vet[MAXSET];
		short int	free;
		int		sizeofp;
		char		*p;
		} OSET;	/* definicao do tipo SET, onde cada elemento eh um 
				string*/




/******************* Tipos com tamanho pre definido *********************/
#define		CHAR		unsigned char	/* 1 byte */
#define		WORD		short int	/* 2 bytes */
#define		DWORD		long int	/* 4 bytes */


/********************* AUXILIAR PROGRAMS ****************************/
#define		PROG_RECINPUT	"recinput"
#define		PROG_HEADTAIL	"headtail"
#define         PROG_SEPARA     "splitarg"

#define		SHELL		"/bin/sh"


/* defines for SUNOS 4 */

#ifdef		SUNOS4

#define		SEEK_SET	0
#define		SEEK_CUR	1
#define		SEEK_END	2

#ifndef 	CLK_TCK
extern long sysconf(int);
#define		CLK_TCK	sysconf(_SC_CLK_TCK)
#endif

#endif


#include	"disco.h"   /* Estruturas e definicoes usadas por disco.c */ 
#include	"globais.h" /* definicoes utilizadas em globais.c */


/*************************************************************************
FUNCOES DEFINIDAS NO MODULO:
------------------------------------------------------------------------*/
void	set_new(OSET *);
int	set_new_inic(OSET *, char *);
int	set_add(OSET *, char *);
char	*set_get(OSET *, int);
int	set_inn(OSET *, char *);
void	set_del(OSET *, int);
int	set_dup(OSET *, OSET *);
void	set_free(OSET *);

#define	set_card(x)	(x->n)


char	*monta_nome(),
	*normaliza();

int	copy_file(FILE *, long, int, FILE *);

int	exec_extern(char *, int *, int);

long	gettimechild(void), gettimedad(void);
#endif
