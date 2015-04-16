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


#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <lib/gerais.h>
#include "lib/tcase.h"

extern char *from_buf();

/**
 * Current test set.
 */
HAND_TCASE	testSet;

/**
 * Name of the test session. Files regarding this session will be named as
 * such (and suffixed with proper file extensions, depending upon the data
 * each file holds).
 */
char	ArqTeste[NOME_LENGTH+1] = "";

/**
 * Current working directory. This can be changed using the option -D at the
 * command line.
 **/
char 	DirCorrente[NOME_LENGTH + 1] = "";

/**
 * Directory for the executable file.
 */
char DirExec[NOME_LENGTH+1] = "";

/**
 * Name of the executable file.
 */
char ArqExec[NOME_LENGTH+1] = "";

/**
 * Name of the instrumented executable file.
 */
char ArqInstrum[NOME_LENGTH+1] = "";

/**
 * Directory from which files with test cases should be imported from.
 */
char DirImport[NOME_LENGTH+1] = "";

/**
 * Prefix for filenames with test cases to be imported. Every filename
 * has this prefix and a number (usually sequential, from the
 * initialTestCase to finalTestCase).
 */
char ArqImport[NOME_LENGTH+1] = "";

/**
 * Prefix for filenames with command line parameters to be used for each
 * test case. Every filename has this prefix and a number (usually sequential,
 * from the initialTestCase to finalTestCase).
 */
char ArqParam[NOME_LENGTH+1] = "";

/**
 * Command line parameters to be used with the software under testing when
 * running it with the test cases.
 */
char Parametros[PARAMSIZE] = "";

/**
 * Label to be used for each imported test cases.
 */
char Label[LABELSIZE] = "";

int Menosinter;

/**
 * Control the generation of trace information when running test cases.
 */
int enableTrace = 0;

/**
 * Index number for the first mutant to be executed against test set.
 */
int firstMutant = -1;

/**
 * Index number for the last mutant to be executed against test set.
 */
int lastMutant = -1;


/**
 * Index number for the first file from which test cases should be imported from.
 */
int firstTestCase = -1;

/**
 * Index number for the last file from which test cases should be imported from.
 */
int lastTestCase = -1;


/**
 * Char used to show for each test case processed.
 */
char verbosityChar = '\0';


/**
 * Set up work directory.
 *
 * @param directory Directory that should be used as working directory.
 * @return 1 if it worked correctly, 0 otherwise.
 */
int
setupWorkDir(char directory[])
{
	struct stat sb; 
	if (stat(directory, &sb) == 0 && S_ISDIR(sb.st_mode)) {
		strcpy(DirCorrente, directory);
		chdir(DirCorrente);
		return OK;
	}
	return ERRO;
}


/**
 * Set up name for current test session.
 */
int
setupTestSession(char sessionName[])
{
	strcpy(ArqTeste, sessionName);
}



/**
 * Read parameters required by option '-create' and create test set.
 */
int
CreateTcase(int argc, char *argv[])
{
	// Create file with empty test set
	if (cria_arquivo_tcase(&testSet, DirCorrente, ArqTeste) == ERRO || descarrega_arquivo_tcase(&testSet) == ERRO) {
		msg("Could not create file with test set");
		return ERRO;
	}

	return OK;
}



/**
 * Handle listing of test cases from a file:
 *  -l (Actually this one selects the mode where test ases will be listed).
 *  -e
 *  -d
 *  -i
 */
int
ListTcase(int argc, char *argv[])
{
int	Menosx;
static	int VetMenosx[MAX_TCASE];
int	Menosl, Menose, Menosi, Menosp, Menosd;
int	f, i,j, k, n, t;
char	*Lista;


   firstMutant = lastMutant = Menosp = FALSE;
   f = MAX_TCASE;
   t = 1;

  firstMutant = (firstMutant || lastMutant);



   Menosx = 0;
   for (i = 2; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-x") == 0)
	{
	   if (firstMutant == -1 && lastMutant == -1)
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
			if ( ! firstMutant != -1 || (j < lastMutant || j > firstMutant))
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

   if (Menosx > 0 && ! firstMutant)
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
	msg("Invalid or missing parameter");
//	exit(1);
   }

/*------------------------------- Carrega arquivo de teste -------------*/

   if (carrega_arquivo_tcase(&testSet, DirCorrente, ArqTeste) == ERRO)
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
   descarrega_arquivo_tcase(&testSet);
   
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
      k = ltofis_tcase(&testSet, i);
   if (k == ERRO)
	return;
   if (le_tcase_reg(&testSet, k) == ERRO)
	return;

   printf("\nTeste Case # %d %d", i, k);
   printf("\nLabel:\t%s", TREG(&testSet).label);
   printf("\n\t%s", TAB_FIS(&testSet)[k].desabili? "DISABLED": "ENABLED");
   if (TAB_FIS(&testSet)[k].error)
	printf("\n\tTest case caused an ERROR");
   printf("\n\tExec. Time (CPU): %ld", TREG(&testSet).exetempo); 
   printf("\n\tExec. Time (total): %ld", TREG(&testSet).totaltempo);
   printf("\n\tReturn Code: %d", TREG(&testSet).retcode);
   if (TAB_FIS(&testSet)[k].error)
	printf(" (Failure with test case)");
   printf("\n\tParameters: %s", (char *) build_param(TREG(&testSet).param));
   printf("\n\tInput Size: %d", TREG(&testSet).tamanho_ent);
   printf("\n\tOutput Size: %d", TREG(&testSet).tamanho_sai);
   printf("\n\tStderr Size: %d", TREG(&testSet).tamanho_err);

   printf("\n\tInput:\n");
   posiciona(TFILEIO(&testSet), TREG(&testSet).apont_ent);
   for (j = 0; j < TREG(&testSet).tamanho_ent; j++)
   {
	fscanf(TFILEIO(&testSet), "%c", &c);
	printf("%c", c);
   }

/*   copy_file(TFILEIO(&testSet), TREG(&testSet).apont_ent, 
				TREG(&testSet).tamanho_ent, stdout); */

   t = TREG(&testSet).tamanho_ent;
   printf("\n\tOutput:\n");
   copy_file(TFILEIO(&testSet), TREG(&testSet).apont_ent + t, 
				TREG(&testSet).tamanho_sai, stdout);
   t += TREG(&testSet).tamanho_sai;
   printf("\n\tStderr:\n");
   copy_file(TFILEIO(&testSet), TREG(&testSet).apont_ent + t, 
				TREG(&testSet).tamanho_err, stdout);
   printf("\nTravessed blocks:");
   tcase_get_log(&testSet, -1, -1);
   if ( testSet.log != NULL)
      for (n = 0; n < testSet.log->nfunction; n++)
      {
	 p = &(testSet.log->vetfunc[n]);
	 printf("\n\tFunction: %ld\n\tBlocks: ", p->function);
	 for (j = 1; j <= p->nnode; j++)
	 {
	   if (get_log_bit(p->nodes, j))
		printf("%d ", j);
	 }
      }

   printf("\n");

}



/**
 * Enable test case from current test set.
 */
int
EnableTCase(int i)
{
	int k;
	k = ltofis_tcase(&testSet, i);
	if (k == ERRO) {
		return ERRO;
	}
	enable_tcase(&testSet, k);
	return OK;
}


   
/**
 * Disable test case from current test set.
 */
int
DisableTCase(int i)
{
	int k;
	k = ltofis_tcase(&testSet, i);
	if (k == ERRO) {
		return ERRO;
	}
	disable_tcase(&testSet, k);
	return OK;
}

/**
 * Delete test case from current test set.
 */
int
DeleteTCase(int i)
{
	if (delete_tcase(&testSet, i, i) == ERRO) {
		msg("Error at deleting test case");
		return ERRO;
	}
	return OK;
}


/**
 * Import test cases from Proteum, Poke-Tool or ASCII file.
 */
int
ImportTcase(int argc, char * argv[])
{
	int i;

	for (i = 1; i < (argc - 1); i++) {
		if (strcmp(argv[i], "-proteum") == 0) {
			printf("\nImporting from Proteum");
			return ImportProteum(argc, argv);
		} else if (strcmp(argv[i], "-poke") == 0) {
			printf("\nImporting from Poke-Tool");
			return ImportPoke(argc, argv);
		} else if (strcmp(argv[i], "-ascii") == 0) {
			printf("\nImporting from ASCII");
			return ImportAscii();
		}
	}

	msg("Invalid import option");
	return ERRO;
}

/**
 * Import test cases from Proteum.
 */
int
ImportProteum(int argc, char *argv[])
{
	int i, k, cont = 0;
	int recordNumber;
	FILE *fp_ascii;
	char *c;
	HAND_TCASE importedTestSet;
	char XLabel[NOME_LENGTH+1];

	if (carrega_arquivo_tcase(&testSet, DirCorrente, ArqTeste) == ERRO) {
		return ERRO;
	}

	if (carrega_arquivo_tcase(&importedTestSet, DirImport, ArqImport) == ERRO) {
		descarrega_arquivo_tcase(&testSet);
		return ERRO;
	}

	lastTestCase = MINI(lastTestCase, NTCASE(&importedTestSet));
	for (i = firstTestCase, cont = 0; i <= lastTestCase; i++) {
		recordNumber = ltofis_tcase(&importedTestSet, i);
		if (recordNumber == ERRO) {
			fprintf(stderr, "Error finding test case %d in the test set", i);
			continue;
		}

		if (le_tcase_reg(&importedTestSet, recordNumber) == ERRO) {
			fprintf(stderr, "Error importing test case %d (physical record %d)", i, recordNumber);
			continue;
		}

		fp_ascii = criarq(DirCorrente, ArqTeste, SUFIXO_INPUT);
		if (fp_ascii == NULL) {
			fprintf(stderr, "Error creating temporary test case for imported test case %d", i);
			continue;
		}
		
		if (copy_file(TFILEIO(&importedTestSet), TREG(&testSet).apont_ent, TREG(&testSet).tamanho_ent, fp_ascii) == ERRO) {
			fprintf(stderr, "Error copying imported test case %i to temporary test case", i);
			continue;
		}
		fecharq(fp_ascii);

		if (exec_from_ascii(DirExec, ArqExec, ArqInstrum, TREG(&importedTestSet).param, DirCorrente, ArqTeste, &(TREG(&testSet)), 120,  enableTrace, NULL, TREG(&testSet).interativo) == ERRO) {
			continue;
		}
		TREG(&testSet).interativo = TREG(&importedTestSet).interativo;
		inic_to_buf(TREG(&testSet).param, sizeof(TREG(&testSet).param));
		inic_from_buf(TREG(&importedTestSet).param);
		for (c = from_buf(); c != NULL; c = from_buf()) {
			if (to_buf(c) == ERRO) {
				break;
			}
		}
   		
 		if (Label != NULL) {
			sprintf(XLabel, "%s%d", Label, i);
		} else {
	   		strcpy(XLabel, (char*) TREG(&importedTestSet).label);
		}

		if (strlen(XLabel) <= LABELSIZE) {
			strcpy(TREG(&testSet).label, XLabel);
		} else {
           		fprintf(stderr, "Invalid label for test case %d", i);
			TREG(&testSet).label[0] = '\0';
		}

		if (insere_tcase(&testSet, enableTrace) == ERRO) {
			return ERRO;
		} 

		if (verbosityChar != '\0') {	
			fprintf(stdout, "%c", verbosityChar);
			fflush(stdout);
		}
		cont++;
	}
	fprintf(stdout, "\n%d test cases imported from Proteum file", cont);

	descarrega_arquivo_tcase(&importedTestSet);
	descarrega_arquivo_tcase(&testSet);
}



ImportPoke()
{
	FILE	*fp_ascii, *fp_par;
	int	i, j, k, cont, TemPar;
	char 	*c;
	char XLabel[NOME_LENGTH+1];



   if (carrega_arquivo_tcase(&testSet, DirCorrente, ArqTeste) == ERRO)
	exit(1);

   for (i = firstMutant, cont = 0; i <= lastMutant; i++)
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
                        DirCorrente, ArqTeste, &(TREG(&testSet)), 
			120,  enableTrace, SHELL, Menosinter) == ERRO )
		   break;
	TREG(&testSet).interativo = Menosinter;

	inic_to_buf(TREG(&testSet).param, sizeof(TREG(&testSet).param));
	inic_from_buf(Parametros);
   	for (c = from_buf(); c != NULL; c = from_buf())
	{
    	  if (to_buf(c) == ERRO)
	    break;
   	}

        if (! isempty(Label)) {
		sprintf(XLabel, "%s%d", Label, i);
	} else {
		ArqImport[0] = '\0';
	}

	if (strlen(XLabel) <= LABELSIZE)
           strcpy(TREG(&testSet).label, XLabel);
	else
	{
	   msg("Invalid label");
	   TREG(&testSet).label[0] = '\0';
 	}


       if (insere_tcase(&testSet,  enableTrace) == ERRO)
                break;

	cont++;
	if (verbosityChar != '\0' && ! isspace(verbosityChar)) {
           printf("%c", verbosityChar);
           fflush(stdout);
        }

    }

   printf("\n%d test cases imported from POKE-TOOL files\n", cont);

fim:
   descarrega_arquivo_tcase(&testSet);
}



ImportAscii()
{
FILE	*fp_ascii, *fp2;
int	i, j, k, cont;
static char	buf1[NOME_LENGTH+10], buf[NOME_LENGTH+10];
long	t;
char *c;

	char XLabel[NOME_LENGTH+1];

	if (carrega_arquivo_tcase(&testSet, DirCorrente, ArqTeste) == ERRO) {
		msg("Could not load test set");
		exit(1);
	}

	for (i = firstMutant, cont = 0; i <= lastMutant; i++) {
		buf[0] = buf1[0] = '\0';
		if (strlen(ArqImport) > 0) {
			sprintf(buf, "%s%d", ArqImport, i);
			printf("\nReading input data from file %s", buf);
		}
		if (strlen(ArqParam) > 0) {
			sprintf(buf1, "%s%d", ArqParam, i);
			printf("\nReading command line parameters from file %s", buf1);
		}

		if (! tem_arq(DirImport, buf, "") && ! tem_arq(DirImport, buf1, "")) {
			printf("\nCould not find the required files, skipping...");
			continue;
		}
		if (tem_arq(DirImport, buf1, "")) {
		 	fp_ascii = abrearq(DirImport, buf1, "", 0);
			if (fp_ascii == NULL) {
				printf("\nError reading command line parameters");
				break;
			}
			posifim(fp_ascii);
			t = ftell(fp_ascii);
			posiciona(fp_ascii, 0);
			if (t > sizeof(Parametros)) {
				msg("Parameter too big");
			   	fclose(fp_ascii);
				continue;
			}
			fgets(Parametros, (int) t, fp_ascii);
			fclose(fp_ascii);
			k = strlen(Parametros);
			if (k > 0 && Parametros[k-1] == '\n') {
				Parametros[k-1] = '\0';
			}
		} else {
			strcpy(Parametros, "");
		}


		if ( tem_arq(DirImport, buf, "")) {
			fp_ascii = abrearq(DirImport, buf, "", 0);
		} else {
			fp_ascii = tmpfile();
		}
		fp2 = criarq(DirCorrente, ArqTeste, SUFIXO_INPUT);
		if (fp_ascii == NULL || fp2 == NULL) {
			printf("\nError reading input data");
			break;
		}
		posifim(fp_ascii);
	        if (copy_file(fp_ascii, 0l, (int) ftell(fp_ascii), fp2) == ERRO)
	       	        break;
        	fecharq(fp_ascii);
		fecharq(fp2);
		if (exec_from_ascii(DirExec, ArqExec, ArqInstrum, Parametros, DirCorrente, ArqTeste, &(TREG(&testSet)), 120,  enableTrace, SHELL, Menosinter) == ERRO) {
			printf("\nCould not run executable against input data and command line parameters");
			break;
		}
		TREG(&testSet).interativo = Menosinter;
		inic_to_buf(TREG(&testSet).param, sizeof(TREG(&testSet).param));
		inic_from_buf(Parametros);
   		for (c = from_buf(); c != NULL; c = from_buf())	{
			if (to_buf(c) == ERRO)
				break;
	   	}
        	if (! isempty(Label)) {
			sprintf(XLabel, "%s%d", Label, i);
		} else {
        		XLabel[0] = '\0';
		}

	        if (strlen(XLabel) <= LABELSIZE) {
        		strcpy(TREG(&testSet).label, XLabel);
		} else {
     			msg("Invalid label");
			TREG(&testSet).label[0] = '\0';
		}


		if (insere_tcase(&testSet,  enableTrace) == ERRO) {
			cont--;
		}
		cont++;
        	if (verbosityChar != '\0' && ! isspace(verbosityChar)) {       
			printf("%c", verbosityChar);
	       		fflush(stdout);
        	}
	}
	printf("\n%d test cases imported from ASCII files\n", cont);
fim:
	descarrega_arquivo_tcase(&testSet);
}



/**
 * Reset test set (parameter -z)
 */
int
ZapTcase(int argc, char *argv[])
{
	if (carrega_arquivo_tcase(&testSet, DirCorrente, ArqTeste) == ERRO) {
		return ERRO;
	}

	if (delete_io(&testSet) == ERRO) {
		msg("Error deleteting I/O registers");
		return ERRO;
	}
	descarrega_arquivo_tcase(&testSet);
}


/**
 * Read command line parameters and add test cases (-add option).
 */
int
AddTcase(int argc, char * argv[])
{
	int i, n, timeout = 20;
	char *c;

	Parametros[0] = '\0';
	for (i = 0; i < (argc - 2); i++) {
		if (strcmp(argv[i], "-timeout") == 0) {
			timeout = atoi(argv[i+1]);
			argv[i] = argv[i+1] = "";
		}
	}

	if (carrega_arquivo_tcase(&testSet, DirCorrente, ArqTeste) == ERRO) {
	        return ERRO;
	}

	if (Menosinter) {
		if (tcase_ex_inter(DirCorrente, ArqTeste, DirExec, ArqExec, Parametros, SHELL) == ERRO) {
			return ERRO;;
		}
	} else {
		if (tcase_ex_bat(DirCorrente, ArqTeste, DirExec, ArqExec, Parametros) == ERRO) {
			return ERRO;
		}
	}

 	if (exec_from_ascii(DirExec, ArqExec, ArqInstrum, Parametros, DirCorrente, ArqTeste, &(TREG(&testSet)), timeout,  enableTrace, SHELL, Menosinter) == ERRO ) {
		return ERRO;
	}
	TREG(&testSet).interativo = Menosinter;


	inic_to_buf(TREG(&testSet).param, sizeof(TREG(&testSet).param));
	inic_from_buf(Parametros);
	for (c = from_buf(); c != NULL; c = from_buf()) {
		if (to_buf(c) == ERRO) {
			break;
		}
	}
	strcpy(TREG(&testSet).label, Label);
	if (insere_tcase(&testSet,  enableTrace) == ERRO) {
                return ERRO;
	}

	descarrega_arquivo_tcase(&testSet);
}


/**
 * Print error message.
 */
void
msg(char *x)
{
	fprintf(stderr, "tcase: %s\n", x);
}


int
main(int argc, char *argv[])
{
	int err, i;

	if (argc < 3) {
		msg("Missing parameters");
		exit(1);
	}

	for (i = 0; i < (argc - 2); i++) {
		fprintf(stdout, "Processando parâmetro %d: %s", i, argv[i]);
		if (strcmp(argv[i], "-D") == 0) {
			if (setupWorkDir(argv[i + 1]) == ERRO) {
				msg("Invalid working directory on parameter -D");
				return ERRO;
        	    	}
			argv[i] = "";
			argv[i+1] = "";
			fprintf(stdout, "\nSet test session directory for %s", DirCorrente);
		} else if (strcmp(argv[i], "-DD") == 0) {
			if (! tem_dir(argv[i+1])) {
				msg("Invalid directory on parameter -DD");
				return ERRO;
       			}
			strcpy(DirImport, argv[i+1]);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet directory for importing test cases to %s", DirImport);
		} else if (strcmp(argv[i], "-DE") == 0) {
			if (! tem_dir(argv[i+1])) {
				msg("Invalid directory on parameter -DE");
				return ERRO;
       			}
			strcpy(DirExec, argv[i+1]); // FIXME: DirExec?
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet directory for executable file to %s", DirExec);
		} else if (strcmp(argv[i], "-I") == 0) {
			strcpy(ArqImport, argv[i+1]);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet file prefix for test cases to %s", ArqImport);
		} else if (strcmp(argv[i], "-p") == 0) {
        	        strcpy(ArqParam, argv[i+1]);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet file prefix for test cases to %s", ArqImport);
		} else if (strcmp(argv[i], "-E") == 0) {
			strcpy(ArqExec, argv[i+1]);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet executable file to %s", ArqExec);
		} else if (strcmp(argv[i], "-EE") == 0) {
			strcpy(ArqInstrum, argv[i+1]);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet executable for trace file to %s", ArqInstrum);
		} else if (strcmp(argv[i], "-label") == 0 ) {
        	        strcpy(Label, argv[i+1]);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet label (?) to %s", Label);
	        } else if (strcmp(argv[i], "-p") == 0) {
			strcpy(Parametros, argv[i+1]);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet command line parameters to %s", Parametros);
		} else if (strcmp(argv[i], "-f") == 0) {
			firstMutant = MAXI(atoi(argv[i+1]), 0);
			firstTestCase = firstMutant;
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet initial test case and mutant to %d", firstMutant);
		} else if (strcmp(argv[i], "-t") == 0) {
			lastMutant = MINI(atoi(argv[i+1]), MAX_TCASE);
			lastTestCase = lastMutant;
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet final test case and mutant to %d", lastMutant);
		} else if (strcmp(argv[i], "-v") == 0) {
	                verbosityChar = *argv[i+1];
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet progress symbol to %c", verbosityChar);
	        }
	}

	for (i = 2; i < argc-1; i++) {
		if (strcmp(argv[i], "-P") == 0) {
			if (Parametros == NULL) {
				msg("Invalid options -p and -P");
				exit(1);
			}
			printf("\nParameters: ");
			gets(Parametros);
			argv[i] = "";
		} else if (strcmp(argv[i], "-trace") == 0) {
			enableTrace = TRUE;
			argv[i] = "";
			printf("\nEnable tracing.");
		} else if (strcmp(argv[i], "-ninter") == 0) {
			Menosinter = FALSE;
			argv[i] = "";
			printf("\nDisable ninter (?)");
		} else if (strcmp(argv[i], "-inter") == 0) {
			Menosinter = TRUE;
			argv[i] = "";
			printf("\nEnable ninter (?)");
		}
	}

	if (setupTestSession(argv[argc - 1]) == ERRO) {
		msg("Invalid test session");
		return ERRO;
	}
	argv[argc - 1] = "";



	// Could not find a parameter configuring the working directory, so the use
	// the default one (current directory).
	if (isempty(DirCorrente)) {
		setupWorkDir(".");	
	}

	if (isempty(DirExec)) {
		strcpy(DirExec, DirCorrente);
		fprintf(stdout, "\nSet directory for executable file to %s", DirExec);
	}

	if (isempty(DirImport)) {
		strcpy(DirImport, DirCorrente);
		fprintf(stdout, "\nSet directory for importing test cases to %s", DirImport);
	}

	if (isempty(ArqImport)) {
		strcpy(ArqImport, ArqTeste);
		fprintf(stdout, "\nSet file prefix for test cases to %s", ArqImport);
	}

	if (isempty(ArqExec)) {
		strcpy(ArqExec, ArqTeste);
		fprintf(stdout, "\nSet executable file to %s", ArqExec);
	}

	if (isempty(ArqInstrum)) {
		strcpy(ArqInstrum, ArqTeste);
		fprintf(stdout, "\nSet executable for trace file to %s", ArqInstrum);
	}

	if (lastMutant == 0) {
		lastMutant = firstMutant + MAX_TCASE;
	} else {
		lastMutant = MINI(lastMutant, firstMutant+MAX_TCASE);
	}
	fprintf(stdout, "\nSet initial test case to %d", lastMutant);
	fprintf(stdout, "\nSet final test case to %d", firstMutant);



	if (strcmp(argv[1], "-create") == 0) {
		argv[1] = "";
		err = CreateTcase(argc, argv);
		if (err != 0) {
			msg("Error creating test set");
		}
		exit(2);
	} else if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "-i") == 0) {
		err = ListTcase(argc, argv);
		if (err != 0) {
			msg("Error listing test cases");
		}
		exit(3);
	} else if (strcmp(argv[1], "-proteum") == 0 || strcmp(argv[1], "-poke") == 0 || strcmp(argv[1], "-ascii") == 0 ) {
		err = ImportTcase(argc, argv);
		if (err != 0) {
			msg("Error importing test cases");
		}
		exit(4);
	} else if (strcmp(argv[1], "-add") == 0) {
		argv[1] = "";
		err = AddTcase(argc, argv);
		if (err != 0) {
			msg("Error adding test case to test set");
		}
		exit(5);
	} else if (strcmp(argv[1], "-z") == 0) {
	        argv[1] = "";
        	err = ZapTcase(argc, argv);
		if (err != 0) {
			msg("Error reseting test set");
		}
		exit(6);
	} else {
		msg("Missing parameter");
		exit(1);
	}

	return err;
}



