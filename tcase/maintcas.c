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
HAND_TCASE testSet;

/**
 * Name of the test session. Files regarding this session will be named as
 * such (and suffixed with proper file extensions, depending upon the data
 * each file holds).
 */
char testSessionName[NOME_LENGTH+1] = "";

/**
 * Current working directory. This can be changed using the option -D at the
 * command line.
 **/
char workingDir[NOME_LENGTH + 1] = "";

/**
 * Directory for the executable file.
 */
char executableDir[NOME_LENGTH+1] = "";

/**
 * Name of the executable file.
 */
char executableFilename[NOME_LENGTH+1] = "";

/**
 * Name of the instrumented executable file.
 */
char instrumentedExecutableFilename[NOME_LENGTH+1] = "";

/**
 * Directory from which files with test cases should be imported from.
 */
char testCaseImportDir[NOME_LENGTH+1] = "";

/**
 * Prefix for filenames with test cases to be imported. Every filename
 * has this prefix and a number (usually sequential, from the
 * initialTestCase to finalTestCase).
 */
char testCaseInputDataImportFilename[NOME_LENGTH+1] = "";

/**
 * Prefix for filenames with command line parameters to be used for each
 * test case. Every filename has this prefix and a number (usually sequential,
 * from the initialTestCase to finalTestCase).
 */
char testCaseParametersImportFilename[NOME_LENGTH+1] = "";

/**
 * Label to be used for each imported test cases.
 */
char Label[LABELSIZE] = "";

/**
 * Define if the program under testing is interactive, ie, it requires
 * user input while running.
 */
int isInteractive = 0;

/**
 * Control the generation of trace information when running test cases.
 */
int enableTrace = 0;

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
		strcpy(workingDir, directory);
		chdir(workingDir);
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
	strcpy(testSessionName, sessionName);
}



/**
 * Read parameters required by option '-create' and create test set.
 */
int
CreateTcase(int argc, char *argv[])
{
	// Create file with empty test set
	if (cria_arquivo_tcase(&testSet, workingDir, testSessionName) == ERRO || descarrega_arquivo_tcase(&testSet) == ERRO) {
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
	int useTCList = 0;
	int tcList[MAX_TCASE];
	int tcListSize = 0;
	int tcNumber;
	int showTestCases = FALSE;
	int enableTestCases = FALSE;
	int disableTestCases = FALSE;
	int deleteTestCases = FALSE;
	int i, k;
	char *Lista;

	for (i = 2; i < argc-2; i++) {
		if (strcmp(argv[i], "-x") == 0) {
			useTCList = 1;
			Lista = argv[i+1];
			do {
				while (isspace(*Lista)) {
					Lista++;
				}
				k = 0;
				if (*Lista != '\0') {
					while (isdigit(Lista[k])) {
						k++;
					}
					if (k > 0 && (isspace(Lista[k]) || Lista[k] == '\0')) {
						if (sscanf(Lista, "%d", &tcNumber) == 1) {
							if (tcNumber > 0 && tcNumber < MAX_TCASE) {
								if (tcListSize < MAX_TCASE) {
									tcList[tcListSize] = tcNumber;
									tcListSize++;
								} else {
									msg("Maximum list size exceded: %d", MAX_TCASE);
								}
							} else {
								fprintf(stderr, "Invalid test case number: %d", tcNumber);
							}
						}
					}
				}			
				Lista = &Lista[k];
			} while (*Lista != '\0');
			argv[i] = argv[i+1] = "";
		}
	}



	for (i = 2; i < argc-1; i++) {
		if (strcmp(argv[i], "-l") == 0) {
			showTestCases = TRUE;
			argv[i] = "";
		} else if (strcmp(argv[i], "-e") == 0) {
			enableTestCases = TRUE;
			argv[i] = "";
		} else if (strcmp(argv[i], "-i") == 0) {
			disableTestCases = TRUE;
			argv[i] = "";
		} else if (strcmp(argv[i], "-d") == 0) {
			deleteTestCases = TRUE;
			argv[i] = "";
		}
	}

	if (enableTestCases && disableTestCases) {
		fprintf(stderr, "Conflicting options regarding enabling/disabling test cases");
		return ERRO;
	}

	if (carrega_arquivo_tcase(&testSet, workingDir, testSessionName) == ERRO) {
		return ERRO;
	}

	for (i = firstTestCase; i <= lastTestCase; i++) {
		if (showTestCases) {
			ListaTCase(i, 0);
		} else if (enableTestCases) {
			EnableTCase(i);
		} else if (disableTestCases) {
			DisableTCase(i);
		}
	}

	if (useTCList) {
		for (i = 0; i < tcListSize; i++) {
			if (showTestCases) {
				ListaTCase(tcList[i], 0);
			} else if (enableTestCases) {
				EnableTCase(tcList[i]);
			} else if (disableTestCases) {
				DisableTCase(tcList[i]);
			}
		}
	}

	// Defer deletion of test cases
	//TODO: check if its better to erase from the last to the first (the original code was like this)
	for (i = firstTestCase; i <= lastTestCase; i++) {
		if (deleteTestCases) {
   			DeleteTCase(i);
		}
	}

	if (useTCList) {
		for (i = 0; i < tcListSize; i++) {
			if (deleteTestCases) {
   				DeleteTCase(tcList[i]);
			}
		}
	}

	descarrega_arquivo_tcase(&testSet);
	return OK;
}


/**
 * Shot data of a test case.
 *
 * @param i Number of test case
 * @param phis Physical record number of this test case. If you do not know
 * it, set it to zero (so that Proteum look for this data by itself).
 */
int
ListaTCase(int i, int phis)
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
	FILE *testCaseFile;
	char *c;
	HAND_TCASE importedTestSet;
	char XLabel[NOME_LENGTH+1];

	if (carrega_arquivo_tcase(&testSet, workingDir, testSessionName) == ERRO) {
		return ERRO;
	}

	if (carrega_arquivo_tcase(&importedTestSet, testCaseImportDir, testCaseInputDataImportFilename) == ERRO) {
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

		testCaseFile = criarq(workingDir, testSessionName, SUFIXO_INPUT);
		if (testCaseFile == NULL) {
			fprintf(stderr, "Error creating temporary test case for imported test case %d", i);
			continue;
		}
		
		if (copy_file(TFILEIO(&importedTestSet), TREG(&testSet).apont_ent, TREG(&testSet).tamanho_ent, testCaseFile) == ERRO) {
			fprintf(stderr, "Error copying imported test case %i to temporary test case", i);
			continue;
		}
		fecharq(testCaseFile);

		if (exec_from_ascii(executableDir, executableFilename, instrumentedExecutableFilename, TREG(&importedTestSet).param, workingDir, testSessionName, &(TREG(&testSet)), 120,  enableTrace, NULL, TREG(&testSet).interativo) == ERRO) {
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

	return OK;
}

/**
 * Import test cases from POKE-TOOL.
 */
ImportPoke(int argc, char *argv[])
{
	FILE	*testCaseFile, *parametersFile;
	int	i, j, k, cont, TemPar;
	char 	*c;
	char XLabel[NOME_LENGTH+1];
	char parameters[PARAMSIZE] = "";

	if (carrega_arquivo_tcase(&testSet, workingDir, testSessionName) == ERRO) {
		return ERRO;
	}

	for (i = firstTestCase, cont = 0; i <= lastTestCase; i++) {
		// Try to import command line parameters for test case number i
		printf(testCaseInputDataImportFilename, "input%d.tes", i);
		strcpy(parameters, "");
		TemPar = FALSE;
		if (tem_arq(testCaseImportDir, testCaseInputDataImportFilename, "")) {
			parametersFile = abrearq(testCaseImportDir, testCaseInputDataImportFilename, "", 0);
			if (parametersFile != NULL) {
				posifim(parametersFile);
				k = ftell(parametersFile);
				if (k >= sizeof(parameters)) {
					msg("Invalid POKE-TOOL parameter file");
					fecharq(parametersFile);
				} else {
					posiciona(parametersFile, 0);
					fgets(parameters, (int) k, parametersFile);
				        k = strlen(parameters);
				        if (k > 0 && parameters[k-1] == '\n') {
						parameters[k-1] = '\0';
					}
					fecharq(parametersFile);
					TemPar = TRUE;
				}
			}
		}

		// Try to import test case number i
		sprintf(testCaseInputDataImportFilename, "tec%d.tes", i);
		if (tem_arq(testCaseImportDir, testCaseInputDataImportFilename, "")) {
			testCaseFile = abrearq(testCaseImportDir, testCaseInputDataImportFilename, "", 0);
			if (testCaseFile == NULL) {
				msg("Invalid POKE-TOOL test case");
				break;
			}
			fecharq(testCaseFile);
		}

		if (exec_from_ascii(executableDir, executableFilename, instrumentedExecutableFilename, parameters, workingDir, testSessionName, &(TREG(&testSet)), 120,  enableTrace, SHELL, isInteractive) == ERRO) {
			break;
		}
		TREG(&testSet).interativo = isInteractive;
		inic_to_buf(TREG(&testSet).param, sizeof(TREG(&testSet).param));
		inic_from_buf(parameters);
	   	for (c = from_buf(); c != NULL; c = from_buf()) {
			if (to_buf(c) == ERRO) {
				break;
			}
		}
		if (! isempty(Label)) {
			sprintf(XLabel, "%s%d", Label, i);
		} else {
			testCaseInputDataImportFilename[0] = '\0';
		}

		if (strlen(XLabel) <= LABELSIZE) {
			strcpy(TREG(&testSet).label, XLabel);
		} else {
			msg("Invalid label");
			TREG(&testSet).label[0] = '\0';
		}

		if (insere_tcase(&testSet,  enableTrace) == ERRO) {
                	break;
		}

		cont++;
		if (verbosityChar != '\0' && ! isspace(verbosityChar)) {
			fprintf(stdout, "%c", verbosityChar);
			fflush(stdout);
		}
	}

	printf("\n%d test cases imported from POKE-TOOL files\n", cont);

	descarrega_arquivo_tcase(&testSet);
	return OK;
}

/**
 * Import test cases from plain text files.
 */
int
ImportAscii(int argc, char *argv[])
{
	FILE *testCaseFile, *parametersFile;
	int i, j, k, cont;
	char testCaseFilename[NOME_LENGTH+10];
	char parametersFilename[NOME_LENGTH+10];
	long t;
	char *c;
	char parameters[PARAMSIZE] = "";
	char XLabel[NOME_LENGTH+1];

	if (carrega_arquivo_tcase(&testSet, workingDir, testSessionName) == ERRO) {
		msg("Could not load test set");
		return ERRO;
	}

	if (strlen(testCaseInputDataImportFilename) > 0 || strlen(testCaseParametersImportFilename)) {
		msg("Invalid import filename");
	}

	for (i = firstTestCase, cont = 0; i <= lastTestCase; i++) {
		sprintf(parametersFilename, "%s%d", testCaseInputDataImportFilename, i);
		fprintf(stdout, "\nReading command line parameters from file %s", parametersFilename);
		strcpy(parameters, "");
		if (tem_arq(testCaseImportDir, parametersFilename, "")) {
		 	parametersFile = abrearq(testCaseImportDir, parametersFilename, "", 0);
			if (parametersFile == NULL) {
				fprintf(stdout, "\nError reading command line parameters");
			} else {
				posifim(parametersFile);
				t = ftell(parametersFile);
				posiciona(parametersFile, 0);
				if (t > sizeof(parameters)) {
					msg("Parameter too big");
				   	fclose(parametersFile);
				} else {
					fgets(parameters, (int) t, parametersFile);
					fclose(parametersFile);
					k = strlen(parameters);
					if (k > 0 && parameters[k-1] == '\n') {
						parameters[k-1] = '\0';
					}
				}
			}
		}

		sprintf(testCaseFilename, "%s%d", testCaseParametersImportFilename, i);
		printf("\nReading input data from file %s", testCaseFilename);
		if ( tem_arq(testCaseImportDir, testCaseFilename, "")) {
			testCaseFile = abrearq(testCaseImportDir, testCaseFilename, "", 0);
			if (testCaseFile == NULL) {
				fprintf(stdout, "\nError reading input data");
			}
			fecharq(testCaseFile);
			if (exec_from_ascii(executableDir, executableFilename, instrumentedExecutableFilename, parameters, workingDir, testSessionName, &(TREG(&testSet)), 120,  enableTrace, SHELL, isInteractive) == ERRO) {
				fprintf(stdout, "\nCould not run executable against input data and command line parameters");
			} else {
				TREG(&testSet).interativo = isInteractive;
				inic_to_buf(TREG(&testSet).param, sizeof(TREG(&testSet).param));
				inic_from_buf(parameters);
		   		for (c = from_buf(); c != NULL; c = from_buf())	{
					if (to_buf(c) == ERRO) {
						break;
					}
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
					fprintf(stderr, "\nCould not import the test case %d", i);
				} else {
					cont++;
			        	if (verbosityChar != '\0' && ! isspace(verbosityChar)) {       
						fprintf(stdout, "%c", verbosityChar);
				       		fflush(stdout);
					}
				}
			}
		}
	}

	fprintf(stdout, "\n%d test cases imported from ASCII files\n", cont);
	descarrega_arquivo_tcase(&testSet);

	return OK;
}



/**
 * Reset test set (parameter -z)
 */
int
ZapTcase(int argc, char *argv[])
{
	if (carrega_arquivo_tcase(&testSet, workingDir, testSessionName) == ERRO) {
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
	char parameters[PARAMSIZE] = "";

	parameters[0] = '\0';
	for (i = 0; i < (argc - 2); i++) {
		if (strcmp(argv[i], "-timeout") == 0) {
			timeout = atoi(argv[i+1]);
			argv[i] = argv[i+1] = "";
		}
	}

	if (carrega_arquivo_tcase(&testSet, workingDir, testSessionName) == ERRO) {
	        return ERRO;
	}

	if (isInteractive) {
		if (tcase_ex_inter(workingDir, testSessionName, executableDir, executableFilename, parameters, SHELL) == ERRO) {
			return ERRO;;
		}
	} else {
		if (tcase_ex_bat(workingDir, testSessionName, executableDir, executableFilename, parameters) == ERRO) {
			return ERRO;
		}
	}

 	if (exec_from_ascii(executableDir, executableFilename, instrumentedExecutableFilename, parameters, workingDir, testSessionName, &(TREG(&testSet)), timeout,  enableTrace, SHELL, isInteractive) == ERRO ) {
		return ERRO;
	}
	TREG(&testSet).interativo = isInteractive;


	inic_to_buf(TREG(&testSet).param, sizeof(TREG(&testSet).param));
	inic_from_buf(parameters);
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
			fprintf(stdout, "\nSet test session directory for %s", workingDir);
		} else if (strcmp(argv[i], "-DD") == 0) {
			if (! tem_dir(argv[i+1])) {
				msg("Invalid directory on parameter -DD");
				return ERRO;
       			}
			strcpy(testCaseImportDir, argv[i+1]);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet directory for importing test cases to %s", testCaseImportDir);
		} else if (strcmp(argv[i], "-DE") == 0) {
			if (! tem_dir(argv[i+1])) {
				msg("Invalid directory on parameter -DE");
				return ERRO;
       			}
			strcpy(executableDir, argv[i+1]); // FIXME: DirExec?
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet directory for executable file to %s", executableDir);
		} else if (strcmp(argv[i], "-I") == 0) {
			strcpy(testCaseInputDataImportFilename, argv[i+1]);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet file prefix for test cases to %s", testCaseInputDataImportFilename);
		} else if (strcmp(argv[i], "-p") == 0) {
        	        strcpy(testCaseParametersImportFilename, argv[i+1]);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet file prefix for test cases to %s", testCaseInputDataImportFilename);
		} else if (strcmp(argv[i], "-E") == 0) {
			strcpy(executableFilename, argv[i+1]);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet executable file to %s", executableFilename);
		} else if (strcmp(argv[i], "-EE") == 0) {
			strcpy(instrumentedExecutableFilename, argv[i+1]);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet executable for trace file to %s", instrumentedExecutableFilename);
		} else if (strcmp(argv[i], "-label") == 0 ) {
        	        strcpy(Label, argv[i+1]);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet label (?) to %s", Label);
		} else if (strcmp(argv[i], "-f") == 0) {
			firstTestCase = MAXI(atoi(argv[i+1]), 0);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet initial test case to %d", firstTestCase);
		} else if (strcmp(argv[i], "-t") == 0) {
			lastTestCase = MINI(atoi(argv[i+1]), MAX_TCASE);
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet final test case to %d", lastTestCase);
		} else if (strcmp(argv[i], "-v") == 0) {
	                verbosityChar = *argv[i+1];
			argv[i] = "";
			argv[i+1] = "";
			printf("\nSet progress symbol to %c", verbosityChar);
	        }
	}

	for (i = 2; i < argc-1; i++) {
		if (strcmp(argv[i], "-trace") == 0) {
			enableTrace = TRUE;
			argv[i] = "";
			printf("\nEnable tracing.");
		} else if (strcmp(argv[i], "-ninter") == 0) {
			isInteractive = FALSE;
			argv[i] = "";
			printf("\nDisable ninter (?)");
		} else if (strcmp(argv[i], "-inter") == 0) {
			isInteractive = TRUE;
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
	if (isempty(workingDir)) {
		setupWorkDir(".");	
	}

	if (isempty(executableDir)) {
		strcpy(executableDir, workingDir);
		fprintf(stdout, "\nSet directory for executable file to %s", executableDir);
	}

	if (isempty(testCaseImportDir)) {
		strcpy(testCaseImportDir, workingDir);
		fprintf(stdout, "\nSet directory for importing test cases to %s", testCaseImportDir);
	}

	if (isempty(testCaseInputDataImportFilename)) {
		strcpy(testCaseInputDataImportFilename, testSessionName);
		fprintf(stdout, "\nSet file prefix for test cases to %s", testCaseInputDataImportFilename);
	}

	if (isempty(executableFilename)) {
		strcpy(executableFilename, testSessionName);
		fprintf(stdout, "\nSet executable file to %s", executableFilename);
	}

	if (isempty(instrumentedExecutableFilename)) {
		strcpy(instrumentedExecutableFilename, testSessionName);
		fprintf(stdout, "\nSet executable for trace file to %s", instrumentedExecutableFilename);
	}

	if (lastTestCase == 0) {
		lastTestCase = firstTestCase + MAX_TCASE;
	} else {
		lastTestCase = MINI(lastTestCase, firstTestCase + MAX_TCASE);
	}
	fprintf(stdout, "\nSet initial test case to %d", firstTestCase);
	fprintf(stdout, "\nSet final test case to %d", lastTestCase);



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



