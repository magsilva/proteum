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
#include	<sys/resource.h>
#include	<sys/wait.h>
#include	<sys/ioctl.h>
#include	<sys/filio.h>
#include 	<signal.h>
#include	<fcntl.h>
#include 	<sys/stat.h>
#include	<unistd.h>

#include	<lib/gerais.h>
#include	<li/lib/li.h>
#include	<muta/lib/muta.h>
#include	<tcase/lib/tcase.h>
#include	<pteste/lib/pteste.h>

#define		TAMANHO_BUF	256

static	char	bufaux[1024],
		buf_sai[TAMANHO_BUF+1],
		buf_ent[TAMANHO_BUF+1];



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


executa_mutante(g_timeout, dequal, atequal, verbose)
int	g_timeout, dequal, atequal, verbose;
{
char	*p, *q, *r, *s,
	*monta_exec(), 
	*monta_n_mutante();
int	i, j, k, t, z1, cont, grancont, to_exec, vet_exec[100];
DWORD	calling, called;
DWORD	last_tcase;
WORD	seq1, seq2;

   if (GERADOS(&HMuta) == 0)
   {
	msg("No Mutants to Execute");
	return;
   }

   if (carrega_descr(&HMuta, dequal) == ERRO)
	return;

   calling = REG(&HMuta).calling;
   called = REG(&HMuta).called;
   seq1 = REG(&HMuta).seq1;  
    i = dequal;
   grancont = 0;
   while (i < atequal && ! flg_interrup)
   {


/* ----------- Escolhe quais mutantes precisam ser executados -------------*/
	cont = 0;
	do {
      	for (z1 = 0; 
	     cont < LEN(vet_exec) &&  i + z1 < atequal && !flg_interrup; z1++)
	{
           if (carrega_descr(&HMuta, i+z1) == ERRO)
                return;

	   if (get_mutante_inactive(&HMuta))
		continue;

	   j = get_mutante_status(&HMuta);
	   if (j == EQUIV || j == ANOMALO)
		continue;

	   if (REG(&HMuta).calling != calling ||  REG(&HMuta).seq1 != seq1) 
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
/*------------------ atualiza bitmap dos casos de teste -----------------*/
	   to_exec = 0;
           for (j = 1; j <= NTCASE(&HTCase) && 
		(status == VIVO || pteste_cab.tipo == T_RESEARCH); j++)
	   {
		k = ltofis_tcase(&HTCase, j);

		if (TAB_FIS(&HTCase)[k].desabili) 
		   continue;

		if (last_tcase <= TAB_FIS(&HTCase)[k].reg_log)
		        set_bitmap(REG(&HMuta).tcase, k, NO_EXEC);

		switch (get_bitmap(REG(&HMuta).tcase, k))
		{
		    case EXEC_MORTO_SAIDA:
		    case EXEC_MORTO_RETCOD:
		    case EXEC_MORTO_TIMEOUT:
		    case EXEC_MORTO_TRAP:
			status = MORTO;
			status1 = k;	/* numero do caso de teste */
			break;
		    case NO_EXEC:
			to_exec++;
			break;
		}
	    }
	    set_mutante_status(&HMuta, status, status1, status2);

	    if (to_exec != 0 && 
		(status == VIVO || pteste_cab.tipo == T_RESEARCH))
	    {
		vet_exec[cont++] = i + z1;
		grancont++;
	    }
	    else
	    {
		set_mutante_status(&HMuta, status, status1, status2);
	     }
	    if (rewrite_mutante(&HMuta) == ERRO)
		break;
	}
 	i += z1; 

	} while (cont == 0 && i < atequal && ! flg_interrup);

	p = q = NULL;
	if (cont != 0 && ! flg_interrup) 
	{
	   printf("\nGroup of %d (of %d)", cont, grancont);
	   p = monta_n_mutante(&HMuta, &pteste_cab, cont, vet_exec);
	   if (p == NULL)
	      return;

	   r = monta_exec(p, &pteste_cab);

	for (z1 = 0; z1 < cont && ! flg_interrup ; z1++)
	{
	   
	   if (carrega_descr(&HMuta, vet_exec[z1]) == ERRO)
		return;



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
	   		         return ;
		    q = monta_exec(p, &pteste_cab);
		    delearq(pteste_cab.dir, p, SUFIXO_FONTE); 
		   
		}


		if (le_tcase_reg(&HTCase, k) == ERRO)
		        return;

		if (q == NULL)	/* nao conseguiu compilar */
		   retorno_exec_muta(NO_EXEC_COMP, vet_exec[z1], k);
		else
		if (TAB_FIS(&HTCase)[k].error)
		   retorno_exec_muta(EXEC_MORTO_RETCOD, vet_exec[z1], k);
		else
		{
		   s = monta_nome(pteste_cab.dir,q, "");
		   t = exec_1tcase_muta(s,&(TREG(&HTCase)), 
					vet_exec[z1], g_timeout);
		   retorno_exec_muta(t, vet_exec[z1], k);
		}
	   }
	   if (r == NULL && q != NULL)
		delearq(pteste_cab.dir, q, "");

	   if (verbose)
	   {
		 printf("%c", (char) verbose); 
		fflush(stdout);
	   }

	   REG(&HMuta).ultimo_tcase = CONTLOG(&HTCase);
	   set_mutante_status(&HMuta, status, status1, status2);
	   if (rewrite_mutante(&HMuta) == ERRO)
		break;
	}
	if ( q != NULL)
	   delearq(pteste_cab.dir, q, "");
	if (p != NULL)
	   delearq(pteste_cab.dir, p, SUFIXO_FONTE);
       }
     }
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
int	i, pid, c, bytes, ret1, k, j;
static  char	buf[50], *argv[50];
struct  itimerval	t1;
long	tt2, tt1;
struct	tms xtms;
struct	rlimit	rlp;
long	st1, st2;
int	terr, tsai, tinput, fim_le;
FILE	*fp_io0, *fp_io1, *fp_io2;
struct timeval	sele_time;
fd_set	fdset;

   sprintf(buf, "MUTANTTOEXEC=%d", n);
   if (putenv(buf) != 0)
   {
	perror("Erro no putenv"); 
	return ERRO;
   }

   memset(&sele_time, 0, sizeof(sele_time));
   strcpy(bufaux, muta);

   argv[0] = monta_nome(pteste_cab.dir, pteste_cab.exec, "");
   separa_param(reg->param, &argv[1]);


   pipe(pipe_io[0]);
   pipe(pipe_io[1]);
   pipe(pipe_io[2]);
   fcntl(pipe_io[0][1], F_GETFL, &i);
   fcntl(pipe_io[0][1], F_SETFL, i | O_NDELAY | O_NONBLOCK);


 
   switch (pid = fork())
       {
	case -1:
	    k = ERRO;
	    goto fim;
	case 0:
            dup2(pipe_io[0][0], 0);
            dup2(pipe_io[1][1], 1);
	    dup2(pipe_io[2][1], 2);

	    for (i = maxfile(); i > 2; i--)
		close(i);

	    rlp.rlim_cur = rlp.rlim_max = 0;	    
	    setrlimit(RLIMIT_CORE, &rlp); /* tamanho maximo para arquivo
						CORE = 0 bytes */

		/* liga alarme para gerenciar timeout */
            tick2timeval(timeout*reg->exetempo, &t1.it_value);
	    t1.it_interval.tv_sec = t1.it_value.tv_sec ;
	    t1.it_interval.tv_usec = t1.it_value.tv_usec;
	    setitimer(ITIMER_PROF, &t1, NULL); 
	    signal(SIGPROF, SIG_DFL); 
	    execv(bufaux,argv ); 
	    return ERRO;
		
	default:

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

             posiciona(fp_io0, reg->apont_ent);
	     posiciona(fp_io1, reg->apont_ent+reg->tamanho_ent);
	     posiciona(fp_io2, reg->apont_ent+reg->tamanho_ent+tsai);

	     tt1 = times(&xtms);
	     st2 = timeout * reg->totaltempo;
	     fim_le = FALSE;

	     while ( waitpid(pid, &c, WNOHANG) != pid )
	     {
		  bytes = 0;

                  if (ioctl(pipe_io[1][0],FIONREAD,&bytes) != -1 && bytes > 0)
                           bytes = read(pipe_io[1][0],buf_ent,TAMANHO_BUF);
                     else  bytes = 0;


                   if (compara_sai(fp_io1, tsai, buf_ent, bytes))
                   {
                        kill(pid, SIGKILL);
                        k =  EXEC_MORTO_SAIDA;
                        goto fim;
                   }
                   tsai -= bytes;

                  bytes = 0;
                  if (ioctl(pipe_io[2][0],FIONREAD,&bytes) != -1 && bytes > 0)
                           bytes = read(pipe_io[2][0],buf_ent,TAMANHO_BUF);
                     else  bytes = 0;


                   if (compara_sai(fp_io2, terr, buf_ent, bytes))
                   {
                        kill(pid, SIGKILL);
                        k =  EXEC_MORTO_SAIDA;
                        goto fim;
                   }
                   terr-= bytes;

                  if ( ! fim_le)
                   {
                        if (bytes = MINI(TAMANHO_BUF, tinput))
                           bytes = fread(buf_ent, 1, bytes, fp_io0);

                        if (bytes <= 0)
                        {
                           fim_le = TRUE;
                           close(pipe_io[0][1]);
                        }
                        else
                        {
                        int t;
                           t = bytes;
                           bytes = write(pipe_io[0][1], buf_ent, bytes);
                           if (bytes > 0)
                           {
                              tinput -= bytes;
                              if (t != bytes)
                                  fseek(fp_io0, bytes-t, SEEK_CUR);
                            }
                            else
                               fseek(fp_io0, -t, SEEK_CUR);
                        }

                    }
                   /* sometimes, itimer doesn't work then this is a
                        guarantee for aborting looping mutants */
                   tt2 = times(&xtms);
                   st1 = tt2 - tt1;
                   if (st1 > st2)
                   {
                        kill(pid, SIGKILL);
                        k = EXEC_MORTO_TIMEOUT;
                        goto fim;
                   }
             }
             do {

                   if (ioctl(pipe_io[1][0],FIONREAD,&bytes) != -1 && bytes > 0)
                           bytes = read(pipe_io[1][0],buf_ent,TAMANHO_BUF);
                     else  bytes = 0;

                   if (compara_sai(fp_io1, tsai, buf_ent, bytes))
                             {
                                k =  EXEC_MORTO_SAIDA;
                                goto fim;
                             }
                   tsai -= bytes;
                 } while (bytes > 0);

             do {

                   if (ioctl(pipe_io[2][0],FIONREAD,&bytes) != -1 && bytes > 0)
                           bytes = read(pipe_io[2][0],buf_ent,TAMANHO_BUF);
                     else  bytes = 0;

                   if (compara_sai(fp_io2, terr, buf_ent, bytes))
                             {
                                k =  EXEC_MORTO_SAIDA;
                                goto fim;
                             }
                    terr -= bytes;
                 } while (bytes > 0);



	     ret1 = c;
	     if (WIFSIGNALED(ret1))
		switch (WTERMSIG(ret1))
		{
		   case SIGPROF:
			k = EXEC_MORTO_TIMEOUT;
			goto fim;
		   case SIGKILL:
			k = EXEC_MORTO_TRAP;
			goto fim;
		}		
 	}

   if (! compara_retcode(ret1,reg->retcode))
	     {
		k = EXEC_MORTO_RETCOD;
		goto fim;
	      }


   if (tsai > 0 || terr > 0)
   {
	k = EXEC_MORTO_SAIDA;
	goto fim;
   }     	

   k = EXEC_VIVO;

fim:
   wait(NULL);

   fecharq(fp_io2);
   fecharq(fp_io0);
   close(pipe_io[2][0]);
   close(pipe_io[2][1]);
   close(pipe_io[1][0]);
   close(pipe_io[1][1]);
   close(pipe_io[0][0]);
   close(pipe_io[0][1]);

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
retorno_exec_muta(ret, nmuta, ntcase)
int	nmuta, ntcase, ret;
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


