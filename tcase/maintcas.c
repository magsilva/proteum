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
#include	"lib/tcase.h"

extern char *from_buf();

HAND_TCASE	HTCase,		/* structure to handle tcase file */
		HTCase2;


char	ArqTeste[NOME_LENGTH+1], /* nome dos arquivos de casos de teste e E/S */
	DirCorrente[NOME_LENGTH+1], /* dir corrente */
	DirExec[NOME_LENGTH+1],	 /* diretorio do arquivo a executar */
	ArqExec[NOME_LENGTH+1],	 /*  arquivo a executar */
	ArqInstrum[NOME_LENGTH+1], /* arquivo executavel instrumentado */
	DirImport[NOME_LENGTH+1], /*  dir. de onde importar Casos de Teste */
	ArqImport[NOME_LENGTH+1], /*  arquivo de onde importar Casos de Teste */
        ArqParam[NOME_LENGTH+1], /*  arquivo de onde pegar parametros */
	Parametros[PARAMSIZE],
	XLabel[NOME_LENGTH+1],
	Label[LABELSIZE];

int	MenosP,
	Menosp,
	Menost,
	Menoslabel,
	Menosf,
	Menosv,
	Menosinter,
	Menostrace;

char	verchar;

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
	CreateTcase(argc, argv);
   }
   else
   if (	strcmp(argv[1], "-l") == 0 ||
	strcmp(argv[1], "-d") == 0 ||
	strcmp(argv[1], "-e") == 0 ||
	strcmp(argv[1], "-i") == 0)
   {
	ListTcase(argc, argv);
   }
   else
   if (	strcmp(argv[1], "-proteum") == 0 ||
	strcmp(argv[1], "-poke") == 0 ||
	strcmp(argv[1], "-ascii") == 0 )
   {
	ImportTcase(argc, argv);
   }
   else
   if (strcmp(argv[1], "-add") == 0)
   {
	argv[1] = "";
	AddTcase(argc, argv);
   }
   else
   if (strcmp(argv[1], "-z") == 0)
   {
        argv[1] = "";
        ZapTcase(argc, argv);
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
CreateTcase(argc, argv)
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

/*----------------------------- Cria arquivos de casos de teste ------------*/

   if (cria_arquivo_tcase(&HTCase, DirCorrente, ArqTeste) == ERRO)
	exit(1);

   descarrega_arquivo_tcase(&HTCase);

}



/***************************************************************************
	Le parametros das opcoes -l -e -d -i do programa

***************************************************************************/
ListTcase(argc, argv)
int	argc;
char	*argv[];
{
int	Menosx;
static	int VetMenosx[MAX_TCASE];
int	Menosl, Menose, Menosi, Menosp, Menosd;
int	f, i,j, k, n, t;
char	*Lista;

   strcpy(ArqTeste, argv[argc-1]);
   n = argc - 3;
   strcpy(DirCorrente, ".");
   Menost = Menosf = Menosp = FALSE;
   f = MAX_TCASE;
   t = 1;

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

   for (i = 2; i < argc-1; i++)
   {
        if (strcmp(argv[i], "-p") == 0)
        {
                Menosp = TRUE;
                argv[i] = "";
                n--;
        }
   }

   Menost = (Menost || Menosf);



   Menosx = 0;
   for (i = 2; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-x") == 0)
	{
	   if (Menost)
	   {
		msg("Invalid conbination of -t or -f and -x");
		return;
	   }
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
			msg("Invalid argument -x");
			goto fim;
		    }
		}
		Lista = &Lista[k];
	      } while (*Lista != '\0');
		    
 	   argv[i] = argv[i+1] = "";
	   n -= 2;
	}
   }

   if (Menosx > 0 && ! Menost)
   	f = 0;
	

   Menosl = Menosd = Menosi = Menose = FALSE;
   switch(argv[1][1])
   {
	case 'l':
		Menosl = TRUE;
		break;
	case 'e':
		Menose = TRUE;
		break;

	case 'i':
		Menosi = TRUE;
		break;

	case 'd':
		Menosd = TRUE;
		break;
   }

   if (n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

/*------------------------------- Carrega arquivo de teste -------------*/

   if (carrega_arquivo_tcase(&HTCase, DirCorrente, ArqTeste) == ERRO)
	exit(1);

/*------------------ Atua sobre os casos de teste selecionados -----*/



   if (Menosd)
	for (i = f; i >= t; i--)
	   DeleteTCase(i);
   else

   for (i = t; i <= f; i++)
   {
	if (Menosl)
	   ListaTCase(i, Menosp);
	else
	if (Menose)
	    EnableTCase(i);
	else
	if (Menosi)
	   DisableTCase(i);
   }

   if (Menosd)
   {
	OrdenaDecr(VetMenosx, Menosx);
   }

   for (i = 0; i < Menosx; i++)
   {
	if (Menosl)
	   ListaTCase(VetMenosx[i], Menosp);
	else
	if (Menose)
	    EnableTCase(VetMenosx[i]);
	else
	if (Menosi)
	   DisableTCase(VetMenosx[i]);
	else
	   DeleteTCase(VetMenosx[i]);
   }



fim:
   descarrega_arquivo_tcase(&HTCase);
   
}


/*--------------------------- Lista Caso de teste -------------------------*/
ListaTCase(i, phis )
int	i, phis;
{
int	n, j, k, t;
struct vetfunc	*p;
char c;

   k = i;
   if (! phis) 
      k = ltofis_tcase(&HTCase, i);
   if (k == ERRO)
	return;
   if (le_tcase_reg(&HTCase, k) == ERRO)
	return;

   printf("\nTeste Case # %d %d", i, k);
   printf("\nLabel:\t%s", TREG(&HTCase).label);
   printf("\n\t%s", TAB_FIS(&HTCase)[k].desabili? "DISABLED": "ENABLED");
   if (TAB_FIS(&HTCase)[k].error)
	printf("\n\tTest case caused an ERROR");
   printf("\n\tExec. Time (CPU): %d", TREG(&HTCase).exetempo); 
   printf("\n\tExec. Time (total): %d", TREG(&HTCase).totaltempo);
   printf("\n\tReturn Code: %d", TREG(&HTCase).retcode);
   if (TAB_FIS(&HTCase)[k].error)
	printf(" (Failure with test case)");
   printf("\n\tParameters: %s", (char *) build_param(TREG(&HTCase).param));
   printf("\n\tInput Size: %d", TREG(&HTCase).tamanho_ent);
   printf("\n\tOutput Size: %d", TREG(&HTCase).tamanho_sai);
   printf("\n\tStderr Size: %d", TREG(&HTCase).tamanho_err);

   printf("\n\tInput:\n");
   posiciona(TFILEIO(&HTCase), TREG(&HTCase).apont_ent);
   for (j = 0; j < TREG(&HTCase).tamanho_ent; j++)
   {
	fscanf(TFILEIO(&HTCase), "%c", &c);
	printf("%c", c);
   }

/*   copy_file(TFILEIO(&HTCase), TREG(&HTCase).apont_ent, 
				TREG(&HTCase).tamanho_ent, stdout); */

   t = TREG(&HTCase).tamanho_ent;
   printf("\n\tOutput:\n");
   copy_file(TFILEIO(&HTCase), TREG(&HTCase).apont_ent + t, 
				TREG(&HTCase).tamanho_sai, stdout);
   t += TREG(&HTCase).tamanho_sai;
   printf("\n\tStderr:\n");
   copy_file(TFILEIO(&HTCase), TREG(&HTCase).apont_ent + t, 
				TREG(&HTCase).tamanho_err, stdout);
   printf("\nTravessed blocks:");
   tcase_get_log(&HTCase, -1, -1);
   if ( HTCase.log != NULL)
      for (n = 0; n < HTCase.log->nfunction; n++)
      {
	 p = &(HTCase.log->vetfunc[n]);
	 printf("\n\tFunction: %d\n\tBlocks: ", p->function);
	 for (j = 1; j <= p->nnode; j++)
	 {
	   if (get_log_bit(p->nodes, j))
		printf("%d ", j);
	 }
      }

   printf("\n");

}



/*--------------------------------- Habilita Caso de Teste ----------------- */
EnableTCase(i)
int	i;
{
int	k;

   k = ltofis_tcase(&HTCase, i);
   if (k == ERRO)
	return;
   enable_tcase(&HTCase, k);
}


   
/*------------------------------- Desabilita Caso de Teste ----------------- */
DisableTCase(i)
int	i;
{
int	k;

   k = ltofis_tcase(&HTCase, i);
   if (k == ERRO)
	return;
   disable_tcase(&HTCase, k);
}

/*------------------------------- Elimina Caso de Teste ----------------- */
DeleteTCase(i)
int	i;
{

     if (delete_tcase(&HTCase, i, i) == ERRO)
	msg("Error at deleting test case");
}

   



/***************************************************************************
	Le parametros das opcoes -proteum -poke e -ascii do programa

***************************************************************************/
ImportTcase(argc, argv)
int	argc;
char	*argv[];
{
int	i, k, MenosDD, MenosDE, MenosE, MenosEE, MenosI, n;

   n = argc - 3;
   strcpy(ArqTeste, argv[argc-1]);
   strcpy(DirCorrente, ".");
   MenosDD = MenosDE = MenosI = MenosE = MenosEE = Menostrace = FALSE;
   Menost = 1;
   Menosf = 0;
   Menosv = Menosinter = FALSE;
   for (i = 2; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-D") == 0)
	{
		if (! tem_dir(argv[i+1]))
		{
			msg("Invalid directory on parameter -D");
			exit(1);
       		}
		strcpy(DirCorrente, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
	else

	if (strcmp(argv[i], "-DE") == 0)
	{
		if (! tem_dir(argv[i+1]))
		{
			msg("Invalid directory on parameter -DE");
			exit(1);
       		}
		MenosDE = TRUE;
		strcpy(DirExec, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
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
		strcpy(DirImport, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
	else

	if (strcmp(argv[i], "-I") == 0 && strcmp(argv[1], "-poke") != 0)
	{
		MenosI = TRUE;
		strcpy(ArqImport, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n-=2;
	}
	else

        if (strcmp(argv[i], "-p") == 0 && strcmp(argv[1], "-ascii") == 0)
        {
                MenosI = TRUE;
                strcpy(ArqParam, argv[i+1]);
                argv[i] = argv[i+1] = "";
                n-=2;
        }
        else

        if (strcmp(argv[i], "-label") == 0 )
        {
                Menoslabel = TRUE;
                strcpy(Label, argv[i+1]);
                argv[i] = argv[i+1] = "";
                n-=2;
        }
        else

	if (strcmp(argv[i], "-E") == 0)
	{
		MenosE = TRUE;
		strcpy(ArqExec, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n-=2;
	}
	else


	if (strcmp(argv[i], "-EE") == 0)
	{
		MenosEE = TRUE;
		strcpy(ArqInstrum, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n-=2;
	}
	else

	if (strcmp(argv[i], "-f") == 0)
	{
		k = atoi(argv[i+1]);
		Menost = MAXI(k, Menost);
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
	else

	if (strcmp(argv[i], "-t") == 0)
	{
		k = atoi(argv[i+1]);
		Menosf = k;
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
        else

        if (strcmp(argv[i], "-v") == 0)
        {
                verchar = *argv[i+1];
                Menosv = TRUE;
                argv[i] = argv[i+1] = "";
                n -= 2;
        }

   }

   for (i = 0; i < argc-1; i++)
   {
	if (strcmp(argv[i], "-trace") == 0)
	{
	   Menostrace = TRUE;
	   argv[i] = "";
	   n--;
	}
	else
	if (strcmp(argv[i], "-inter") == 0)
	{
	   Menosinter = TRUE;
	   argv[i] = "";
	   n--;
	}
   }


   if (n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

   if (Menosf == 0)
	Menosf = Menost + MAX_TCASE;
   else
	Menosf = MINI(Menosf, Menost+MAX_TCASE);

   if ( ! MenosDE)
	strcpy(DirExec, DirCorrente);

   if ( ! MenosDD)
	strcpy(DirImport, DirCorrente);

   if (! MenosI)
	strcpy(ArqImport, ArqTeste);

   if (! MenosE)
	strcpy(ArqExec, ArqTeste);
   if (! MenosEE)
	strcpy(ArqInstrum, ArqTeste);

   if (strcmp(argv[1], "-proteum") == 0)
	ImportProteum(argc, argv);
   else
   if (strcmp(argv[1], "-poke") == 0)
	ImportPoke(argc, argv);
   else
	ImportAscii(argc, argv);


}


ImportProteum(argc, argv)
int	argc;
char	*argv[];
{
int	i, k, cont;
FILE	*fp_ascii;
char *c;


   if (carrega_arquivo_tcase(&HTCase, DirCorrente, ArqTeste) == ERRO)
	exit(1);
   if (carrega_arquivo_tcase(&HTCase2, DirImport, ArqImport) == ERRO)
   {
	descarrega_arquivo_tcase(&HTCase);
	exit(1);
   }

   Menosf = MINI(Menosf, NTCASE(&HTCase2));
   for (i = Menost, cont = 0; i <= Menosf; i++)
   {
    	k = ltofis_tcase(&HTCase2, i);
   	if (k == ERRO)
	   break;
   	if (le_tcase_reg(&HTCase2, k) == ERRO)
	   break;

	fp_ascii = criarq(DirCorrente, ArqTeste, SUFIXO_INPUT);
	if (fp_ascii == NULL)
	   break;
	if (copy_file(TFILEIO(&HTCase2), TREG(&HTCase2).apont_ent, 
		  TREG(&HTCase2).tamanho_ent, fp_ascii) == ERRO)
		break;
	fecharq(fp_ascii);


	if (exec_from_ascii(DirExec, ArqExec, ArqInstrum, TREG(&HTCase2).param, 
		    DirCorrente, ArqTeste, &(TREG(&HTCase)), 
		   20,  Menostrace, NULL, TREG(&HTCase2).interativo) == ERRO )
		   break;
	TREG(&HTCase).interativo = TREG(&HTCase2).interativo;

	inic_to_buf(TREG(&HTCase).param, sizeof(TREG(&HTCase).param));
	inic_from_buf(TREG(&HTCase2).param);
   	for (c = from_buf(); c != NULL; c = from_buf())
	{
    	  if (to_buf(c) == ERRO)
	    break;
   	}
	
        if (Menoslabel)
           sprintf(XLabel, "%s%d", Label, i);
        else
	   strcpy(XLabel, (char*) TREG(&HTCase2).label);

        if (strlen(XLabel) <= LABELSIZE)
           strcpy(TREG(&HTCase).label, XLabel);
        else
        {
           msg("Invalid label");
           TREG(&HTCase).label[0] = '\0';
        }

	if (insere_tcase(&HTCase,  Menostrace) == ERRO)
		break; 


	cont++;
	if (Menosv)
	{	
	   printf("%c", verchar);
	   fflush(stdout);
	}
    }

   printf("\n%d test cases imported from Proteum file\n", cont);

fim:
   descarrega_arquivo_tcase(&HTCase2);
   descarrega_arquivo_tcase(&HTCase);
}



ImportPoke()
{
FILE	*fp_ascii, *fp_par;
int	i, j, k, cont, TemPar;
char 	*c;




   if (carrega_arquivo_tcase(&HTCase, DirCorrente, ArqTeste) == ERRO)
	exit(1);

   for (i = Menost, cont = 0; i <= Menosf; i++)
   {

	sprintf(ArqImport, "input%d.tes", i);
	TemPar = FALSE;
	if (tem_arq(DirImport, ArqImport, ""))
	{
		fp_par = abrearq(DirImport, ArqImport, "", 0);
		if (fp_par == NULL)
			break;
	 	posifim(fp_par);
		k = ftell(fp_par);
		if (k >= sizeof(Parametros))
		{
		   msg("Invalid POKE-TOOL parameter file");
		   fecharq(fp_par);
		   continue;
		}
		posiciona(fp_par, 0);
		fgets(Parametros, (int) k, fp_par);
	        k = strlen(Parametros);
	        if (k > 0 && Parametros[k-1] == '\n')
		   Parametros[k-1] = '\0';
		fecharq(fp_par);
		TemPar = TRUE;
	}
	else
		strcpy(Parametros, "");

	sprintf(ArqImport, "tec%d.tes", i);
	if (tem_arq(DirImport, ArqImport, ""))
	{
		fp_par = abrearq(DirImport, ArqImport, "", 0);
		if (fp_par == NULL)
			break;
	}
	else
	{
		fp_par = NULL;
		if (! TemPar)
		   break;
	}

        fp_ascii = criarq(DirCorrente, ArqTeste, SUFIXO_INPUT);
        if (fp_ascii == NULL)
	{
	   l1:
	   if (fp_par)
		fecharq(fp_par);
           break;
	}

        if (fp_par != NULL)
        {
           posifim(fp_par);
           if (copy_file(fp_par, 0l, (int) ftell(fp_par), fp_ascii) == ERRO)
           {
                fecharq(fp_ascii);
                goto l1;
           }
	   fecharq(fp_par);
        }
        fecharq(fp_ascii);

       if (exec_from_ascii(DirExec, ArqExec, ArqInstrum, Parametros,
                        DirCorrente, ArqTeste, &(TREG(&HTCase)), 
			20,  Menostrace, SHELL, Menosinter) == ERRO )
		   break;
	TREG(&HTCase).interativo = Menosinter;

	inic_to_buf(TREG(&HTCase).param, sizeof(TREG(&HTCase).param));
	inic_from_buf(Parametros);
   	for (c = from_buf(); c != NULL; c = from_buf())
	{
    	  if (to_buf(c) == ERRO)
	    break;
   	}

        if (Menoslabel)
           sprintf(XLabel, "%s%d", Label, i);
        else
           ArqImport[0] = '\0';

	if (strlen(XLabel) <= LABELSIZE)
           strcpy(TREG(&HTCase).label, XLabel);
	else
	{
	   msg("Invalid label");
	   TREG(&HTCase).label[0] = '\0';
 	}


       if (insere_tcase(&HTCase,  Menostrace) == ERRO)
                break;

	cont++;
        if (Menosv)
        {
           printf("%c", verchar);
           fflush(stdout);
        }

    }

   printf("\n%d test cases imported from POKE-TOOL files\n", cont);

fim:
   descarrega_arquivo_tcase(&HTCase);
}



ImportAscii()
{
FILE	*fp_ascii, *fp2;
int	i, j, k, cont;
static char	buf1[NOME_LENGTH+10], buf[NOME_LENGTH+10];
long	t;
char *c;



   if (carrega_arquivo_tcase(&HTCase, DirCorrente, ArqTeste) == ERRO)
	exit(1);

   for (i = Menost, cont = 0; i <= Menosf; i++)
   {
	buf[0] = buf1[0] = '\0';
	if (strlen(ArqImport) > 0)
	    sprintf(buf, "%s%d", ArqImport, i);
	if (strlen(ArqParam) > 0)
	    sprintf(buf1, "%s%d", ArqParam, i);

	if (! tem_arq(DirImport, buf, "") && ! tem_arq(DirImport, buf1, ""))
		continue;
	if (tem_arq(DirImport, buf1, ""))
	{
	   fp_ascii = abrearq(DirImport, buf1, "");
	   if (fp_ascii == NULL)
		break;
	   posifim(fp_ascii);
	   t = ftell(fp_ascii);
	   posiciona(fp_ascii, 0);
	   if (t > sizeof(Parametros))
	   {
		msg("Parameter too big");
	   	fclose(fp_ascii);
		continue;
	   }
           fgets(Parametros, (int) t, fp_ascii);
	   fclose(fp_ascii);
	   k = strlen(Parametros);
	   if (k > 0 && Parametros[k-1] == '\n')
		Parametros[k-1] = '\0';
	}
	else
	   strcpy(Parametros, "");


        if ( tem_arq(DirImport, buf, ""))
	   fp_ascii = abrearq(DirImport, buf, "", 0);
	else
	   fp_ascii = tmpfile();

	fp2 = criarq(DirCorrente, ArqTeste, SUFIXO_INPUT);
	if (fp_ascii == NULL || fp2 == NULL)
		break;

	posifim(fp_ascii);
        if (copy_file(fp_ascii, 0l, (int) ftell(fp_ascii), fp2) == ERRO)
                break;
        fecharq(fp_ascii);
	fecharq(fp2);

       if (exec_from_ascii(DirExec, ArqExec, ArqInstrum, Parametros,
                       DirCorrente, ArqTeste, &(TREG(&HTCase)), 
			20,  Menostrace, SHELL, Menosinter) == ERRO )
		break;
	TREG(&HTCase).interativo = Menosinter;

	inic_to_buf(TREG(&HTCase).param, sizeof(TREG(&HTCase).param));
	inic_from_buf(Parametros);
   	for (c = from_buf(); c != NULL; c = from_buf())
	{
    	  if (to_buf(c) == ERRO)
	    break;
   	}

        if (Menoslabel)
           sprintf(XLabel, "%s%d", Label, i);
        else
           XLabel[0] = '\0';

        if (strlen(XLabel) <= LABELSIZE)
           strcpy(TREG(&HTCase).label, XLabel);
        else
        {
           msg("Invalid label");
           TREG(&HTCase).label[0] = '\0';
        }


	if (insere_tcase(&HTCase,  Menostrace) == ERRO)
		cont--;
	cont++;
        if (Menosv)
        {       
           printf("%c", verchar);
           fflush(stdout);
        }

    }

   printf("\n%d test cases imported from ASCII files\n", cont);

fim:
   descarrega_arquivo_tcase(&HTCase);
}



/***************************************************************************
        Le parametros da opcao -z do programa

***************************************************************************/
ZapTcase(argc, argv)
int     argc;
char    *argv[];
{
int     i, n;

   n = argc - 3;
   strcpy(ArqTeste, argv[argc-1]);
   strcpy(DirCorrente, ".");
   for (i = 2; i < argc-2; i++)
   {
        if (strcmp(argv[i], "-D") == 0)
        {
                if (! tem_dir(argv[i+1]))
                {
                        msg("Invalid directory on parameter -D");
                        exit(1);
                }
                strcpy(DirCorrente, argv[i+1]);
                argv[i] = argv[i+1] = "";
                n -= 2;
        }
   }

   if (n != 0)
   {
        msg("Invalid parameter");
        exit(1);
   }

   if (carrega_arquivo_tcase(&HTCase, DirCorrente, ArqTeste) == ERRO)
        exit(1);

   if (delete_io(&HTCase) == ERRO)
   {
        msg("Error deleteting I/O registers");
   }

   descarrega_arquivo_tcase(&HTCase );

}


/***************************************************************************
	Le parametros da opcao -add do programa

***************************************************************************/
AddTcase(argc, argv)
int	argc;
char	*argv[];
{
int	i, MenosDD, MenosE, MenosEE, n;
char *c;

   n = argc - 3;
   strcpy(ArqTeste, argv[argc-1]);
   strcpy(DirCorrente, ".");
   MenosDD = MenosE = MenosEE = Menostrace = FALSE;
   Parametros[0] = '\0';
   MenosP = Menosp = FALSE;
   Menosinter = FALSE;
   for (i = 2; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-D") == 0)
	{
		if (! tem_dir(argv[i+1]))
		{
			msg("Invalid directory on parameter -D");
			exit(1);
       		}
		strcpy(DirCorrente, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
	else

	if (strcmp(argv[i], "-DD") == 0)
	{
		if (! tem_dir(argv[i+1]))
		{
			msg("Invalid directory on parameter -D");
			exit(1);
       		}
		MenosDD = TRUE;
		strcpy(DirExec, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
	else


	if (strcmp(argv[i], "-E") == 0)
	{
		MenosE = TRUE;
		strcpy(ArqExec, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n-=2;
	}
	else

	if (strcmp(argv[i], "-EE") == 0)
	{
		MenosEE = TRUE;
		strcpy(ArqInstrum, argv[i+1]);
		argv[i] = argv[i+1] = "";
		n-=2;
	}
	else

        if (strcmp(argv[i], "-label") == 0 )
        {
                Menoslabel = TRUE;
                strcpy(Label, argv[i+1]);
                argv[i] = argv[i+1] = "";
                n-=2;
        }
        else

	if (strcmp(argv[i], "-p") == 0)
	{
	   strcpy(Parametros, argv[i+1]);
	   argv[i] = argv[i+1] = "";
	   Menosp = TRUE;
	   n -= 2;
	}
   }

   for (i = 0; i < argc-1; i++)
   {
	if (strcmp(argv[i], "-P") == 0)
	{
	   if (Menosp)
	   {
		msg("Invalid options -p and -P");
		exit(1);
	   }
	   printf("\nParameters: ");
	   gets(Parametros);
	   argv[i] = "";
	   n--;
	}
	else

	if (strcmp(argv[i], "-trace") == 0)
	{
	   Menostrace = TRUE;
	   argv[i] = "";
	   n--;
	}
	else

	if (strcmp(argv[i], "-ninter") == 0)
	{
	   Menosinter = FALSE;
	   argv[i] = "";
	   n--;
	}
   }

   if (n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

   if ( ! MenosDD)
	strcpy(DirExec, DirCorrente);


   if (! MenosE)
	strcpy(ArqExec, ArqTeste);
   if (! MenosEE)
	strcpy(ArqInstrum, ArqTeste);

   if (carrega_arquivo_tcase(&HTCase, DirCorrente, ArqTeste) == ERRO)
        exit(1);

   if ( Menosinter)
   {
      if (tcase_ex_inter(DirCorrente, ArqTeste, DirExec,
				ArqExec, Parametros, SHELL) == ERRO)
      {
	 exit(1);
      }
   }
   else
   {
      if (tcase_ex_bat(DirCorrente, ArqTeste, DirExec,
				ArqExec, Parametros) == ERRO)
      {
	 exit(1);
      }
   }

   if (exec_from_ascii(DirExec, ArqExec, ArqInstrum, Parametros,
                       DirCorrente, ArqTeste, &(TREG(&HTCase)),
			20,  Menostrace, SHELL, Menosinter) == ERRO )
	exit(1);
    TREG(&HTCase).interativo = Menosinter;


   inic_to_buf(TREG(&HTCase).param, sizeof(TREG(&HTCase).param));
   inic_from_buf(Parametros);
   for (c = from_buf(); c != NULL; c = from_buf())
   {
       if (to_buf(c) == ERRO)
	    break;
    }
    strcpy(TREG(&HTCase).label, Label);
    if (insere_tcase(&HTCase,  Menostrace) == ERRO)
                exit(1);

   descarrega_arquivo_tcase(&HTCase );


}



msg(x)
char *x;
{
   fprintf(stderr, "tcase: %s\n", x);
}

