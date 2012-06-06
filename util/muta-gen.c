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
int	MenosD, Menosq;

static char	BufPar[1024],
		BufOpmuta[1024],
		BufPipe[2048],
		BufMuta[1024];
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
   
   MenosD = Menosq = FALSE;


   for (i = 1; i < argc-1; i++)
   {

        if (strcmp(argv[i], "-r") == 0)
        {
                strcat(BufPar, " -r ");
                argv[i] = "";
                n --;
        }
        else
        if (strcmp(argv[i], "-q") == 0)
        {
                Menosq = TRUE;
                argv[i] = "";
                n --;
        }
   }

   for (i = 1; i < argc-1; i++)
   {

	if (strcmp(argv[i], "-D") == 0)
	{
		MenosD = TRUE;
		strcpy(DirCorrente, argv[i+1]);
		strcat(BufPar, " -D ");
		strcat(BufPar, DirCorrente);
		argv[i] = argv[i+1] = "";
		n -= 2;
		i++;
	}
	else

        if (i < argc-2 && (strcmp(argv[i], "-all") == 0 ||
            (*argv[i] == '-' && busca_op(&(argv[i][1]))  >= 0)) )
        {
                strcat(BufPar, " ");
                strcat(BufPar, argv[i]);
                i++;
                strcat(BufPar, " ");
                strcat(BufPar, argv[i]);
                i++;
                strcat(BufPar, " ");
                strcat(BufPar, argv[i]);

                argv[i] = argv[i-1] = argv[i-2] = "";
                n-=3;
        }
        else


	if  (strcmp(argv[i], "-O") == 0   || strcmp(argv[i], "-DD") == 0 ||
		strcmp(argv[i], "-unit") == 0) 
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

	if  (strcmp(argv[i], "-c") == 0 )
        {
                strcat(BufPar, " ");
                strcat(BufPar, argv[i]);
                i++;
                strcat(BufPar, " ");
                strcat(BufPar, argv[i]);
                i++;
                strcat(BufPar, " ");
                strcat(BufPar, argv[i]);
                argv[i] = argv[i-1] = argv[i-2] = "";
                n-=3;
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

   if (MenosD)

	sprintf(BufMuta, "muta -add -D %s %s ", DirCorrente, ArqTeste);
   else
	sprintf(BufMuta, "muta -add  %s ", ArqTeste);



   sprintf(BufOpmuta, "opmuta %s %s %s ", BufPar, pteste_cab.fonte_expa,  
							pteste_cab.fonte_expa) ;

   sprintf(BufPipe, "%s | %s", BufOpmuta, BufMuta) ;


   if ( ! Menosq )printf("%s\n", BufPipe);
   fflush(stdout);
   system(BufPipe);
   return 0;   

}

msg(x)
char *x;
{
   fprintf(stderr, "\nmuta-gen : %s\n", x);
}


