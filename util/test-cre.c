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
#include 	<stdio.h>
#include	<stdlib.h>


#define		TRUE	1
#define		FALSE	0

#define		NOME_LENGTH	100




char	ArqFonte[NOME_LENGTH+1],
    	ArqExec[NOME_LENGTH+1],
    	ArqTeste[NOME_LENGTH+1],
    	CompCommand[NOME_LENGTH+1],
    	Funcoes[1024],
    	DirCorrente[NOME_LENGTH+1];







main(argc, argv)
int	argc;
char	*argv[];
{
int	MenosS = 0,
 	MenosE = 0, 
	MenosD = 0, 
	MenosC = 0, 
	MenosTeste = 0, 
	MenosRes = 0;

static char	BufPar[400],
		BufLi[400],
		BufPteste[400],
		BufTcase[400],
		BufMuta[400];
char	*p, *from_buf();
int	n, i;

   if (argc < 2)
   {
	msg("Missing parameters");
	exit(1);
   }
   if (*argv[argc-1] == '-')
   {
	msg("Invalid Parameter");
	exit(1);
   }

   n = argc - 2;
   strcpy(ArqTeste, argv[argc-1]);
   strcpy(ArqFonte, ArqTeste);
   
   inic_to_buf(Funcoes, sizeof(Funcoes));
   MenosS =
   MenosE =
   MenosD =
   MenosC = 
   MenosTeste = FALSE;

   for (i = 1; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-S") == 0)
	{
		MenosS = TRUE;
		strcpy(ArqFonte, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
	else

	if (strcmp(argv[i], "-E") == 0)
	{
	        MenosE = TRUE;
		strcpy(ArqExec, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
   	else

	if (strcmp(argv[i], "-D") == 0)
	{
		MenosD = TRUE;
		strcpy(DirCorrente, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
	else

	if (strcmp(argv[i], "-C") == 0)
	{
		MenosC = TRUE;
		strcpy(CompCommand, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
   }

   for (i = 1; i < argc-1; i++)
   {

	if (strcmp(argv[i], "-research") == 0)
	{
		MenosRes = TRUE;
		argv[i] = "";
		n--;
	}
	else

	if (strcmp(argv[i], "-test") == 0)
	{
		MenosTeste = TRUE;
		argv[i] = "";
		n--;
	}
   }


    if (n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }


/*--------------------------------- Cria arquivo de teste-----------------*/

   strcpy(BufLi, "li ");
   strcpy(BufPteste, "pteste -create ");
   strcpy(BufTcase, "tcase -create ");
   strcpy(BufMuta, "muta -create ");


   if (MenosS)
   {
	sprintf(BufPar, "-S %s ", ArqFonte);
	strcat(BufPteste, BufPar);
   }

   if (MenosE)
   {
	sprintf(BufPar, "-E %s ", ArqExec);
	strcat(BufPteste, BufPar);
   }

   if (MenosD)
   {
	sprintf(BufPar, "-D %s ", DirCorrente);
	strcat(BufPteste, BufPar);
	strcat(BufTcase, BufPar);
	strcat(BufMuta, BufPar);
	strcat(BufLi, BufPar);
   }

   if (MenosC)
   {
	sprintf(BufPar, "-C \"%s\" ", CompCommand);
	strcat(BufPteste, BufPar);
   }

   if (MenosTeste)
   {
	sprintf(BufPar, "-test ");
	strcat(BufPteste, BufPar);
   }

   if (MenosRes)
   {
	sprintf(BufPar, "-research ");
	strcat(BufPteste, BufPar);
   }


   sprintf(BufPar, "-P __%s ", ArqFonte);
   strcat(BufLi, BufPar);
   sprintf(BufPar, "%s __%s ", ArqFonte, ArqFonte);
   strcat(BufLi, BufPar);



   strcat(BufPteste, ArqTeste);

   strcat(BufTcase, ArqTeste);

   strcat(BufMuta, ArqTeste);

   printf("\n%s\n", BufLi);
   fflush(stdout);
   system(BufLi);

   printf("%s\n", BufPteste);
   fflush(stdout);
   system(BufPteste);

   printf("%s\n", BufTcase);
   fflush(stdout);
   system(BufTcase);

   printf("%s\n", BufMuta);
   fflush(stdout);
   system(BufMuta);
   
   return 0;
}


msg(x)
char *x;
{
   fprintf(stderr, "\ntest-new: %s\n", x);
}


