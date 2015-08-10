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
#include	<stdlib.h>
#include	<signal.h>

#include	<lib/gerais.h>
#include	<li/lib/li.h>
#include	<muta/lib/muta.h>
#include	<tcase/lib/tcase.h>
#include	<pteste/pteste.h>

#define		MAXINTEGER	(((unsigned) -1) >> 1)
#define		DEFAULT_MENOSV   ""

CAB_PTESTE	pteste_cab;	/* estrutura onde carregar arq. teste */

HAND_TCASE	HTCase;

HAND_MUTANTE	HMuta;	/* manipulador do arquivo de mutantes */

char	ArqTeste[NOME_LENGTH+1],	/* nome do arquivo de teste */
	DirCorrente[NOME_LENGTH+1], 	/* dir corrente */
	DirHome[NOME_LENGTH+1],		/* diretorio do proteum */
	DirFonte[NOME_LENGTH+1],	/* diretorio p/ criar arq. fonte */
	ArqFonte[NOME_LENGTH+1],	/* arq. fonte */
        DirOp[NOME_LENGTH+1],           /* dir. arq de % de operadores */
        ArqOp[NOME_LENGTH+1],           /* arq. de % de operadores */
        auxbuf[2*NOME_LENGTH+1],
	auxbuf2[2*NOME_LENGTH+2];

int	g_timeout;

int	flg_interrup;

char    Funcoes[4096];                  /* nume das funcoes a testar */

void	interrup_exec();

extern	OPERADOR_MUTACAO	g_tab_operador[];

static int VetMenosx[4200];

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


   if (strcmp(argv[1], "-exec") == 0)
   {
	ExecMuta(argc, argv);  
   }
   else
   if (	strcmp(argv[1], "-select") == 0 )
   {
	SelectMuta(argc, argv);
   }
   else
   if (	strcmp(argv[1], "-build") == 0 )
   {
	BuildSourceMuta(argc, argv, TRUE);
   }
   else
   if (strcmp(argv[1], "-invert") == 0 )
   {
		InvertMuta(argc, argv);
	}
   else
   if (	strcmp(argv[1], "-build-view") == 0 )
   {
	BuildViewMuta(argc, argv);
   }
   else
   if (	strcmp(argv[1], "-build-one") == 0 )
   {
	BuildSourceMutaOne(argc, argv);
   }
   else
   if ( strcmp(argv[1], "-update") == 0 )
   {
        UpdateMuta(argc, argv);
   }
   else
   if ( strcmp(argv[1], "-compile") == 0 )
   {
        CompileMuta(argc, argv);
   }
   else
  {
        msg("Missing parameter");
        exit(1);
   }
   return 0;


}


/***************************************************************************
	Le parametros da opcao -exec do programa

***************************************************************************/
ExecMuta(argc, argv)
int	argc;
char	*argv[];
{
    static int order_tcase[MAX_TCASE];
    int	f, i, n, t;
    int	Menost, Menosf, MenosT, MenosQ, Menostrace, MenosDual, MenosSeed;
    char	*Menosv = NULL;
    long int seed = 0;

   n = argc-3;
   strcpy(ArqTeste, argv[argc-1]);

   f = MAXINTEGER;
   t = 0;
    Menost = Menosf = Menostrace = MenosQ = MenosDual = MenosSeed = FALSE;
   MenosT = 5;
   strcpy(DirCorrente, ".");

   if (getenv(STR_DIR_HOME) != NULL)
      strcpy(DirHome, getenv(STR_DIR_HOME));
   else
      DirHome[0] = '\0';

   for (i = 2; i < argc-1; i++)
   {
	if (strcmp(argv[i], "-trace") == 0)
	{
		Menostrace = TRUE;
		argv[i] = "";
		n--;
	}
        else if (strcmp(argv[i], "-dual") == 0)
	{
		MenosDual = TRUE;
		argv[i] = "";
		n--;
	}
   }

   MenosQ = 1000;
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

	if (strcmp(argv[i], "-Q") == 0)
	{
                MenosQ = atoi(argv[i+1]);
		argv[i] = argv[i+1] = "";
		n-=2;
	}
	else

        if (strcmp(argv[i], "-f") == 0)
        {
                t = MAXI(atoi(argv[i+1]), t);
                Menost = TRUE;
                argv[i] = argv[i+1] = "";
                n -= 2;
        }
        else


	if (strcmp(argv[i], "-t") == 0)
	{
		f = MINI(atoi(argv[i+1])+1, f);
		Menosf = TRUE;
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
                    else if (strcmp(argv[i], "-T") == 0)
	{
		MenosT = atoi(argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
                    else if (strcmp(argv[i], "-v") == 0)
        {
                Menosv = argv[i+1];
                argv[i] = argv[i+1] = "";
                n-=2;
		if ( strlen(Menosv) == 0)
			Menosv = DEFAULT_MENOSV;
        }
                    else if (strcmp(argv[i], "-seed") == 0)
                    {
                        MenosSeed = TRUE;
                        seed = atol(argv[i+1]);
                        argv[i] = argv[i+1] = "";
                        n-=2;
                    }
   }


   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }
    for (i = 0; i < MAX_TCASE; i++)
    {
        order_tcase[i] = i+1;
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

    if (MenosSeed && seed != 0)
            shufle(order_tcase, seed, NTCASE(&HTCase));

    f = MINI(f, GERADOS(&HMuta));

   signal(SIGINT, interrup_exec);
    executa_mutante(MenosT, t, f, MenosQ, Menosv, Menostrace, MenosDual, order_tcase);
   atualiza_mutantes(&HMuta);

   if (flg_interrup && Menosv)
	printf("\nOps! Execution was interrupted\n");
	


   descarrega_arquivo_mutante(&HMuta);
   descarrega_arquivo_tcase(&HTCase);

}


/***************************************************************************
	Le parametros da opcao -select do programa

***************************************************************************/
SelectMuta(argc, argv)  // TODO: revisar (-exec funciona, mas -select não)
int	argc;
char	*argv[];
{
    int	i, n, f, j, k, t, maximum, nmaximum;
    int	MenosDD, Menost, Menosf, Menosall, Menosblock, Menosk, MenosSeed, MenosGlobal, Menosx;
    unsigned long	fhash(), gera_rand();
    double xk, global_percentage = 0.0;
    int long seed = 0;
    char *Lista;

   n = argc-3;
   strcpy(ArqTeste, argv[argc-1]);

   for (j = 0; j < NOPERADORES; j++)
        g_tab_operador[j].percentage = 1.0;
   f = MAXINTEGER;
   t = 0;
    Menost = Menosf = Menosblock = Menosall = Menosk = MenosSeed = MenosGlobal = FALSE;
   strcpy(DirCorrente, ".");
   inic_to_buf(Funcoes, sizeof(Funcoes));

   for (i = 2; i < argc-1; i++)
   {
        if (strcmp(argv[i], "-k") == 0)
        {
                Menosk = TRUE;
                argv[i] = "";
                n --;
        }
   }


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

   for (i = 2; i < argc-2; i++)
   {
        if (strcmp(argv[i], "-O") == 0)
        {
                if (LeArqOp(DirOp, ArqOp, g_tab_operador) == ERRO)
                        exit(1);
                n -= 2;
                argv[i] = argv[i+1] = "";
        }
        else

        if (strcmp(argv[i], "-f") == 0)
        {
                t = MAXI(atoi(argv[i+1]), t);
                Menost = TRUE;
                argv[i] = argv[i+1] = "";
                n -= 2;
        }
        else

            if (strcmp(argv[i], "-seed") == 0)
            {
                seed = atol(argv[i+1]);
                MenosSeed = TRUE;
                argv[i] = argv[i+1] = "";
                n -= 2;
            }
        else


	if (strcmp(argv[i], "-t") == 0)
	{
		f = atoi(argv[i+1])+1;
		Menosf = TRUE;
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
	else

        if (strcmp(argv[i], "-block") == 0)
        {
                maximum = MINI(atoi(argv[i+1]), f);
                nmaximum = MINI(atoi(argv[i+2]), f);
                Menosblock = TRUE;
                argv[i] = argv[i+1] = argv[i+2] = "";
                n -= 3;
	}
	else

	if (strcmp(argv[i], "-all") == 0)
	{
                            xk = atof(argv[i+1]);
	   for (j = 0; j < NOPERADORES; j++)
                                g_tab_operador[j].percentage = xk;
	   argv[i] = argv[i+1] = "";
	   n -= 2;
	   Menosall = TRUE;
	}
	else
                        if (strcmp(argv[i], "-global") == 0)
                        {
                            global_percentage = atof(argv[i+1]);
                            argv[i] = argv[i+1] = "";
                            n -= 2;
                            MenosGlobal = TRUE;
                        }
                        else
                        if (strcmp(argv[i], "-x") == 0)
                        {
                            ;
                        }
                        else

	if (*argv[i] == '-' && (j = busca_op(&(argv[i][1]) )) >= 0)
	{
           do
           {
                                    xk = atof(argv[i+1]);
                                    g_tab_operador[j].percentage = xk;
                j = next_match(&(argv[i][1]), j);
                                }
                                while (j >= 0);

	   argv[i] = argv[i+1] = "";
	   n -= 2;
	   Menosall = TRUE;
	}
        else

        if (strcmp(argv[i], "-c") == 0)
        {
           sprintf(auxbuf, "%s$%s", argv[i+1], argv[i+2]);
           to_buf(auxbuf);
           argv[j] = "";
           argv[i] = argv[i+1] = argv[i+2] = "";
           n -= 3;
        }

   }

   Menosx = 0;
   for (i = 2; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-x") == 0)
	{
	   Lista = argv[i+1];
	   do {
		while (isspace(*Lista)) Lista++;
		k = 0;
		if (*Lista != '\0')
		{
		    while (isdigit(Lista[k])) k++;
		    if (k > 0 && (isspace(Lista[k]) || Lista[k] == '\0'))
		    {
			sscanf(Lista, "%d", &j);
			if ( ! Menost || (j < t || j > f) )
			   if (Menosx < LEN(VetMenosx))
			   	VetMenosx[Menosx++] = j;
			   else
				msg("Maximum list size exceded");
		    }
		    else
		    {
			erro:
			msg("Invalid argument -x");
			exit(1);
		    }
		}
		Lista = &Lista[k];
	      } while (*Lista != '\0');
	   if (Menosx <= 0) goto erro;

 	   argv[i] = argv[i+1] = "";
	   n -= 2;
	}
   }


   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

   if (Menosall && Menosblock)
   {
	msg("Impossible to select by block and by operator at the same time");
	exit(1);
   }



    /*----------------------------- Carrega arquivo de teste ---------*/
   if (carrega_arquivo_teste(DirCorrente, ArqTeste, &pteste_cab) == ERRO)
	return;


    /*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqTeste) == ERRO)
	return;
   f = MINI(f, GERADOS(&HMuta));


    /*------------------ Atua sobre os mutantes selecionados -----*/
   
   strcpy(auxbuf2, pteste_cab.fonte_expa);
   strcat(auxbuf2, SUFIXO_FONTE);
   if (Menosblock)
     sele_byblock_mutante(&HMuta, f, maximum, nmaximum, Menosk);
   else
    if (Menosx)
   {
        sele_by_number(&HMuta, t, f, VetMenosx, Menosx);
    }
    else
     {
        if (MenosGlobal)
        { // nesse caso o op 0 guarda a porcentagem global. assim nao precisa mudar o sorteio
            g_tab_operador[0].percentage = MINI(global_percentage, 1.0);
            init_sort(-1, seed);
        }
       else
        for (i = 0; i < NOPERADORES; i++)
        {
            g_tab_operador[i].percentage = MINI(g_tab_operador[i].percentage, 1.0);
            init_sort(i, seed);
     }

        sele_byop_mutante(&HMuta, t, f, Menosk, seed);
   }


   descarrega_arquivo_mutante(&HMuta);


}


/***************************************************************************
	Le parametros da opcao -build do programa

***************************************************************************/
BuildSourceMuta(argc, argv)
int	argc;
char	*argv[];
{
int	Menosx;
static	int VetMenosx[200];
int	i,j, k, n;
int	Menose;
char	*Lista, *p, *q, *monta_n_mutante(), *monta_exec();

   strcpy(ArqTeste, argv[argc-1]);
   n = argc - 3;
   strcpy(DirCorrente, ".");
   Menose = FALSE;

   if (getenv(STR_DIR_HOME) != NULL)
      strcpy(DirHome, getenv(STR_DIR_HOME));
   else
      DirHome[0] = '\0';

   Menose = FALSE;
   for (i = 2; i < argc-1; i++)
   {
	if (strcmp(argv[i], "-e") == 0)
	{
		argv[i] = "";
		Menose = TRUE;
		n--;
	}
   }

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
		n -= 2;
	}
	else


  	if (strcmp(argv[i], "-H") == 0)
	{
	   if (! tem_dir(argv[i+1]))
	   {
		msg("Invalid directory on parameter -H");
		exit(1);
	   }
           strcpy(DirHome, argv[i+1]);
	   argv[i] = argv[i+1] = "";
	   n -= 2;
	}

   }


   Menosx = 0;

   for (i = 2; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-x") == 0)
	{
	   Lista = argv[i+1];
	   do {
		while (isspace(*Lista)) Lista++;
		k = 0;
		if (*Lista != '\0')
		{
		    while (isdigit(Lista[k])) k++;
		    if (k > 0 && (isspace(Lista[k]) || Lista[k] == '\0'))
		    {
			sscanf(Lista, "%d", &j);
			if (Menosx < LEN(VetMenosx))
			   VetMenosx[Menosx++] = j;
			else
			   msg("Maximum list size exceded");
		    }
		    else
		    {
			msg("Invalid argument -x");
			exit(1);
		    }
		}
		Lista = &Lista[k];
	      } while (*Lista != '\0');
		    
 	   argv[i] = argv[i+1] = "";
	   n -= 2;
	}
   }


   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

   if (Menosx <= 0)
	return;

/*----------------------------- Carrega arquivo de teste ---------*/
   if (carrega_arquivo_teste(DirCorrente, ArqTeste, &pteste_cab) == ERRO)
	return;


/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqTeste) == ERRO)
	return;


/*------------------ Atua sobre os mutantes selecionados -----*/

   p = monta_n_mutante(&HMuta, &pteste_cab, Menosx, VetMenosx);
   if (p != NULL)
   {
	printf("\nSource File: %s", p);

   	if (Menose)
   	{
		q = monta_exec(p, &pteste_cab);
		if (q != NULL)
	   	   printf("\Exec. File: %s", q);
   	}
   }

   descarrega_arquivo_mutante(&HMuta);

}


UpdateMuta(argc, argv)
int     argc;
char    *argv[];
{
int     i, n;

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


   atualiza_mutantes(&HMuta);


   descarrega_arquivo_mutante(&HMuta);
   descarrega_arquivo_tcase(&HTCase);

}


/***************************************************************************
	Le parametros da opcao -build-view do programa

***************************************************************************/
BuildViewMuta(argc, argv)
int	argc;
char	*argv[];
{
int	Menosx;
static	int VetMenosx[200];
int	i,j, k, n;
int	Menose;
char	*Lista, *p, *q, *monta_view_mutante(), *monta_exec();

   strcpy(ArqTeste, argv[argc-1]);
   n = argc - 3;
   strcpy(DirCorrente, ".");
   Menose = FALSE;

   if (getenv(STR_DIR_HOME) != NULL)
      strcpy(DirHome, getenv(STR_DIR_HOME));
   else
      DirHome[0] = '\0';


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
		n -= 2;
	}
	else


  	if (strcmp(argv[i], "-H") == 0)
	{
	   if (! tem_dir(argv[i+1]))
	   {
		msg("Invalid directory on parameter -H");
		exit(1);
	   }
           strcpy(DirHome, argv[i+1]);
	   argv[i] = argv[i+1] = "";
	   n -= 2;
	}

   }


   Menosx = -1;

   for (i = 2; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-x") == 0)
	{
	   Menosx = atoi(argv[i+1]);		    
 	   argv[i] = argv[i+1] = "";
	   n -= 2;
	}
   }


   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

   if (Menosx < 0)
	return;

/*----------------------------- Carrega arquivo de teste ---------*/
   if (carrega_arquivo_teste(DirCorrente, ArqTeste, &pteste_cab) == ERRO)
	return;


/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqTeste) == ERRO)
	return;


/*------------------ Atua sobre os mutantes selecionados -----*/

   p = monta_view_mutante(&HMuta, &pteste_cab, Menosx);
   if (p != NULL)
   {
	printf("\nSource File: %s", p);

      }

    descarrega_arquivo_mutante(&HMuta);

}


/***************************************************************************
	Le parametros da opcao -compile do programa

***************************************************************************/
CompileMuta(argc, argv)
int	argc;
char	*argv[];
{
int	f, i, n, t;
int	Menost, Menosf, MenosQ;
char	*Menosv = NULL;

   n = argc-3;
   strcpy(ArqTeste, argv[argc-1]);

   f = MAXINTEGER;
   t = 0;
   Menost = Menosf = MenosQ = FALSE;
   strcpy(DirCorrente, ".");

   if (getenv(STR_DIR_HOME) != NULL)
      strcpy(DirHome, getenv(STR_DIR_HOME));
   else
      DirHome[0] = '\0';
   

   MenosQ = 1000;
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


        if (strcmp(argv[i], "-f") == 0)
        {
                t = MAXI(atoi(argv[i+1]), t);
                Menost = TRUE;
                argv[i] = argv[i+1] = "";
                n -= 2;
        }
        else


	if (strcmp(argv[i], "-t") == 0)
	{
		f = MINI(atoi(argv[i+1])+1, f);
		Menosf = TRUE;
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
	else

        if (strcmp(argv[i], "-v") == 0)
        {
                Menosv = argv[i+1];
                argv[i] = argv[i+1] = "";
                n-=2;
        }
	else

  	if (strcmp(argv[i], "-H") == 0)
	{
	   if (! tem_dir(argv[i+1]))
	   {
		msg("Invalid directory on parameter -H");
		exit(1);
	   }
           strcpy(DirHome, argv[i+1]);
	   argv[i] = argv[i+1] = "";
	   n -= 2;
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

    f = MINI(f, GERADOS(&HMuta));

   signal(SIGINT, interrup_exec);
   compila_mutante(t, f, 100, Menosv);


   descarrega_arquivo_mutante(&HMuta);
   descarrega_arquivo_tcase(&HTCase);

}


/***************************************************************************
	Le parametros da opcao -build-one do programa

***************************************************************************/
BuildSourceMutaOne(argc, argv)
int	argc;
char	*argv[];
{
int	Menosx;
int 	VetMenosx;
int	i,j, k, n;
int	Menose;
char	*Lista, *p, *q, *monta_mutante();

   strcpy(ArqTeste, argv[argc-1]);
   n = argc - 3;
   strcpy(DirCorrente, ".");

   if (getenv(STR_DIR_HOME) != NULL)
      strcpy(DirHome, getenv(STR_DIR_HOME));
   else
      DirHome[0] = '\0';

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
            n -= 2;
        }
        else


            if (strcmp(argv[i], "-H") == 0)
            {
                if (! tem_dir(argv[i+1]))
                {
                    msg("Invalid directory on parameter -H");
                    exit(1);
                }
                strcpy(DirHome, argv[i+1]);
                argv[i] = argv[i+1] = "";
                n -= 2;
            }

    }

   VetMenosx = -1;

   for (i = 2; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-x") == 0)
	{
		VetMenosx = atoi(argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
   }


   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

   if (Menosx < 0)
	return;

    /*----------------------------- Carrega arquivo de teste ---------*/
   if (carrega_arquivo_teste(DirCorrente, ArqTeste, &pteste_cab) == ERRO)
	return;


    /*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqTeste) == ERRO)
	return;


    /*------------------ Atua sobre os mutantes selecionados -----*/

   p = monta_mutante(&HMuta, &pteste_cab, VetMenosx);
   if (p != NULL)
   {
        printf("\nSource File: %s\n", p);
    }

    descarrega_arquivo_mutante(&HMuta);

}


/***************************************************************************
	Le parametros da opcao -invert do programa

***************************************************************************/
InvertMuta(argc, argv)
int	argc;
char	*argv[];
{
    int	f, i, n, t;
    int	Menost, Menosf;

    n = argc-3;
    strcpy(ArqTeste, argv[argc-1]);

    f = MAXINTEGER;
    t = 0;
    Menost = Menosf =  FALSE;
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


            if (strcmp(argv[i], "-f") == 0)
            {
                t = MAXI(atoi(argv[i+1]), t);
                Menost = TRUE;
                argv[i] = argv[i+1] = "";
                n -= 2;
   }
            else


                if (strcmp(argv[i], "-t") == 0)
                {
                    f = MINI(atoi(argv[i+1])+1, f);
                    Menosf = TRUE;
                    argv[i] = argv[i+1] = "";
                    n -= 2;
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

    f = MINI(f, GERADOS(&HMuta));

    inverte_mutante(&HMuta, t, f);


   descarrega_arquivo_mutante(&HMuta);
    descarrega_arquivo_tcase(&HTCase);

}



/*********************************************************************
INTERRUP_EXEC:
	This function catch SIGINT signal and interrupt mutation 
execution.

***********************************************************************/
void interrup_exec()
{
   flg_interrup = TRUE;
   signal(SIGINT, interrup_exec);
}

msg(x)
char *x;
{
   fprintf(stderr, "\nexemuta: %s\n", x);
}

