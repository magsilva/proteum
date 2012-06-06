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


#include	<lib/gerais.h>
#include	<tcase/lib/tcase.h>
#include	<muta/lib/muta.h>
#include	<pteste/lib/pteste.h>


/*------------------------------------------------------------------------
Variaveis que guardam os dados ligados ao arquivo de mutantes */



HAND_MUTANTE	HMuta;		/* manipulador arq. mutantes */

/*------------------------------------------------------------------------
Variaveis que guardam os dados ligados as arquivo de casos de teste */

HAND_TCASE	HTCase;

CAB_PTESTE	pteste_cab;

int		possivel_status[MAX_TCASE];

char    ArqTeste[NOME_LENGTH+1], /* nome dos arquivos de casos de teste e E/S */
        DirCorrente[NOME_LENGTH+1]; /* dir corrente */


main(argc, argv)
int	argc;
char	*argv[];
{
int	i, n, Menosd, Menosi, Menoslabel, Menosresearch;

   if (argc < 2)
   {
        msg("Missing parameters");
        exit(1);
   }

  if ( *argv[argc-1] == '-')
   {
        msg("Invalid Parameter");
        exit(1);
   }

	   Menosd = Menosi = Menoslabel = Menosresearch = FALSE;
	   n = argc-2;
	   strcpy(ArqTeste, argv[argc-1]);

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

	   for (i = 1; i < argc-1; i++)
	   {
		if (strcmp(argv[i], "-d") == 0)
		{
		   Menosd = TRUE;
                   argv[i] = "";
                   n--;
        	}
		else

        	if (strcmp(argv[i], "-i") == 0)
        	{
                   Menosi = TRUE;
                   argv[i] = "";
                   n--;
        	}
		else
        	if (strcmp(argv[i], "-label") == 0)
        	{
                   Menoslabel = TRUE;
                   argv[i] = "";
                   n--;
        	}
		else
        	if (strcmp(argv[i], "-research") == 0)
        	{
                   Menosresearch = TRUE;
                   argv[i] = "";
                   n--;
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

   if (scan_muta(Menosresearch) == OK)
   {
      lista_good(Menoslabel);
      if (Menosi)
	 delete_good(0);
      if (Menosd)
	 delete_good(1);
   }

   descarrega_arquivo_mutante(&HMuta);
   descarrega_arquivo_tcase(&HTCase);

   return 0;
}


msg(x)
char *x;
{
   fprintf(stderr, "\nlist-good: %s\n", x);
}





/*********************************************************************
SCAN_MUTA:
	This function builds a table that relates mutants and the
test cases that killed them.
******************************************************************/
scan_muta(int research)
{
int	ntcase = NTCASE(&HTCase);
int	i, j, k, t, l, b;


   for (j = 0; j < ntcase; j++)
  	possivel_status[j] = 0;

   for (j = 0; j < GERADOS(&HMuta); j++)
   {
	if (carrega_descr(&HMuta, j) == ERRO)
	   return ERRO;
	if (get_mutante_inactive(&HMuta))
	   continue;
	k = get_mutante_status(&HMuta);
	if (k != MORTO)
	   continue;

	for (k = 1; k <= ntcase; k++)
	{
	   t = ltofis_tcase(&HTCase, k);
	   if  (TAB_FIS(&HTCase)[t].reg_log > REG(&HMuta).ultimo_tcase)
		continue;
	   l = get_bitmap(REG(&HMuta).tcase, t);
	   if (EXEC_MORTO(l))
	   {
		(possivel_status[k-1])++;
		if (research)
		   break;
	   }
	 }
   }
   return OK;
}

lista_good(int lab)
{
int	n,i, j, k;

   n = NTCASE(&HTCase);
   for (i = j = 0; i < n; i++)
   {
        k = ltofis_tcase(&HTCase, i+1);
	if ( TAB_FIS(&HTCase)[k].desabili == FALSE && possivel_status[i] > 0)
	{
	   if (lab)
	   {
		le_tcase_reg(&HTCase, k);
		printf("%s\n", 	TREG(&HTCase).label);
	   }
	   else
	   	printf("%d\n", i+1);
	   j++;
	}
   }
   fprintf(stderr, "\n%d effective test cases\n", j);
}


delete_good(flg_del)
int	flg_del;
{
int	n,i, k;

   n = NTCASE(&HTCase);
   for (i = n-1; i >= 0; i--)
   {
	k = ltofis_tcase(&HTCase, i+1);
        if ( TAB_FIS(&HTCase)[k].desabili == FALSE && possivel_status[i] == 0)
        {  
	   if (flg_del)     
	      delete_tcase(&HTCase, i+1, i+1);
	   else
		disable_tcase(&HTCase, k);		
	}
   }
}

