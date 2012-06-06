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


#include	<string.h>
 
#include	<lib/gerais.h>
#include	<tcase/lib/tcase.h>
#include	<muta/lib/muta.h>
#include	<pteste/lib/pteste.h>


#define		L_DEFAULT_TCASE 	17 /* nivel default p/ C.T */
#define		L_DEFAULT_TRACE		0

#define		SUFIXO_RELAT		".lst"
#define		SUFIXO_RELAT_TRACE	".trc"

/*------------------------------------------------------------------------
Variaveis que guardam os dados ligados ao arquivo de mutantes */



HAND_MUTANTE	HMuta;		/* manipulador arq. mutantes */

/*------------------------------------------------------------------------
Variaveis que guardam os dados ligados as arquivo de casos de teste */

HAND_TCASE	HTCase;


/*------------------------------------------------------------------------
Variaveis que guardam os dados ligados as arquivo de  teste */

CAB_PTESTE	pteste_cab;



char	ArqTeste[NOME_LENGTH+1], /* nome dos arquivos de casos de teste e E/S */
	DirCorrente[NOME_LENGTH+1], /* dir corrente */
	ArqRep[NOME_LENGTH+1];	/* nome do arq de relatorio */


main(argc, argv)
int	argc;
char	*argv[];
{
   if (argc < 3)
   {
	msg("Missing parameters");
	exit(1);
   }

  if ( *argv[argc-1] == '-')
   {
	msg("Invalid Parameter");
	exit(1);
   }


   if (strcmp(argv[1], "-tcase") == 0)
   {
	argv[1] = "";
	ReportTcase(argc, argv);
   }
   else
   if (strcmp(argv[1], "-trace") == 0)
   {
	argv[1] = "";
	ReportTrace(argc, argv);
   }
   else
   if (strcmp(argv[1], "-g") == 0)
   {
	argv[1] = "";
	ReportGera(argc, argv);
   }
   else
	msg("Missing parameter");
   return 0;
}


/***************************************************************************
	Le parametros da opcao -tcase do programa

***************************************************************************/
ReportTcase(argc, argv)
int	argc;
char	*argv[];
{
int	i, n;
int	MenosL;

   n = argc-3;
   strcpy(ArqTeste, argv[argc-1]);
   strcpy(ArqRep, ArqTeste);

   MenosL = L_DEFAULT_TCASE;
   strcpy(DirCorrente, ".");
   for (i = 2; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-D") == 0)
	{
		if (chdir(argv[i+1]) < 0)
		{
			msg("Invalid directory on parameter -D");
			exit(1);
            	}
		argv[i] = argv[i+1] = "";
		n-=2;
	}
	else
	if (strcmp(argv[i], "-L") == 0)
	{
		MenosL = atoi(argv[i+1]);
		argv[i] = argv[i+1] = "";
		n-=2;
	}
	else
	if (strcmp(argv[i], "-S") == 0)
	{
		strcpy(ArqRep, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n-=2;
	}

   }

   strcat(ArqRep, SUFIXO_RELAT);

   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

/*----------------------------- Carrega arquivo de teste ---------*/
   if (carrega_arquivo_teste(DirCorrente, ArqTeste, &pteste_cab) == ERRO)
	exit(1);

/*------------------------------- Carrega arquivo de teste -------------*/

   if (carrega_arquivo_tcase(&HTCase,DirCorrente, ArqTeste) == ERRO)
	exit(1);

/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqTeste) == ERRO)
   {
   	descarrega_arquivo_tcase(&HTCase);
	exit(1);
   }

   gera_report_tcase(DirCorrente, ArqRep, 1, MenosL);


   descarrega_arquivo_mutante(&HMuta);
   descarrega_arquivo_tcase(&HTCase);

}


/***************************************************************************
	Le parametros da opcao -trace do programa

***************************************************************************/
ReportTrace(argc, argv)
int	argc;
char	*argv[];
{
int	i, n;
int	MenosL;

   n = argc-3;
   strcpy(ArqTeste, argv[argc-1]);
   strcpy(ArqRep, ArqTeste);

   MenosL = L_DEFAULT_TRACE;
   strcpy(DirCorrente, ".");
   for (i = 2; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-D") == 0)
	{
		if (chdir(argv[i+1]) < 0)
		{
			msg("Invalid directory on parameter -D");
			exit(1);
            	}
		argv[i] = argv[i+1] = "";
		n-=2;
	}
	else
	if (strcmp(argv[i], "-L") == 0)
	{
		MenosL = atoi(argv[i+1]);
		argv[i] = argv[i+1] = "";
		n-=2;
	}
	else
	if (strcmp(argv[i], "-S") == 0)
	{
		strcpy(ArqRep, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n-=2;
	}

   }

   strcat(ArqRep, SUFIXO_RELAT_TRACE);

   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

/*----------------------------- Carrega arquivo de teste ---------*/
   if (carrega_arquivo_teste(DirCorrente, ArqTeste, &pteste_cab) == ERRO)
	exit(1);

/*------------------------------- Carrega arquivo de teste -------------*/

   if (carrega_arquivo_tcase(&HTCase,DirCorrente, ArqTeste) == ERRO)
	exit(1);

/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqTeste) == ERRO)
   {
   	descarrega_arquivo_tcase(&HTCase);
	exit(1);
   }

   gera_report_trace(DirCorrente, ArqRep, MenosL);


   descarrega_arquivo_mutante(&HMuta);
   descarrega_arquivo_tcase(&HTCase);

}


/***************************************************************************
	Le parametros da opcao -g do programa

***************************************************************************/
ReportGera(argc, argv)
int	argc;
char	*argv[];
{
int	i, n;

   n = argc-3;
   strcpy(ArqTeste, argv[argc-1]);

   strcpy(DirCorrente, ".");
   for (i = 2; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-D") == 0)
	{
		if (chdir(argv[i+1]) < 0)
		{
			msg("Invalid directory on parameter -D");
			exit(1);
            	}
		argv[i] = argv[i+1] = "";
		n-=2;
	}
   }


   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

/*----------------------------- Carrega arquivo de teste ---------*/
   if (carrega_arquivo_teste(DirCorrente, ArqTeste, &pteste_cab) == ERRO)
	exit(1);

/*------------------------------- Carrega arquivo de teste -------------*/

   if (carrega_arquivo_tcase(&HTCase,DirCorrente, ArqTeste) == ERRO)
	exit(1);

/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqTeste) == ERRO)
   {
   	descarrega_arquivo_tcase(&HTCase);
	exit(1);
   }

   gera_report_gera();


   descarrega_arquivo_mutante(&HMuta);
   descarrega_arquivo_tcase(&HTCase);

}


msg(x)
char *x;
{
   fprintf(stderr, "\nreport: %s\n", x);
}


