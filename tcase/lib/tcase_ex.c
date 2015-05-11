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


#include <sys/times.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <limits.h>
#include <lib/gerais.h>
#include <tcase/lib/tcase.h>
#include <dirent.h>

/**
 * Run test case and save data to file.
 *
 * @param dir Directory where data is to be saved to.
 * @param test Name of test case
 * @param direx Directory with executable
 * @param exec Name of executable file
 * @param param Parameters to be used with executable
 * @param shell Toggle to control whether to use PROTEUMIMHOME environment variable
 * to find Proteum's tools.
 */
int
tcase_ex_inter(char *dir, char *test, char *direx, char *exec, char *param, char *shel)
{
	int return_code;
	char exec_fullname[PATH_MAX];
	char test_fullname[PATH_MAX];
	char **argv;

	monta_nome(exec_fullname, direx, exec, "");
	argv = separa_param(param);
	monta_nome(test_fullname, dir, test, SUFIXO_INPUT);
	return_code = recinput(test_fullname, exec_fullname, argv);
	free_array_str(argv);
	if (return_code != 0) {
		return ERRO;
	}
	return OK;
}


tcase_ex_bat(char *dir, char *test, char *direx, char *exec, char *param)
{
	int t;
	int ret_cod;
	char *p;
	char y[_POSIX_ARG_MAX];

	sprintf(y, "%s %s \"%s\" %s", PROG_RECINPUT, monta_nome(direx, exec, ""), param, monta_nome(dir, test, SUFIXO_INPUT));
	if (exec_extern(y, &ret_cod, 1) == ERRO) {
		return ERRO;
	}
	if (ret_cod != 0) {
		return ERRO;
	}
	return OK;
}


FILE	*fpin, *fpout, *fperr;

int le_input(char *buff, int n)
{
int nread;

    nread = fread( buff, 1, n, fpin);
    if (nread <= 0)
    {
	if ( ferror(fpin))
        {
	    msg("Error reading input file");
	    return ERRO;
    	}
    	if (feof(fpin))
	    return 0;
    }
    return nread;
}

int trata_out(char *buff, int n, pid_t pid)
{
int nread;
   nread = gravarq(fpout, buff, n);
   if (nread != OK)
	return ERRO;
   return n;
}

int trata_err(char *buff, int n, pid_t pid)
{
int nread;
   nread = gravarq(fperr, buff, n);
   if (nread != OK)
	return ERRO;
   return n;
}


exec_from_ascii(direx, exec, instrum, param, dir, teste,
		reg, timeout, use_instrum, shel, interactive)
char	*exec, *instrum, *param, *direx, *dir, *teste, *shel;
REG_TCASE	*reg;
int	timeout, use_instrum, interactive;
{
long    t1, tt1, t2, tt2;
int     xret_cod, ret_cod, k;
struct  tms xtms;
struct	itimerval r1;
char *p;
	char exec_fullname[PATH_MAX];
	char inst_exec_fullname[PATH_MAX];
	char test_fullname[PATH_MAX];
	char **argv;

    if (tty_save(STDIN_FILENO) == ERRO)
	return ERRO;

    t1 = gettimechild();
    tt1 = times(&xtms);

    fpin = abrearq(dir, teste, SUFIXO_INPUT, 0);
    if (fpin == NULL)
	return ERRO;
    fpout = criarq(dir, teste, SUFIXO_OUTPUT);
    if (fpout == NULL)
    {
	fecharq(fpin);
	return ERRO;
    }
    fperr = criarq(dir, teste, SUFIXO_ERRO);
    if (fperr == NULL)
    {
	fecharq(fpin);
	fecharq(fpout);
	return ERRO;
    }
    monta_nome(exec_fullname, direx, exec, "");

	if (use_instrum) {
        	monta_nome(inst_exec_fullname, direx, instrum, "");
	} else {
		strcpy(inst_exec_fullname, argv[0]);
	}

	argv = separa_param(param);
	setbuf(fpin, NULL);

    if (interactive) {
/*	aloca_master(); */
        k = playinput(&ret_cod, fileno(fpin), le_input, trata_out, trata_err, timeout*100, timeout*100, p, argv);
/*	libera_master(); */
    } else {
	k = playbatch(&ret_cod, fileno(fpin), le_input, trata_out, trata_err, timeout*100, p, argv);
	}
	free_array_str(argv);

    fecharq(fpin);
    fecharq(fpout);
    fecharq(fperr);
    if ( k < 0)
	return ERRO;
    if (k == 1) /* timeout occurred */
    {
	msg("Timeout occurred. Check your input.");
	ret_cod = CODE_ERROR;
	return ERRO;
    }

   if (WIFEXITED(ret_cod))
   {
       ret_cod = WEXITSTATUS(ret_cod);
    }
    else
    {
	if ( WIFSIGNALED(ret_cod) && (WTERMSIG(ret_cod) == SIGVTALRM ||
		WTERMSIG(ret_cod) == SIGALRM) )
	   msg("Timeout occurred. Check your input.");
	else
	{
	   msg("Not a normal exit. Test case not included");
	   if (WIFSIGNALED(ret_cod) )
	      printf("Signal: %x %x", WTERMSIG(ret_cod), ret_cod);
	   else
		printf("Code: %x", ret_cod);
	}
	ret_cod = CODE_ERROR;
	return ERRO;
    }

    t2 = gettimechild();
    tt2 = times(&xtms);

    reg->exetempo = t2-t1+10;
    reg->totaltempo = tt2-tt1+10;
    reg->retcode = ret_cod;

    return OK;
}


