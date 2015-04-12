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


/*************************************************************************
				D I S C O . H

	Estruturas e definicoes usadas por disco.c

Autor: Marcio Eduardo Delamaro
Data: 02-09-94
Documentacao: 24-10-94 (Mirian)
*************************************************************************/

#include <limits.h>

#define		MAX_ARQ		20	/*numero maximo de arquiovos */

/**
 * Structure that holds information regarding opened files.
 */
typedef	struct {
	char nome[NAME_MAX];
	char dir[PATH_MAX - NAME_MAX];
	char ext[NAME_MAX];
	FILE *fp;
} ARQUIVO;



int insarq(char *d, char *s, char *e, FILE *fp);
int delarq(FILE *fp);
char * nomearq(FILE * fp);
char * extarq(FILE * fp);
char * dirarq(FILE *fp);
FILE * abrearq(char *d, char *s, char *e, int tipo);
int fecharq(FILE *fp);
FILE * criarq(char *directory, char  *filename, char *extension);
int delearq(char d[], char s[], char e[]);
int learq(FILE *fp, char buf[], int len);
int gravarq(FILE *fp, char buf[], int len);
int posiciona(FILE *fp, long offset);
int posifim(FILE *fp);
FILE * criarqtemp(char *directory, char *prefix);
int maxfile();
