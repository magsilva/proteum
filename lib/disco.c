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

#include	"gerais.h" /* estructures and definitions used in gerais.c */
#include	<sys/file.h>
#include	<stdlib.h>
#include	<sys/time.h>
#include	<sys/resource.h>

char	*mktemp(char *);

static	int	proxlivre = 0;

static	ARQUIVO	tabarq[MAX_ARQ];  /* table of open files */	

char	*monta_nome(); /* make diretory+name+extension */ 



/*@FUNCTION******************************************************************
INSARQ:
	Put the file's name in  the table of open files.

Parameters:
	s-name of file
	e-extension
	fp-pointer to the file to insert				
*********************************************************************/
insarq(d, s, e, fp)
char	*d, *s, *e;
FILE	*fp;
{
int	i;
char	*normaliza(); /* remove blanks from the beginning and the end of a
			string */
   for (i = 0; i < MAX_ARQ; i++)
	if (tabarq[i].fp == NULL)
	    break;	/* looks for the first empty location of the table */
   if (i >= MAX_ARQ )   /* if it don't exists, outputs error */
	    {
		d_msg(s, e, "No Memory to Open File");
		return ERRO;
	     }
   tabarq[i].fp = fp; /* if it exists, put the file in the table */	
   strcpy(tabarq[i].nome, normaliza(s)); /* copy the file's name to the field
						of the table */
   strcpy(tabarq[i].ext, normaliza(e));  /* copy the file name extension 
						to the field of the table */

   strcpy(tabarq[i].dir, normaliza(d));
   return OK;
}


/*@FUNCTION************************************************************
DELARQ:
	Removes the file's name from the table of open files.
Parameter: 
	   fp-pointer to the file 	
*********************************************************************/
delarq(fp)
FILE	*fp;	/* pointer to the file for remove */
{
int	i;	/* loop counter */

   for (i = 0; i < MAX_ARQ; i++) /* look for the file pointer in the table of */
	if (tabarq[i].fp == fp)  /* open files */
	    break;
   tabarq[i].fp = NULL; /* free the location of table */
}


/*@FUNCTION**************************************************************
NOMEARQ:
	Find the file's name corresponding to a file pointer.
Parameter: 
	   fp-pointer to the file 
***********************************************************************/
char	*nomearq(fp) /* file name corresponding to the file pointer */
FILE	*fp;	 /* file pointer */   
{
int	i;	/* loop counter */

   for (i = 0; i < MAX_ARQ; i++) /* looks for the pointer on the table of */
	if (tabarq[i].fp == fp)  /* open files */
	   break;
   if (i >= MAX_ARQ)
	  return (char	*) ""; /* the file isn't in the table-> not open*/
   return tabarq[i].nome; /* if it is, returns the file name corresponding*/
}


/*@FUNCTION********************************************************************
EXTARQ:
	Find the file name extension corresponding to a file poiter.
Parameter: 
	   fp-pointer to the file 
******************************************************************************/
char	*extarq(fp) /* extension of file name corresponding to 
			the file pointer */
FILE	*fp;   /* file  pointer */
{
int	i;  /* loop counter */

   for (i = 0; i < MAX_ARQ; i++) /* looks for the pointer on the table of */
	if (tabarq[i].fp == fp)  /* open files */
	    break;
   if (i >= MAX_ARQ)
	return (char	*) "";/* the file isn't in the table-> not open*/
   return tabarq[i].ext; /* if it is, returns the file name ext. corresponding*/
}


/*@FUNCTION********************************************************************
DIRARQ:
        Find the file directory corresponding to a file poiter.
Parameter:
           fp-pointer to the file
******************************************************************************/
char    *dirarq(fp) /* directory of file name corresponding to
                        the file pointer */
FILE    *fp;   /* file  pointer */
{
int     i;  /* loop counter */

   for (i = 0; i < MAX_ARQ; i++) /* looks for the pointer on the table of */
        if (tabarq[i].fp == fp)  /* open files */
            break;
   if (i >= MAX_ARQ)
        return (char    *) "";/* the file isn't in the table-> not open*/
   return tabarq[i].dir; 
}


/*@FUNCTION******************************************************************
ABREARQ:
	Open a file for input and output.

Parameters:
	d- diretory
	s- file name
	e- file name extension 
	Tipo- 0= open for input.
	      1= open for input and output.	
Return:
	file poiter or NULL(if error).
Author: Delamaro
***************************************************************************/
FILE	*abrearq(d, s, e, tipo)
char	*s, *d ,*e;
int	tipo;
{
FILE	*fp; /* pointer for the file which will be open */

   /* makes diretory+name+extension  and opens a file with pointer=fp */ 
   fp = fopen(monta_nome(d,s,e), tipo == 0 ? "r" : "r+");
   if (fp == NULL) 
	    {   /* it's impossible open the file, output error */
		d_msg(s, e, "Error Opening File");
		return NULL;
	     }
   /* if the file was opened */
   /* Insert the file name in  the table of open files */
   if (insarq(d, s, e, fp) == ERRO)
   {
	fclose(fp);
	return NULL;
   }
   return fp; /* return a pointer "fp" to the file opened */
}



/*@FUNCTION*******************************************************************
FECHARQ:
	Close a file (request eliminacion of the file from the table of files 
									open).
Parametro:
	fp : file pointer to the file which will be closed.

Autor: Delamaro
***************************************************************************/
fecharq(fp)
FILE	*fp; /* file pointer to the file which will be closed */
{
int	i;  /* loop counter */

   for (i = 0; i < MAX_ARQ; i++) /*look for the file on the table
							 of open files */
	if (tabarq[i].fp == fp)
	   break;
   if (i >= MAX_ARQ || fp == NULL) /*if it'snt in the table */
	 return;       /*returns fp=NULL */  
   fclose(fp);  /*If the file was found on the table it is closed and */
   delarq(fp);  /* its pointer is left out of the table of open files */
}


/*parei aqui */
/*@FUNCTION*******************************************************************
CRIARQ:
	Creat a file.

Parametro:
	d:diretory
	s: name
	e:extension

Autor: Delamaro
***************************************************************************/
FILE	*criarq(d,s,e)
char	*d,*s,*e;
{
FILE	*fp;

   fp = fopen(monta_nome(d,s,e), "w");
   if (fp == NULL)
	    {
		l1:
		d_msg(s, e,"Error Creating File");
		return NULL;
	     }
   fseek(fp, 0, SEEK_END);
   fclose(fp);
   fp = fopen(monta_nome(d,s,e), "r+");
   if (fp == NULL)
	goto l1;
   if (insarq(d, s, e, fp) == ERRO)
   {
	fclose(fp);
	return NULL;
   }
   fseek(fp, 0, SEEK_END);
   return fp;
}



delearq(d,s,e)
char	*d,*s,*e;
{
char	*p = (char *) monta_nome(d,s,e);

   unlink(p);
}



learq(fp, buf, len)
char	buf[];
FILE	*fp;
int	len;
{
int	i;

   i = fread(buf, 1, len, fp);
   if (i != len) 
	     {
		d_msg(nomearq(fp), extarq(fp), "Error Reading File");
		return ERRO;
	     }
   return OK;
}


gravarq(fp, buf, len)
char	buf[];
FILE	*fp;
int	len;
{
int	i;

   i = fwrite(buf, 1, len, fp);
   if (i != len) 
	    {
		d_msg(nomearq(fp), extarq(fp), "Error Writing File");
		return ERRO;
	     }
 /*  fflush(fp);  */
   return OK;
}


posiciona(fp, off7)
FILE	*fp;
long	off7;
{
   if (fseek(fp, off7, 0) < 0) 
	    {
		d_msg(nomearq(fp), extarq(fp), "Seek Error");
		return ERRO;
	     }
   return OK;
}



posifim(fp)
FILE	*fp;
{
   if (fseek(fp, 0L, 2) < 0) 
	    {
		d_msg(nomearq(fp), extarq(fp), "Seek Error");
		return ERRO;
	     }
   return OK;
}



d_msg(s, e, m)
char	s[], m[];
{
char	buf[102];

   sprintf(buf, "%s: %s%s", m, s,e);
   msg(buf);
}


FILE *criarqtemp(d, s)
char	*d, *s;
{
char	*p, *q;

   p = (char *) malloc(strlen(s)+10);
   if (p == NULL)
   {
	msg("Error creating temporary file (Insuficient Memory)");
	return NULL;
   }

   strcpy(p, s);
   strcat(p, "XXXXXX");
   q = mktemp(p);

   free(p);

   return criarq(d, q, "");
}


	


/*@FUNCTION **************************************************************
MAXFILE:

Return the maximum number of open files.

*************************************************************************/
int maxfile()
{
struct rlimit rlp;
int i;

   i = getrlimit(RLIMIT_NOFILE, &rlp);

   if (i < 0)
	return -1;

   return (int) rlp.rlim_cur;
}

