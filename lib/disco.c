/* Copyright (C) 2012 -- Marcio Eduardo Delamaro and Jose Carlos Maldonado
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
*/


/*@MODULE###################################################################
				D I S C O . C
	This module implements functions for access of disk.

Include Files:
		"gerais.h"
		<sys/file.h>

Author: Marcio Eduardo Delamaro
Date: 02-09-94

Alteration	Responsible	Motive
-----------------------------------------
23-11-94 	Mirian		Documentation

###########################################################################*/

#include "gerais.h" /* estructures and definitions used in gerais.c */
#include <sys/file.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

static	ARQUIVO	tabarq[MAX_ARQ];  /* table of open files */	

char	*monta_nome(); /* make diretory+name+extension */ 



/**
 * Put the file's name in the table of open files if there is space available.
 *
 * @param s Name of file.
 * @param e Extension
 * @param fp pointer to the file to insert				
 */
int
insarq(char *d, char *s, char *e, FILE *fp)
{
	int i;
	char fullname[PATH_MAX];

	monta_nome(fullname, d, s, e);
	for (i = 0; i < MAX_ARQ; i++) {
		if (tabarq[i].fp == NULL) {
			tabarq[i].fp = fp; /* if it exists, put the file in the table */
			strcpy(tabarq[i].nome, normaliza(s)); /* copy the file's name to the field of the table */
			strcpy(tabarq[i].ext, normaliza(e));  /* copy the file name extension to the field of the table */
			strcpy(tabarq[i].dir, normaliza(d));
			return OK;
		}
	}

	fprintf(stderr, "No free slot to insert the file %s", fullname);
	return ERRO;
}


/**
 * Removes the file's name from the table of open files.
 *
 * @param fp Pointer to the file.
 */
int
delarq(FILE *fp)
{
	int i;
	for (i = 0; i < MAX_ARQ; i++) { /* look for the file pointer in the table of open files */
		if (tabarq[i].fp == fp) {
			tabarq[i].fp = NULL; /* free the location of table */
			return OK;
		}
	}
	return ERRO;
}


/**
 * Find the file's name corresponding to a file pointer.
 *
 * @param fp Pointer to the file or NULL if not found.
 */
char *
nomearq(FILE * fp)
{
	int i;
	for (i = 0; i < MAX_ARQ; i++) { /* looks for the pointer on the table of */
		if (tabarq[i].fp == fp) { /* open files */
			return tabarq[i].nome; /* if it is, returns the file name corresponding*/
		}
	}
	return NULL;
}


/**
 * Find the file name extension corresponding to a file poiter.
 *
 * @param fp Pointer to the file or NULL if not found.
 */
char *
extarq(FILE * fp)
{
	int i;
	for (i = 0; i < MAX_ARQ; i++) { /* looks for the pointer on the table of */
		if (tabarq[i].fp == fp)  { /* open files */
	   		return tabarq[i].ext; /* if it is, returns the file name ext. corresponding*/
		}
	}
	return NULL;
}


/**
 * Find the file directory corresponding to a file poiter.
 *
 * @param fp pointer to a file or NULL if not found.
 */
char *
dirarq(FILE *fp)
{
	int i;
	for (i = 0; i < MAX_ARQ; i++) { /* looks for the pointer on the table of */
		if (tabarq[i].fp == fp) { /* open files */
	   		return tabarq[i].dir;
		}
	}
	return NULL;
}


/**
 * Open a file for input and output.
 *
 * @param d diretory
 * @param s file name
 * @param e file name extension 
 * @param tipo 0= open for input, 1= open for input and output.	
 *
 * @return File pointer or NULL on any error.
 */
FILE *
abrearq(char *d, char *s, char *e, int tipo)
{
	FILE *fp; /* pointer for the file which will be open */
	char fullname[PATH_MAX];

	/* makes diretory+name+extension  and opens a file with pointer=fp */ 
	monta_nome(fullname, d, s, e);
	fp = fopen(fullname, tipo == 0 ? "r" : "r+");
	if (fp == NULL) {   /* it's impossible open the file, output error */
		fprintf(stderr, "Error opening file: %s", fullname);
		return NULL;
	}

	/* if the file was opened, insert it in  the table of open files */
	if (insarq(d, s, e, fp) == ERRO) {
		fclose(fp);
		return NULL;
	}

	return fp;
}



/**
 * Close a file (request elimination of the file from the table of files open).
 *
 * @param fp Pointer to the file which will be closed.
 */
int fecharq(FILE *fp)
{
	int i;
	for (i = 0; fp != NULL && i < MAX_ARQ; i++) {
		if (tabarq[i].fp == fp) {
			fclose(fp);
			delarq(fp);
			return OK;
		}
	}

	return ERRO;
}


/**
 * Create a file and insert it into the table of open files.
 *
 * @param directory Directory where the file should be created.
 * @param filename Name of the file to be created.
 * @param extension Extension of the file to be created.
 */
FILE *
criarq(char *directory, char *filename, char *extension)
{
	FILE *fp;
	char fullname[PATH_MAX];
	
	if (monta_nome(fullname, directory, filename, extension) == NULL) {
		return NULL;
	}

	fp = fopen(fullname, "w");
	if (fp == NULL) {
		fprintf(stderr, "Cannot create file %s", fullname);
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	fclose(fp);

	fp = fopen(fullname, "r+");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open file for rw: %s", fullname);
		return NULL;
	}

	if (insarq(directory, filename, extension, fp) == ERRO) {
		fclose(fp);
		return NULL;
 	}
	fseek(fp, 0, SEEK_END);
	return fp;
}


/**
 * Delete file
 */
int
delearq(char d[], char s[], char e[])
{
	char fullname[PATH_MAX];
	monta_nome(fullname, d, s, e);
	unlink(fullname);
}


/**
 * Read data from file.
 */
int
learq(FILE *fp, char buf[], int len)
{
	int i = fread(buf, 1, len, fp);
	if (i != len) {
		fprintf(stderr, "Error reading data to file %s.%s (%d from %d bytes)", nomearq(fp), extarq(fp), i, len);
		return ERRO;
	}
	return OK;
}

/**
 * Save data to file.
 */
int
gravarq(FILE *fp, char buf[], int len)
{
	int i = fwrite(buf, 1, len, fp);
	if (i != len) {
		fprintf(stderr, "Error saving data to file %s%s (%d from %d bytes)", nomearq(fp), extarq(fp), i, len);
		return ERRO;
	}
	return OK;
}

/**
 * Seek to a specific position of the file.
 */
int
posiciona(FILE *fp, long offset)
{
	if (fseek(fp, offset, 0) < 0) {
		fprintf(stderr, "Error seeking to the position %ld of %s.%s", offset, nomearq(fp), extarq(fp));
		return ERRO;
	}
	return OK;
}

/**
 * Seek to the end of the file.
 */
int
posifim(FILE *fp)
{
	if (fseek(fp, 0L, 2) < 0) {
		fprintf(stderr, "Error seeking to the end of %s.%s", nomearq(fp), extarq(fp));
		return ERRO;
	}
	return OK;
}


/**
 * Create temporary file.
 *
 * @param directory Directory where the file should be created.
 * @param prefix Prefix for the file to be created.
 *
 * @return Temporary file (open for reading and writing).
 */
FILE *
criarqtemp(char *directory, char *prefix)
{
	char fullname[PATH_MAX];
	int fd;
	
	strcpy(fullname, directory);
	strcat(fullname, prefix);
	if (fullname[strlen(fullname)] != '/') {
		strcat(fullname, "/");
	}
	strcat(fullname, "XXXXXX");
	fd = mkstemp(fullname);

	return fdopen(fd, "r+");
}

/**
 * Get the maximum number of open files.
 *
 * @return The number of maximum open files or -1 on errors.
 */
int maxfile()
{
	struct rlimit rlp;
	int i = getrlimit(RLIMIT_NOFILE, &rlp);
	if (i < 0) {
		return -1;
	}
	return (int) rlp.rlim_cur;
}
