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
#include	<muta/lib/muta.h>
#include	<li/lib/li.h>
#include	"lib/opmuta.h"



char	ArqLI[NOME_LENGTH+1],		/* nome do arquivo LI*/
	ArqFonte[NOME_LENGTH+1],	/* arq. fonte */
	DirCorrente[NOME_LENGTH+1], 	/* dir corrente */
	DirOp[NOME_LENGTH+1],		/* dir. arq de % de operadores */
	ArqOp[NOME_LENGTH+1],		/* arq. de % de operadores */
	auxbuf[2*NOME_LENGTH+1];

char	Conexoes[4096],			/* nome das conexoes a testar */
	Funcoes[4096];			/* nume das funcoes a testar */


extern	OPERADOR_MUTACAO	g_tab_operador[];

int	debug;

main(argc, argv)
int	argc;
char	*argv[];
{
int	MenosDD, Menosr;
int	n, i, j , k, l;
char	*getenv();

   if (argc < 3)
   {
	msg("Missing parameters");
	exit(1);
   }

  if ( *argv[argc-1] == '-' || *argv[argc-2] == '-')
   {
	msg("Invalid Parameter");
	exit(1);
   }

   n = argc - 3;

   strcpy(ArqFonte, argv[argc-2]);
   strcpy(ArqLI, argv[argc-1]);

   strcpy(DirCorrente, ".");
   MenosDD = Menosr = FALSE;


   inic_to_buf(Conexoes, sizeof(Conexoes));

   for (i = 1; i < argc-2; i++)
   {
        if (strcmp(argv[i], "-r") == 0)
        {
		Menosr = TRUE;
                argv[i] = "";
                n--;
        }
   }


   for (i = 1; i < argc-3; i++)
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

	if (strcmp(argv[i], "-DD") == 0)
	{
		if (! tem_dir(argv[i+1]))
		{
			msg("Invalid directory on parameter -DD");
			exit(1);
       		}
		MenosDD = TRUE;
		strcpy(DirOp, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n-=2;
	}
	else

	if (strcmp(argv[i], "-O") == 0)
	{
		strcpy(ArqOp, argv[i+1]);
	}
   }

   if ( ! MenosDD)
	strcpy(DirOp, DirCorrente);

  
   for (i = 1; i < argc-3; i++)
   {

	if (strcmp(argv[i], "-O") == 0)
	{
		if (LeArqOp(DirOp, ArqOp, g_tab_operador) == ERRO)
			exit(1);
		n -= 2;
	        argv[i] = argv[i+1] = "";
	}
	else

        if (i < argc-4 && *argv[i] == '-' && (j = busca_op(&(argv[i][1]) )) >= 0)
        {
           do
           {
                k = atoi(argv[i+1]);
                g_tab_operador[j].percent = MINI(k,100);
                k = atoi(argv[i+2]);
                g_tab_operador[j].maximum = MINI(k,128);
                j = next_match(&(argv[i][1]), j);
           } while (j >= 0);

           argv[i] = argv[i+1] = argv[i+2] = "";
           n -= 3;
        }
	else
  
        if (i < argc-4 && strcmp(argv[i], "-all") == 0)
        {
           k = atoi(argv[i+1]);
	   l = atoi(argv[i+2]);
           for (j = 0; j < NOPERADORES; j++)
	   {
		g_tab_operador[j].maximum = MINI(l,128);
                g_tab_operador[j].percent = MINI(k,100);
	   }
           argv[i] = argv[i+1] = argv[i+2] ="";
           n -= 3;
        }
        else

	if (strcmp(argv[i], "-c") == 0)
	{
	   sprintf(auxbuf, "%s$%s", argv[i+1], argv[i+2]);
	   to_buf(auxbuf);
	   argv[i] = argv[i+1] = argv[i+2] = "";
	   n -= 3;
	}
   }


   inic_to_buf(Funcoes, sizeof(Funcoes));

   for (i = 1; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-unit") == 0)
	{
	   to_buf(argv[i+1]);
	   argv[i] = argv[i+1] =  "";
	   n -= 2;
	}
   }



   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }
   strcat(ArqFonte, SUFIXO_FONTE);

   n = opmuta(DirCorrente, ArqLI, DirCorrente, 
			ArqFonte, Funcoes, Conexoes, Menosr);

   fprintf(stderr, "\n%d Mutants Generated\n", n);
   return 0;
}


msg(x)
char *x;
{
   fprintf(stderr, "\nopmuta: %s\n", x);
}




