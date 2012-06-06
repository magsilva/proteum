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

#include	<lib/gerais.h>
#include	<pteste/lib/pteste.h>


/*------------------------------------------------------------------------
Variaveis que guardam os dados ligados as arquivo de  teste */

CAB_PTESTE	pteste_cab;



char	   	ArqTeste[NOME_LENGTH+1],
		DirCorrente[NOME_LENGTH+1];







main(argc, argv)
int	argc;
char	*argv[];
{
int	MenosD;

static char	BufPar[200],
		BufTcase[400];
char	*p, *from_buf();
int	n, i;

   if (argc < 2)
   {
	msg("Missing parameters");
	exit(1);
   }
   if (*argv[argc-1] == '-' )
   { 
	msg("Invalid Parameter");
	exit(1);
   }

   n = argc - 2;
   strcpy(ArqTeste, argv[argc-1]);
   
   MenosD = FALSE;

   for (i = 1; i < argc-2; i++)
   {

	if  (strcmp(argv[i], "-D") == 0   || strcmp(argv[i], "-DD") == 0 ||
	     strcmp(argv[i], "-label") == 0 || strcmp(argv[i], "-EE") == 0 )
	{
		strcat(BufPar, " ");
		strcat(BufPar, argv[i]);
		i++;
		strcat(BufPar, " ");
		strcat(BufPar, argv[i]);
		argv[i] = argv[i-1] = "";
		n-=2;
	}
	else 
	if  (strcmp(argv[i], "-p") == 0 ) 
	{
		strcat(BufPar, " ");
		strcat(BufPar, argv[i]);
		i++;
		strcat(BufPar, " \"");
		strcat(BufPar, argv[i]);
		strcat(BufPar, "\"");
		argv[i] = argv[i-1] = "";
		n-=2;
	}
   }

   for (i = 1; i < argc-1; i++)
   {
	
 	if (strcmp(argv[i], "-P") == 0 || strcmp(argv[i], "-trace") == 0)
	{
		strcat(BufPar, " ");
		strcat(BufPar, argv[i]);
		argv[i] = "";
		n--;
	}
  }


    if (n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

/*----------------------------- Carrega arquivo de teste ---------*/
   if (carrega_arquivo_teste(DirCorrente, ArqTeste, &pteste_cab) == ERRO)
	exit(1);



   sprintf(BufTcase, "tcase -add %s -E %s %s", BufPar, pteste_cab.exec,  
								ArqTeste) ;



   printf("%s\n", BufTcase);
   fflush(stdout);
   system(BufTcase);
   
   return 0;
}


msg(x)
char *x;
{
   fprintf(stderr, "\tcase-add: %s\n", x);
}


