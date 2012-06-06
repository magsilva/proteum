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
#include	"lib/muta.h"


#define		MAXINTEGER	(((unsigned) -1) >> 1)

char      function[FUNC_NAME+1];
char	  param[FUNC_NAME+1];

HAND_MUTANTE	HMuta;	/* manipulador do arquivo de mutantes */

char	ArqMuta[NOME_LENGTH+1],		/* nome do arquivo de mutantes */
	DirCorrente[NOME_LENGTH+1]; 	/* dir corrente */

extern	OPERADOR_MUTACAO	g_tab_operador[];


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


   if (strcmp(argv[1], "-create") == 0)
   {
	argv[1] = "";
	CreateMuta(argc, argv);
   }
   else
   if (	strcmp(argv[1], "-l") == 0 )
   {
	ListMuta(argc, argv);
   }
   else
   if (	strcmp(argv[1], "-equiv") == 0 )
   {
	SetEquivMuta(argc, argv, TRUE);
   }
   else
   if (	strcmp(argv[1], "-nequiv") == 0 )
   {
	SetEquivMuta(argc, argv, FALSE);
   }
   else
   if ( strcmp(argv[1], "-anomalous") == 0 )
   {
        SetAnomalous(argc, argv, TRUE);
   }
   else
   if ( strcmp(argv[1], "-nanomalous") == 0 )
   {
        SetAnomalous(argc, argv, FALSE);
   }
   else
   if ( strcmp(argv[1], "-active") == 0 )
   {
        SetActive(argc, argv, TRUE);
   }
   else
   if ( strcmp(argv[1], "-nactive") == 0 )
   {
        SetActive(argc, argv, FALSE);
   }
   else
   if (strcmp(argv[1], "-add") == 0)
   {
	AddMuta(argc, argv);
   }
   else
   if (strcmp(argv[1], "-up") == 0)
   {
	SearchMuta(argc, argv, 1);
   }
   else
   if (strcmp(argv[1], "-down") == 0)
   {
	SearchMuta(argc, argv, -1);
   }
   else
   if (strcmp(argv[1], "-ms") == 0)
   {
        QueryMuta(argc, argv);
   }
   else
   if (strcmp(argv[1], "-import") == 0)
   {
        ImportMuta(argc, argv);
   }
   else
   if (strcmp(argv[1], "-searchall") == 0)
   {
        SearchAllMuta(argc, argv, 1);
   }
   else
  {
        msg("Missing parameter");
        exit(1);
   }
   return 0;
 

}



/***************************************************************************
	Le parametros da opcao -create do programa

***************************************************************************/
CreateMuta(argc, argv)
int	argc;
char	*argv[];
{
int	i, n;

   n = argc-3;
   strcpy(ArqMuta, argv[argc-1]);

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

/*----------------------------- Cria arquivos de mutantes ------------*/

   if (cria_arquivo_mutante(&HMuta, DirCorrente, ArqMuta) == ERRO)
	exit(1);

   descarrega_arquivo_mutante(&HMuta);

}

/***************************************************************************
	Le parametros da opcao -add do programa

***************************************************************************/
AddMuta(argc, argv)
int	argc;
char	*argv[];
{
int	i, j, n, Tipo, No, Seq1, Seq2;
char	c;
static	char	BufLe[1024], BufLe2[1024];
DESCR_MUTANTE	descr_aux;
int	OpMuta, Percent, Tamanho, t;
long	Offset;
long	IniFunc, Calling, Called, TamFunc;
int	IniName, TamDcl, TamCab, EVoid, x;


   n = argc-3;
   strcpy(ArqMuta, argv[argc-1]);

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

/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqMuta) == ERRO)
	exit(1);

/*---------------------- Le descritores e inclui no arquivo ------------*/

   for (i = 0; i < NOPERADORES; i++)
	g_tab_operador[i].percent = 0;

   do
   {
      if (scanf("%ld", &IniFunc) != 1)
	break;
      if (IniFunc < 0)
	break;
      if (scanf("%d %d %d %ld %d %s", &IniName, &TamDcl, &TamCab,
		  &TamFunc, &EVoid, &function) != 6)
          break;
      if (gets(param) == NULL)
	  break;

      if (add_descr_func(&HMuta, IniFunc, function, param, IniName, TamDcl, 
			 TamCab,TamFunc, EVoid) == ERRO)
	  break;
    } while (1);

    if (scanf("%ld", &Calling) != 1)
	Calling = -1;

     while ( Calling >= 0)
     {    
	if (scanf("%ld %d %d %d", &Called, &OpMuta, &Seq1, &Seq2) != 4)
	   break;
	if (scanf("%d", &Tipo) != 1)
	   break;
	t = 0;
	if ( Tipo == CONNECT_DESCR )
	{
                scanf("%ld %d %d", &Offset, &Tamanho, &No);
                do
                {
                    scanf("%c", &c);
                 } while (c != '\n');
		 gets(BufLe2);

                 t = gera_descritor(&descr_aux, Tipo, Offset, 
						      Tamanho, No, BufLe2);
                 scanf("%d", &Tipo);
        }


	while ( Tipo >= 0)	/* se for < 0, terminou para esse op */
	{
		if (Tipo != OPER_DESCR)
		{
		   msg("Invalid Descriptor");
		   return;
		}
		scanf("%ld %d %d", &Offset, &Tamanho, &No);	
		
		do 
		{
	  	    scanf("%c", &c);
		 } while (c != '\n');

		 BufLe2[0] = '\0';
		 while (! EOChange(fgets(BufLe, 1024, stdin)))
		 {
		   strcat(BufLe2, BufLe);
		 }
		 j = strlen(BufLe2) - 1;
		 BufLe2[j] = '\0';	/* tira ultimo \n, que nao pertence
						ao string */
		   
		 if (t == 0)
		   t = gera_descritor(&descr_aux, Tipo, Offset, 
							Tamanho, No, BufLe2);
		 else
		   t += more_descritor(&descr_aux, Tipo, Offset, 
							Tamanho, No, BufLe2);

 	         scanf("%d", &Tipo);
	} 

/*------------------------ Insere o mutante ---------------------------*/
	if (ins_mutante(&HMuta, OpMuta, Calling, Called, Seq1, Seq2,
			&descr_aux, t) == ERRO)
	      goto fim;

    	if (scanf("%ld", &Calling) != 1)
           Calling = -1;

    }

    fim:
    descarrega_arquivo_mutante(&HMuta);

}


/***************************************************************************
	Le parametros da opcao -l  do programa

***************************************************************************/
ListMuta(argc, argv)
int	argc;
char	*argv[];
{
int	Menosx;
static	int VetMenosx[1200];
int	f, i,j, k, n, t;
int	Menost, Menosf;
char	*Lista;

   strcpy(ArqMuta, argv[argc-1]);
   n = argc - 3;
   strcpy(DirCorrente, ".");
   Menost = Menosf = FALSE;
   f = MAXINTEGER;
   t = 0;

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
		f = MINI(atoi(argv[i+1]), f);
		Menosf = TRUE;
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
   }

   Menost = (Menost || Menosf);



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

   if (Menosx > 0 && ! Menost)
   	f = -1;


   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqMuta) == ERRO)
	exit(1);

   f = MINI(f, GERADOS(&HMuta)-1);

/*------------------ Atua sobre os mutantes selecionados -----*/

   for (i = t; i <= f; i++)
   {
	List1Muta(i);
   }

   for (i = 0; i < Menosx; i++)
   {
	List1Muta(VetMenosx[i]);
   }



   descarrega_arquivo_mutante(&HMuta);

}
	


/****************************************************************************
Exibe o conteudo de 1 mutante
***************************************************************************/
List1Muta(k)
int	k;
{
int	i, j;
char	*p, *q;
DESCR_MUTANTE	*descr, *fim_descr();
char	xbuf[15];

   if (carrega_descr(&HMuta, k) == ERRO)
	return;

   printf("\n\nMUTANT # %d", k);
   i = get_mutante_status(&HMuta);
   switch(i)
   {
	case VIVO:
		p = "Alive";
		break;
	case MORTO:
		p = "Dead";
		break;
	case ANOMALO:
		p = "Anomalous";
		break;
	case EQUIV:
		p = "Equivalent";
		break;
	default:
		p = "???";
		break;
   }


   if (get_mutante_inactive(&HMuta))
	q = "Inactive";
   else
	q = "Active";
   printf("\n\tStatus %s %s", p, q);

   p = "None";
   if (i == MORTO)
   {
 	j = get_causa_mortis(&(REG(&HMuta)));
	switch (j)
	{
	   case EXEC_MORTO_TIMEOUT:
		p = "Timeout";
		break;
	   case EXEC_MORTO_TRAP:
		p = "Trap";
		break;	
	   case EXEC_MORTO_RETCOD:
		p = "Return-Code";
		break;
	   case EXEC_MORTO_SAIDA:
		p = "Stdout";
		break;
	   default: 
		p = xbuf;
		sprintf(xbuf, "Cod:%d", j);
		break;
	}

   }
   printf("\n\tCausa Mortis: %s - Test Case %d", p, i == MORTO ? REG(&HMuta).status1 : -1);

   printf("\n\tDescriptor size.: %d", REG(&HMuta).tamanho);
   printf("\n\tCalling function starts at: %ld", REG(&HMuta).calling);
   printf("\n\tCalled function starts at: %ld", REG(&HMuta).called);
   printf("\n\tSequencial numbering: %d %d",REG(&HMuta).seq1,REG(&HMuta).seq2);
   printf("\n\tLast test case used: %ld", REG(&HMuta).ultimo_tcase);
   i = REG(&HMuta).operador;
   printf("\n\tOperator: %d (%s)", i, g_tab_operador[i].nome);
   printf("\n\tDescriptor:");
   descr = &(DESCR(&HMuta));

   while (descr->offset != 0)
   {
	printf("\n\t\tTipo: %d", descr->tipo);
        printf("\n\t\tProgram graph node: %d", descr->no);
	printf("\n\t\tOffset: %ld, get out %ld characters", descr->offset, 
							descr->tamanho);
	printf("\n\t\tGet on: %s", descr->string);
  	descr = fim_descr(descr);
   }
   
}


/***************************************************************************
	Le parametros da opcao -equiv e -nequiv  do programa

***************************************************************************/
SetEquivMuta(argc, argv, equiv)
int	argc, equiv;
char	*argv[];
{
int	Menosx;
static	int VetMenosx[1200];
int	f, i,j, k, n, t;
int	Menost, Menosf;
char	*Lista;

   strcpy(ArqMuta, argv[argc-1]);
   n = argc - 3;
   strcpy(DirCorrente, ".");
   Menost = Menosf = FALSE;
   f = MAXINTEGER;
   t = 0;

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
		f = MINI(atoi(argv[i+1]), f);
		Menosf = TRUE;
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
   }

   Menost = (Menost || Menosf);



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

   if (Menosx > 0 && ! Menost)
   	f = -1;


   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqMuta) == ERRO)
	exit(1);

   f = MINI(f, GERADOS(&HMuta)-1);

/*------------------ Atua sobre os mutantes selecionados -----*/
   for (i = t; i <= f; i++)
   {
	if (carrega_descr(&HMuta, i) == ERRO)
	   break;
	troca_status_muta(&HMuta, equiv? EQUIV: VIVO);
	if (rewrite_mutante(&HMuta) == ERRO)
	   break;
   }

   for (i = 0; i < Menosx; i++)
   {
        if (carrega_descr(&HMuta, VetMenosx[i]) == ERRO)
           break;

	troca_status_muta(&HMuta, equiv? EQUIV: VIVO);
        if (rewrite_mutante(&HMuta) == ERRO)
           break;
   }


   descarrega_arquivo_mutante(&HMuta);

}


/***************************************************************************
        Le parametros da opcao -equiv e -nequiv  do programa

***************************************************************************/
SetAnomalous(argc, argv, anom)
int     argc, anom;
char    *argv[];
{
int     Menosx;
static  int VetMenosx[1200];
int     f, i,j, k, n, t, s;
int     Menost, Menosf;
char    *Lista;

   strcpy(ArqMuta, argv[argc-1]);
   n = argc - 3;
   strcpy(DirCorrente, ".");
   Menost = Menosf = FALSE;
   f = MAXINTEGER;
   t = 0;

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
                f = MINI(atoi(argv[i+1]), f);
                Menosf = TRUE;
                argv[i] = argv[i+1] = "";
                n -= 2;
        }
   }

   Menost = (Menost || Menosf);
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

   if (Menosx > 0 && ! Menost)
        f = -1;


   if ( n != 0)
   {
        msg("Invalid parameter");
        exit(1);
   }

/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqMuta) == ERRO)
        exit(1);

   f = MINI(f, GERADOS(&HMuta)-1);

   s = (anom ? VIVO : ANOMALO);
   anom = (anom ? ANOMALO: VIVO);

/*------------------ Atua sobre os mutantes selecionados -----*/
   for (i = t; i <= f; i++)
   {
        if (carrega_descr(&HMuta, i) == ERRO)
           break;
	j = get_mutante_status(&HMuta);
        if (j == s)
	{
	   set_mutante_status(&HMuta, anom, 0, 0);
           if (rewrite_mutante(&HMuta) == ERRO)
              break;
	}
   }

   for (i = 0; i < Menosx; i++)
   {
        if (carrega_descr(&HMuta, VetMenosx[i]) == ERRO)
           break;
        j = get_mutante_status(&HMuta);
        if (j == s)
        {
           set_mutante_status(&HMuta, anom, 0, 0);
           if (rewrite_mutante(&HMuta) == ERRO)
              break;
        }

   }


   descarrega_arquivo_mutante(&HMuta);

}

/***************************************************************************
        Le parametros da opcao -active e -nactive  do programa

***************************************************************************/
SetActive(argc, argv, anom)
int     argc, anom;
char    *argv[];
{
int     Menosx;
static  int VetMenosx[1200];
int     f, i,j, k, n, t, s;
int     Menost, Menosf;
char    *Lista;

   strcpy(ArqMuta, argv[argc-1]);
   n = argc - 3;
   strcpy(DirCorrente, ".");
   Menost = Menosf = FALSE;
   f = MAXINTEGER;
   t = 0;

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
                f = MINI(atoi(argv[i+1]), f);
                Menosf = TRUE;
                argv[i] = argv[i+1] = "";
                n -= 2;
        }
   }

   Menost = (Menost || Menosf);
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

   if (Menosx > 0 && ! Menost)
        f = -1;


   if ( n != 0)
   {
        msg("Invalid parameter");
        exit(1);
   }

/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqMuta) == ERRO)
        exit(1);

   f = MINI(f, GERADOS(&HMuta)-1);

   s =  ! anom;

/*------------------ Atua sobre os mutantes selecionados -----*/
   for (i = t; i <= f; i++)
   {
        if (carrega_descr(&HMuta, i) == ERRO)
           break;
	j = get_mutante_inactive(&HMuta);
        if (j != s)
	{
	   set_mutante_inactive(&HMuta, s);
           if (rewrite_mutante(&HMuta) == ERRO)
              break;
	}
   }

   for (i = 0; i < Menosx; i++)
   {
        if (carrega_descr(&HMuta, VetMenosx[i]) == ERRO)
           break;
        j = get_mutante_inactive(&HMuta);
        if (j != s)
        {
           set_mutante_inactive(&HMuta, s);
           if (rewrite_mutante(&HMuta) == ERRO)
              break;
        }

   }


   descarrega_arquivo_mutante(&HMuta);

}



/***************************************************************************
	Print mutation score
***************************************************************************/
QueryMuta(int argc, char *argv[])
{
int     f, i,j, k, n, t, s;
float	score;

   strcpy(ArqMuta, argv[argc-1]);
   n = argc - 3;
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
                n -= 2;
        }
    }

/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqMuta) == ERRO)
        exit(1);

   s = GERADOS(&HMuta) - INATIVOS(&HMuta) -
         ANOMALOS(&HMuta) - EQUIVS(&HMuta);

   score = (s == 0 ? 0.0 : (float) (s - VIVOS(&HMuta)) / (float) s);
   printf("\n%f\n", score);

   descarrega_arquivo_mutante(&HMuta);

}

/**************************************************************************
			Le parametros da opcao -search
**************************************************************************/
SearchMuta(argc, argv, incr)
int	argc, incr;
char	*argv[];
{
int	f, i,j, k, n, t;
int	Menost, Menosf;
int	MenosAlive, MenosDead, MenosEquiv, MenosAnom, MenosInactive;

   strcpy(ArqMuta, argv[argc-1]);
   n = argc - 3;
   strcpy(DirCorrente, ".");
   Menost = Menosf = FALSE;
   f = MAXINTEGER;
   t = 0;
   MenosAlive = MenosDead = MenosEquiv = MenosAnom = MenosInactive = FALSE;

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
		f = MINI(atoi(argv[i+1]), f);
		Menosf = TRUE;
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
   }

   Menost = (Menost || Menosf);

   for (i = 2; i < argc-1; i++)
   {
	if (strcmp(argv[i], "-alive") == 0)
	{
	   MenosAlive = TRUE;
	   argv[i] = "";
	   n --;
	}
	else

	if (strcmp(argv[i], "-dead") == 0)
	{
	   MenosDead = TRUE;
	   argv[i] = "";
	   n --;
	}
	else

	if (strcmp(argv[i], "-equiv") == 0)
	{
	   MenosEquiv = TRUE;
	   argv[i] = "";
	   n --;
	}
	else

	if (strcmp(argv[i], "-anomalous") == 0)
	{
	   MenosAnom = TRUE;
	   argv[i] = "";
	   n --;
	}
	else

	if (strcmp(argv[i], "-inactive") == 0)
	{
	   MenosInactive = TRUE;
	   argv[i] = "";
	   n --;
	}
   }

   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqMuta) == ERRO)
	exit(1);

   f = MINI(f, GERADOS(&HMuta)-1);

/*------------------ Atua sobre os mutantes selecionados -----*/
   for (i = t; i <= f && i >= 0; i += incr)
   {
	if (carrega_descr(&HMuta, i) == ERRO)
	   break;

	if ( ! MenosInactive && get_mutante_inactive(&HMuta))
	   continue;

	j = get_mutante_status(&HMuta);
	if ( (MenosAlive && j == VIVO) || (MenosDead && j == MORTO) ||
	     (MenosEquiv && j == EQUIV) || (MenosAnom && j == ANOMALO) )
	{
	   printf(" %d ", i);
	   break;
	}
   }


   descarrega_arquivo_mutante(&HMuta); 

}

/**************************************************************************
			Le parametros da opcao -searchall
**************************************************************************/
SearchAllMuta(argc, argv, incr)
int	argc, incr;
char	*argv[];
{
int	f, i,j, k, n, t;
int	Menost, Menosf;
int	MenosAlive, MenosDead, MenosEquiv, MenosAnom, MenosInactive;
char 	*op;

   strcpy(ArqMuta, argv[argc-1]);
   n = argc - 3;
   strcpy(DirCorrente, ".");
   Menost = Menosf = FALSE;
   f = MAXINTEGER;
   t = 0;
   MenosAlive = MenosDead = MenosEquiv = MenosAnom = MenosInactive = FALSE;

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
		f = MINI(atoi(argv[i+1]), f);
		Menosf = TRUE;
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
   }

   Menost = (Menost || Menosf);

   for (i = 2; i < argc-1; i++)
   {
	if (strcmp(argv[i], "-alive") == 0)
	{
	   MenosAlive = TRUE;
	   argv[i] = "";
	   n --;
	}
	else

	if (strcmp(argv[i], "-dead") == 0)
	{
	   MenosDead = TRUE;
	   argv[i] = "";
	   n --;
	}
	else

	if (strcmp(argv[i], "-equiv") == 0)
	{
	   MenosEquiv = TRUE;
	   argv[i] = "";
	   n --;
	}
	else

	if (strcmp(argv[i], "-anomalous") == 0)
	{
	   MenosAnom = TRUE;
	   argv[i] = "";
	   n --;
	}
	else

	if (strcmp(argv[i], "-inactive") == 0)
	{
	   MenosInactive = TRUE;
	   argv[i] = "";
	   n --;
	}
   }

   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqMuta) == ERRO)
	exit(1);

   f = MINI(f, GERADOS(&HMuta)-1);

/*------------------ Atua sobre os mutantes selecionados -----*/
   for (i = t; i <= f && i >= 0; i += incr)
   {
	if (carrega_descr(&HMuta, i) == ERRO)
	   break;

	if ( ! MenosInactive && get_mutante_inactive(&HMuta))
	   continue;

	j = get_mutante_status(&HMuta);
	op = g_tab_operador[REG(&HMuta).operador].nome;
	if ( (MenosAlive && j == VIVO) || (MenosDead && j == MORTO) ||
	     (MenosEquiv && j == EQUIV) || (MenosAnom && j == ANOMALO) )
	{
	   printf("\n%d %ld %ld %s %d %d", i, REG(&HMuta).calling,
	    REG(&HMuta).called, op, REG(&HMuta).seq1,
	    REG(&HMuta).seq2);
	}
   }


   descarrega_arquivo_mutante(&HMuta);

}
	

/***************************************************************************
	Le parametros da opcao -import  do programa

***************************************************************************/
ImportMuta(argc, argv)
int	argc;
char	*argv[];
{
int	Menosx;
static	int VetMenosx[1200];
int	i,j, k, n;
long	calling, called;
int 	oper, seq1, seq2;
char 	opc[] = {'u','-','-','-','-','-','-','-','-','-',
                 '-','-','-','-','-','-','-','-','-','-',
                 '-','-','-','-','-','-','-','-','-','-'};

   strcpy(ArqMuta, argv[argc-1]);
   n = argc - 3;
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
		n -= 2;
	}

  }
   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqMuta) == ERRO)
	exit(1);


/*------------------ Atua sobre os mutantes selecionados -----*/
   for(scanf("%d %ld %ld %s %d %d", &n, &calling, &called,
              &opc[2], &seq1, &seq2); n >= 0;
       scanf("%d %ld %ld %s %d %d", &n, &calling, &called,
              &opc[2], &seq1, &seq2) )
   {
       oper = busca_op(&opc[2]);
       if (oper < 0)
          oper = busca_op(opc); // poe o u-
       if (oper < 0)
           printf("\nOperator %s not found", opc);
       else
       if (carrega_descr_key(&HMuta, calling, called,
   		oper,seq1, seq2) == ERRO)
       {
            printf("\nNot found mutant %d", n);
  	    printf("\n\tCalling: %ld Called: %ld", calling, called);
            printf("\n\tOper: %s seq1: %d seq2: %d", opc, seq1,seq2);
       }
       else
       {
          set_mutante_status(&HMuta, EQUIV, 0,0);
          rewrite_mutante(&HMuta);
       }
       n = -1;
   }


   descarrega_arquivo_mutante(&HMuta);
}


/*************************************************************************/

EOChange(x)
char	x[];
{
   return (x[0] == '\0' && x[1] == '\n');
}


msg(x)
char *x;
{
   fprintf(stderr, "\nmuta: %s\n", x);
}


