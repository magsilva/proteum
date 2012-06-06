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
#include	<li/lib/li.h>
#include	"lib/pteste.h"


int	Menosl,
	MenosAll,
	TipoTeste;

char	ArqFonte[NOME_LENGTH+1],
    	ArqExec[NOME_LENGTH+1],
    	ArqTeste[NOME_LENGTH+1],
    	CompCommand[1024],
    	Funcoes[1024],
    	DirCorrente[NOME_LENGTH+1];



CAB_PTESTE	pteste_cab;




main(argc, argv)
int	argc;
char	*argv[];
{
   if (argc < 3)
   {
	msg("Missing parameters");
	exit(1);
   }
   if (*argv[argc-1] == '-')
   {
	msg("Invalid Parameter");
	exit(1);
   }


   if (strcmp(argv[1], "-create") == 0)
   {
	argv[1] = "";
	CreatePteste(argc, argv);
   }
   else
   if (strcmp(argv[1], "-l") == 0)
   {
	argv[1] = "";
	ListPteste(argc, argv);
   }
   else
   {
	msg("Invalid Parameters");
        exit(1);
  }
   return 0;

}



ListPteste(argc, argv)
int	argc;
char	*argv[];
{
int	i, n;
char	*p, *from_buf();

   n = argc-3;
   strcpy(DirCorrente, ".");

   for (i = 1; i < argc-2; i++)
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

   if (n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }


   strcpy(ArqTeste, argv[argc-1]);

/*----------------------------- Carrega arquivo e mostra conteudo ---------*/
   if (carrega_arquivo_teste(DirCorrente, ArqTeste, &pteste_cab) == ERRO)
	exit(1);

   printf("\n\nName.........: %s", pteste_cab.nome);
   printf("\nSource File..: %s.%s", pteste_cab.fonte, SUFIXO_FONTE);
   printf("\nExec. File...: %s", pteste_cab.exec);
   printf("\nComp. Command: %s", pteste_cab.comp);
   printf("\nTest Type....: %s", 
		pteste_cab.tipo == T_TESTE? STR_TESTE : STR_RESEARCH);
/*   printf("\nFunctions....: ");
   if (pteste_cab.todas)
	printf("%s", STR_ALLFUNC);
   else
   {
	inic_from_buf(pteste_cab.funcoes);
	while ((p = from_buf()) != NULL)
	{
	   printf("%s\n               ", p);
	}
   }
*/
  printf("\n");

}


CreatePteste(argc, argv)
int	argc;
char	*argv[];
{
int	i, n;
int	MenosE = FALSE;

   n = argc - 3;
   strcpy(ArqTeste, argv[argc-1]);
   
   strcpy(ArqFonte, ArqTeste);
   strcpy(DirCorrente, ".");
   inic_to_buf(Funcoes, sizeof(Funcoes));
   MenosAll = TRUE;

   for (i = 1; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-S") == 0)
	{
		strcpy(ArqFonte, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
	else

	if (strcmp(argv[i], "-E") == 0)
	{
		strcpy(ArqExec, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
		MenosE = TRUE;
	}
   	else

	if (strcmp(argv[i], "-D") == 0)
	{
		if (chdir(argv[i+1]) < 0)
		{
		 	msg("Invalid directory on parameter -D");
			exit(1);
            	}
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
/*	else

	if (strcmp(argv[i], "-u") == 0)
	{
		MenosAll = FALSE;
		to_buf(argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
	} */
   }

   if (! MenosE)
	strcpy(ArqExec, ArqFonte);

   sprintf(CompCommand, "gcc %s%s -o %s -w ", ArqFonte, SUFIXO_FONTE, ArqExec);
   for (i = 1; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-C") == 0)
	{
		strcpy(CompCommand, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
		break;
	}
   }

   TipoTeste = -1;  
   for (i = 1; i < argc-1; i++)
   {
/*	if (strcmp(argv[i], "-all") == 0)
	{
		if (! MenosAll)
		{
		   msg("Invalida parameters: -u and -All");
		   exit(1);
		}
		argv[i] = "";
		n--;
	}
	else */

	if (strcmp(argv[i], "-research") == 0)
	{
		if (TipoTeste == T_TESTE)
		{
		   msg("Invalida Parameters: -Test and -Research");
		   exit(1);
		}
		TipoTeste = T_RESEARCH;
		argv[i] = "";
		n--;
	}
	else

	if (strcmp(argv[i], "-test") == 0)
	{
		if (TipoTeste == T_RESEARCH)
		{
		   msg("Invalida Parameters: -Test and -Research");
		   exit(1);
		}
		TipoTeste = T_TESTE;
		argv[i] = "";
		n--;
	}
   }

   TipoTeste = TipoTeste == - 1? T_TESTE : TipoTeste;

    if (n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }


/*--------------------------------- Cria arquivo de teste-----------------*/

   cria_arquivo_teste(DirCorrente, ArqTeste, ArqFonte, ArqExec, 
			CompCommand, TipoTeste, NONE, MenosAll? NULL: Funcoes,
			&pteste_cab);

}

msg(x)
char *x;
{
   fprintf(stderr, "\npteste: %s\n", x);
}


