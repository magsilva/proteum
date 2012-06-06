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


/**************************************************************************
				E X E M U T A . C 
Este modulo cuida da execucao de cada mutante


****************************************************************************/
/* #include	<stropts.h> */
#include 	<sys/types.h>
#include	<sys/time.h>
#include	<sys/times.h>
#include	<sys/wait.h>
#include	<sys/ioctl.h>

#ifndef		LINUX
#include	<sys/filio.h>
#endif

#include 	<signal.h>
#include	<fcntl.h>
#include 	<sys/stat.h>
#include	<unistd.h>

#include	<lib/gerais.h>
#include	<li/lib/li.h>
#include	<muta/lib/muta.h>
#include	<tcase/lib/tcase.h>
#include	<pteste/lib/pteste.h>

#define		TAMANHO_BUF	512 /* if this change, change playinput.c */

static	char	bufaux[1024],
		buf_sai[TAMANHO_BUF+1];

extern OPERADOR_MUTACAO g_tab_operador[];


/*------------------------------------------------------------------------
Variaveis que guardam os dados ligados as arquivo de casos de teste */

extern  HAND_TCASE	HTCase;   


/*------------------------------------------------------------------------
Variaveis que guardam os dados ligados ao arquivo de mutantes e tcase*/

extern	HAND_MUTANTE	HMuta;	/* manipulador do arquivo de mutantes */



/* -------------------------------------------------------------------
variaveis do arquivo de teste (.PTM) */

extern	CAB_PTESTE	pteste_cab;


extern	char	DirHome[];	/* diretorio onde estah "proteum.h" */



static	int	status, status1, status2;

static	int	pipe_io[3][2];

extern	int	flg_interrup;

char	*monta_nome();


executa_mutante(g_timeout, dequal, atequal, max, verbose, uselog, dual)
int	g_timeout, dequal, atequal, uselog, max, dual;
char	*verbose;
{
char	*p, *q, *r, *s,
	*monta_exec(),
	*monta_n_mutante();
int	i, j, k, t, z1, cont, grancont, to_exec, vet_exec[100];
DWORD	last_tcase;

   if (GERADOS(&HMuta) == 0)
   {
	msg("No Mutants to Execute");
	return;
   }

   if (carrega_descr(&HMuta, dequal) == ERRO)
	return;
    if (tty_save(STDIN_FILENO) == ERRO)
	return;

/*/   if ( aloca_master() == ERRO)
//	return; /* alocate a master pty for use in all executions */

   i = dequal;
   grancont = 0;
   max = MINI(LEN(vet_exec), max);
   while (i < atequal && ! flg_interrup)
   {


/* ----------- Escolhe quais mutantes precisam ser executados -------------*/
	cont = select_muta_to_exec(&i, atequal, vet_exec, max, uselog);

	if (cont == ERRO)
	   goto fim;

	if (cont == 0 || flg_interrup)
	   continue;

	if (verbose)
	   printf("\nGroup of %d (of %d) ", cont, grancont += cont);

	p = q = NULL;
	p = monta_n_mutante(&HMuta, &pteste_cab, cont, vet_exec);
	if (p == NULL)
	    goto fim;

	r = monta_exec(p, &pteste_cab);
	// printf("\n%s", r);

	for (z1 = 0; z1 < cont && ! flg_interrup ; z1++)
	{
           int number_of_tcase = 0;
	   if (carrega_descr(&HMuta, vet_exec[z1]) == ERRO)
		goto fim;

	   status = VIVO;
	   q = r;

/*--------------- Executa o mutante com cada caso de teste---------------*/

	   for (j = 1; j <= NTCASE(&HTCase) &&
	       (status == VIVO || (status == MORTO &&
				     pteste_cab.tipo == T_RESEARCH)); j++)
	   {
		k = ltofis_tcase(&HTCase, j);

/* Se caso de teste estiver desabilitado ou ja foi executado, nao executa */

		if (TAB_FIS(&HTCase)[k].desabili
		    || get_bitmap(REG(&HMuta).tcase, k) != NO_EXEC)
		           continue;

		if (q == NULL)
	   	{
		   if ((p = monta_n_mutante(&HMuta, &pteste_cab, 1,
						&vet_exec[z1])) == NULL )
	   		         goto fim;
		    q = monta_exec(p, &pteste_cab);
		    delearq(pteste_cab.dir, p, SUFIXO_FONTE);

		}


		if (le_tcase_reg(&HTCase, k) == ERRO)
		        goto fim;

		if (q == NULL)	/* nao conseguiu compilar */
		   retorno_exec_muta(NO_EXEC_COMP, vet_exec[z1], k, dual);
		else
		if (TAB_FIS(&HTCase)[k].error)
		   retorno_exec_muta(EXEC_MORTO_RETCOD, vet_exec[z1], k, dual);
		else
		{
		   s = monta_nome(pteste_cab.dir,q, "");
		   t = exec_1tcase_muta(s,&(TREG(&HTCase)),
					vet_exec[z1], g_timeout);
		   retorno_exec_muta(t, vet_exec[z1], k, dual);
		   number_of_tcase++;
		}
	   }
	   if (r == NULL && q != NULL)
		delearq(pteste_cab.dir, q, "");

	   if (verbose)
	   {
		 // printf("%d ", number_of_tcase);
		printf("%s",  verbose);
		fflush(stdout);
	   }

	   set_mutante_status(&HMuta, status, status1, status2);
	   if (rewrite_mutante(&HMuta) == ERRO)
		break;
	}
	if ( q != NULL)
	   delearq(pteste_cab.dir, q, "");
	if (p != NULL)
	   delearq(pteste_cab.dir, p, SUFIXO_FONTE);
    }
   fim:
	libera_master();
}



/**************************************************************************
MONTA_EXEC:
Cria arquivo executavel, compilando o fonte.
Parametros:
	i: numero do mutante
***************************************************************************/
char	*monta_exec(nome, pteste_cab)
char	*nome;
CAB_PTESTE	*pteste_cab;
{
char	*p, c;
static	char	buf[3*NOME_LENGTH+1];
struct stat	sstat;

   strcpy(buf,  monta_nome(pteste_cab->dir, nome, SUFIXO_EXEC));
   sub_str(bufaux, pteste_cab->comp, pteste_cab->exec, buf);

   delearq(pteste_cab->dir, nome, SUFIXO_EXEC);

   strcpy(buf, monta_nome("", pteste_cab->fonte, SUFIXO_FONTE));
   p = monta_nome(pteste_cab->dir, nome, SUFIXO_FONTE);
   sub_str(bufaux, bufaux, buf, p);

   while (get_str_exec(bufaux, &c, 0) == ERRO );

   if ( !tem_arq(pteste_cab->dir, nome,SUFIXO_EXEC))
	     {
/*		msg("Fail to Create Executable Mutant"); */
		return (NULL);
	      }

   if (stat(monta_nome(pteste_cab->dir, nome, SUFIXO_EXEC), &sstat) < 0)
	return NULL;

   if (! (S_ISREG(sstat.st_mode) && (sstat.st_mode & S_IXUSR)))
	return NULL;

   strcpy(buf, nome);
   strcat(buf, SUFIXO_EXEC);
   return buf;
}

/**********************************************************************
 FUNCTIONS FOR READING AND COMPARING MUTANT OUTPUT */

int	terr, tsai, tinput, differs;
FILE	*fp_io0, *fp_io1, *fp_io2;

int le_in(char *buff, int n)
{
int nread;

    n = MINI(n, tinput);
    if ( n == 0)
	return 0;

    nread = fread( buff, 1, n, fp_io0);
    if (nread <= 0)
    {
	if ( ferror(fp_io0))
        {
	    msg("Error reading I/O file");
	    return ERRO;
    	}
    	if (feof(fp_io0))
	    return 0;
    }
    tinput -= nread;
    return nread;
}

int compara_out(char *buff, int n, pid_t pid)
{
int nread;
int i;

   if (n == 0)
	return 0;
   if (tsai < n)
   {
	r1:
	differs = TRUE;
	kill(pid, SIGKILL);
	return 0;
   }

   nread = learq(fp_io1, buf_sai, n);
   if (nread != OK)
	   return ERRO;
   for (i = 0; i < n && buff[i] == buf_sai[i]; i++);
   if (i < n )
	goto r1;
   tsai -= n;
   return n;
}


int compara_err(char *buff, int n, pid_t pid)
{
int nread;
int i;

   if (n == 0)
	return 0;
   if (terr < n)
   {
	r1:
	differs = TRUE;
	kill(pid, SIGKILL);
	return 0;
   }

   nread = learq(fp_io2, buf_sai, n);
   if (nread != OK)
	return ERRO;
   for (i = 0; i < n && buff[i] == buf_sai[i]; i++);
   if (i < n )
	goto r1;
   terr -= n;
   return n;
}


/**************************************************************************
EXEC_1TCASE_MUTA:
	Inicia a execucao do mutante com 1 caso de teste
Parametros:
	nmuta: numero do mutante
	ntcase: numero do caso de teste
***************************************************************************/
exec_1tcase_muta(muta, reg, n, timeout)
char	*muta;
REG_TCASE	*reg;
int	n, timeout;
{
int	ret_cod, k;
static  char	buf[50], *argv[2048];
fd_set	fdset;

   sprintf(buf, "MUTANTTOEXEC=%d", n);
   if (putenv(buf) != 0)
   {
	perror("Erro no putenv");
	return ERRO;
   }

   argv[0] = monta_nome(pteste_cab.dir, pteste_cab.exec, "");
   separa_param(reg->param, &argv[1], NULL);

   fp_io2 = abrearq(pteste_cab.dir, pteste_cab.nome, SUFIXO_IO, 0);
   fp_io0 = abrearq(pteste_cab.dir, pteste_cab.nome, SUFIXO_IO, 0);
   if (fp_io2 == NULL)
   {
	k = ERRO;
	goto fim;
    }
    fp_io1 = TFILEIO(&HTCase);
    tinput = reg->tamanho_ent;
    tsai = reg->tamanho_sai;
    terr = reg->tamanho_err;
    differs = FALSE;

    posiciona(fp_io0, reg->apont_ent);
    posiciona(fp_io1, reg->apont_ent+reg->tamanho_ent);
    posiciona(fp_io2, reg->apont_ent+reg->tamanho_ent+tsai);

   if (reg->interativo)
       k = playinput(&ret_cod, fileno(fp_io0), le_in, compara_out,
		   compara_err, timeout*reg->exetempo,
		   timeout*reg->totaltempo, muta, argv);
   else
       k = playbatch(&ret_cod, fileno(fp_io0), le_in, compara_out,
		   compara_err, timeout*reg->exetempo,
		   timeout*reg->totaltempo, muta, argv);

   if (k < 0)
   {
        k = ERRO;
   	goto fim;
   }

   if (k == 1 ) /* timeout occurred */
   {
	k = EXEC_MORTO_TIMEOUT;
	goto fim;
   }

   k = EXEC_VIVO;

/*   if (WIFEXITED(ret_cod))
   {
    	if (! compara_retcode(ret_cod,reg->retcode))
	{
	    k = EXEC_MORTO_RETCOD;
	    goto fim;
	 }

    }
    else */
    if ( WIFSIGNALED(ret_cod) )
    {
	switch (WTERMSIG(ret_cod) )
	{
	   default:
	      /* printf("Signal: %d", WTERMSIG(ret_cod)); */
	   case SIGVTALRM:	/* timeout occurred */
	   case SIGALRM:
	      k = EXEC_MORTO_RETCOD;
	      goto fim;
	   case SIGKILL:	/* TRAP function executed */
	      k = EXEC_MORTO_TRAP;
	      goto fim;
	}
    }

   if (differs || tsai > 0 || terr > 0) /* output differs */
   {
	k = EXEC_MORTO_SAIDA;
	goto fim;
   }



fim:
   wait(NULL);

   fecharq(fp_io2);
   fecharq(fp_io0);

   return k;
}



/*************************************************************************
RETORNO_EXEC_MUTA:
	Esta funcoa e chamada quando termina a execucao de um mutante com 
album caso de teste.
Parametros:
	i: entrada da tabela onde esta informacao sobre qual caso de teste
	retorno: status da execucao;
	numero do Pid da execucao;
*************************************************************************/
retorno_exec_muta(ret, nmuta, ntcase, dual)
int	nmuta, ntcase, ret, dual;
{
int	t, tcase_status;


   status1 = ntcase;
   tcase_status = ret;
   switch(ret)
     {
	case ERRO:
	    tcase_status = NO_EXEC;
	case EXEC_VIVO:
	    break;
	case NO_EXEC_COMP:
	    status = ANOMALO;
    	    break;
	case EXEC_MORTO_TIMEOUT:
	case EXEC_MORTO_TRAP:
	case EXEC_MORTO_RETCOD:
	case EXEC_MORTO_SAIDA:
	    status = MORTO;
    	    break;
      }
   if (dual)
   {
        if (status == MORTO)
        {
            status = VIVO;
            tcase_status = EXEC_VIVO;
        }
        else
        if (status == VIVO)
        {
            status = MORTO;
            tcase_status = EXEC_MORTO_TRAP;
        }
   }
   set_bitmap(REG(&HMuta).tcase, ntcase, tcase_status);
   return OK;
}






/**************************************************************************
COMPARA_SAI:
	Compara saidaq produzida produzida pelo mutante com a saida
esperada.
Parametros:
	buf: saida produzida
	tam: tamanho do buffer
***************************************************************************/
compara_sai(fp, tamrest, buf, tam)
char	*buf;
int	tam, tamrest;
FILE *fp;
{
int	i, k;

   if (tam == 0)
        return 0;
/*
   k = tam;
   printf("\nTamanho saida armazenada: %d", tamrest);
   printf("\nTamanaho da saida obtida: %d", tam); */
   if (tamrest < tam)
 	return -1;

   k = tamrest;

   if (learq(fp, buf_sai, tam) < 0)
       return -1;

/*   buf_sai[k] = buf[tam] = 0;
   printf("\nSaida armazenada: %s", buf_sai);
   printf("\nSaida obtida: %s", buf); */

   for (i = 0; i < tam && buf[i] == buf_sai[i]; i++);
   return i < tam;
}



/*****************************************************************************
COMPARA_RETCODE:
	Verifica se codigo sao iguais
****************************************************************************/
compara_retcode(z, x)
int	x,z;
{
/* verifica apenas se os dois codigos representam terminacao normal */
/* Nao considera os valores propriamente dito do retorno */
/* se forem, returna TRUE. CC retorna FALSE */

  return (WIFEXITED(z));
}



/***************************************************************************
ATUALIZA_MUTANTES:
	This functtions goes thru mutants file and counts the number
of live, dead, anomalous, etc mutants.
**************************************************************************/
atualiza_mutantes(HMuta)
HAND_MUTANTE	*HMuta;
{
int	i;

   VIVOS(HMuta) =
   EQUIVS(HMuta) =
   INATIVOS(HMuta) =
   ANOMALOS(HMuta) = 0;

   for (i = 0; i < GERADOS(HMuta); i++)
   {
	if (carrega_descr(HMuta, i) == ERRO)
	   return ERRO;

 	if (get_mutante_inactive(HMuta))
	   (INATIVOS(HMuta))++;
	else
	   switch (get_mutante_status(HMuta))
	   {
		case EQUIV:
		   (EQUIVS(HMuta))++;
		   break;

                case ANOMALO:
                   (ANOMALOS(HMuta))++;
                   break;

                case VIVO:
                   (VIVOS(HMuta))++;
                   break;

	   }
   }
   if (grava_cab_indice(HMuta) == ERRO)
	return ERRO;

   return OK;
}


/*****************************************************************************
SELECT_MUTA_TO_COMPILE
	This function selects a group of mutants to be compiled
*****************************************************************************/
int select_muta_to_compile(int *dequal, int atequal, int v[], int max)
{
int cont = 0;
DWORD calling, called, last_tcase;
WORD  seq1, seq2;
int i, tx, j, k;

   if (carrega_descr(&HMuta, *dequal) == ERRO)
   	   return ERRO;
   calling = REG(&HMuta).calling;
   called = REG(&HMuta).called;
   seq1 = REG(&HMuta).seq1;

   start:
   while (*dequal < atequal && cont < max)
   {
	if (carrega_descr(&HMuta, *dequal) == ERRO)
	   return ERRO;

	if (REG(&HMuta).calling != calling || REG(&HMuta).seq1 != seq1)
	{
	   l1:
   	   calling = REG(&HMuta).calling;
   	   called = REG(&HMuta).called;
   	   seq1 = REG(&HMuta).seq1;
	   break;
	}
	if (seq1 >= 0)
	{
	   if (REG(&HMuta).called != called)
		goto l1;
	}

	calling = REG(&HMuta).calling;
   	called = REG(&HMuta).called;
   	seq1 = REG(&HMuta).seq1;

	 v[cont++] = *dequal;
	(*dequal)++;
   }
   if ( cont == 0 && *dequal < atequal)
	goto start;
   return cont;
}
   

compila_mutante(int dequal, int atequal, int max, char *verbose)
{
char	*p, *r,
	*monta_exec(), 
	*monta_n_mutante();
int	i, cont, grancont, vet_exec[100];

   if (GERADOS(&HMuta) == 0)
   {
	msg("No Mutants to Compile");
	return;
   }

   if (carrega_descr(&HMuta, dequal) == ERRO)
	return;
   i = dequal;
   grancont = 0;
   max = MINI(LEN(vet_exec), max);
   while (i < atequal && ! flg_interrup)
   {


/* ----------- Escolhe quais mutantes precisam ser compilados -------------*/
	cont = select_muta_to_compile(&i, atequal, vet_exec, max);

	if (cont == ERRO)
	   break;

	if (cont == 0)
	   continue;
		
	if (verbose)
	   printf("\nGroup of %d (of %d) ", cont, grancont += cont);

	p = r = NULL;
	p = monta_n_mutante(&HMuta, &pteste_cab, cont, vet_exec); 
	if (p != NULL)
	   r = monta_exec(p, &pteste_cab);
	if ( r != NULL)
	   delearq(pteste_cab.dir, r, "");
	if (p != NULL)
	   delearq(pteste_cab.dir, p, SUFIXO_FONTE);

   }
}


/*****************************************************************************
SELECT_MUTA_TO_EXEC
	This function selects a group of mutants to be executed
*****************************************************************************/
int select_muta_to_exec(int *dequal, int atequal, int v[], int max, int uselog)
{
int cont = 0;
DWORD calling, called, last_tcase;
WORD  seq1, seq2;
int i, tx, j, k;

   if (carrega_descr(&HMuta, *dequal) == ERRO)
   	   return ERRO;
   calling = REG(&HMuta).calling;
   called = REG(&HMuta).called;
   seq1 = REG(&HMuta).seq1;

   start:
   init_exec_log(&HTCase, &HMuta);
   while (*dequal < atequal && cont < max)
   {
	if (carrega_descr(&HMuta, *dequal) == ERRO)
	   return ERRO;

	i = get_mutante_status(&HMuta);
	if ( i == EQUIV | i == ANOMALO || get_mutante_inactive(&HMuta))
	{
	   (*dequal)++;
	   continue;
	}
	if (REG(&HMuta).calling != calling || REG(&HMuta).seq1 != seq1)
	{
	   l1:
   	   calling = REG(&HMuta).calling;
   	   called = REG(&HMuta).called;
   	   seq1 = REG(&HMuta).seq1;
	   break;
	}
	if (seq1 >= 0)
	{
	   if (REG(&HMuta).called != called)
		goto l1;
	}

	calling = REG(&HMuta).calling;
   	called = REG(&HMuta).called;
   	seq1 = REG(&HMuta).seq1;

	last_tcase = REG(&HMuta).ultimo_tcase;
	status = VIVO;
	status1 = status2 = 0;
	tx = 0;
	for (j = 1; j <= NTCASE(&HTCase) && 
		    (status == VIVO || pteste_cab.tipo == T_RESEARCH); j++)
	{
	   k = ltofis_tcase(&HTCase, j);
	   if (last_tcase <= TAB_FIS(&HTCase)[k].reg_log)
		set_bitmap(REG(&HMuta).tcase, k, NO_EXEC);
	   if (TAB_FIS(&HTCase)[k].desabili)
		continue;

	   switch (get_bitmap(REG(&HMuta).tcase, k))
	   {
		case EXEC_MORTO_SAIDA:
		case EXEC_MORTO_RETCOD:
		case EXEC_MORTO_TIMEOUT:
		case EXEC_MORTO_TRAP:
			status = MORTO;
			status1 = k;
			break;
		case NO_EXEC:
			if ( ! uselog || /*/ option -trace not used or
					 // operator does not suport trace */
			    ! g_tab_operador[REG(&HMuta).operador].use_trace || 
			    check_exec_log(&HTCase, &HMuta, k) )
			   tx++;
			else
			{
			   set_bitmap(REG(&HMuta).tcase, k, AVOIDED);
			}
			break;
	   }
	}
	set_mutante_status(&HMuta, status, status1, status2);

	if (tx > 0 && (status == VIVO || pteste_cab.tipo == T_RESEARCH) )
	   v[cont++] = *dequal;

	REG(&HMuta).ultimo_tcase = CONTLOG(&HTCase);	
	if (rewrite_mutante(&HMuta) == ERRO)
	   return ERRO;
	(*dequal)++;
   }
   clear_exec_log(&HTCase, &HMuta);
   if ( cont == 0 && *dequal < atequal)
	goto start;
   return cont;
}
   
