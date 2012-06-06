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


/*#########################################################################
			G E R A I S . C

	Rotinas de uso geral.

Autor: Delamaro
Data: 14/01/93
Atualizacao: 18/01/93

############################################################################*/



#include	"gerais.h" 
#include	<dirent.h>
#include	<time.h>
#include	<sys/wait.h>
#include        <sys/times.h>
#include        <limits.h>
#include	<sys/time.h>
#include	<string.h>
#include	<malloc.h>
#include	<errno.h>

#define		INCREMENT	128




/*---------------------- Utilizadas pela funcoes
			 INIC_TO_BUF e TO_BUF -----------------------*/
static int	cont_buf, max_buf;

static char	*ap_buf;


/*---------------------- Utilizadas pela funcoes
			 INIC_FROM_BUF e FROM_BUF -----------------------*/

static char	*ap_from_buf;


static	char	bufaux[1024];


extern errno;


/*************************************************************************
inic_to_buf/to_buf:
	Essas rotinas sao usadas para armazenar o conteudo de um menu
 em um buffer no formato ASCII.
************************************************************************/

inic_to_buf(buffer, max)
char	buffer[];
int	max;		/* tamanho maximo do buffer */
{
   max_buf = max;
   ap_buf = buffer;
   cont_buf = 0;
   buffer[0] = '\0';
}


to_buf(x)
char	x[];
{
int	i;
char	*s;

   s = x;
   i = strlen(s)+1;
   if (i + cont_buf >= max_buf)
	 return ERRO;
   strcpy(&ap_buf[cont_buf], s);
   cont_buf += i;
   ap_buf[cont_buf] = '\0';
   return OK;
}




/*************************************************************************
inic_from_buf/from_buf:
	Essas rotinas sao usadas para recuperar o conteudo de varias strings
 de um buffer.
************************************************************************/


inic_from_buf(buffer)
char	buffer[];
{
   ap_from_buf = buffer;
}



char	*from_buf()
{
char	*p;

   if (ap_from_buf[0] == '\0')
	 return NULL;
   p = ap_from_buf;
   ap_from_buf += strlen(p) + 1;
   return p;
}




/**************************************************************************
ITOA:
	Transforma inteiro em string.

**************************************************************************/
char	*itoa(x)
int	x;
{
static char buf[10];

   sprintf(buf, "%d", x);
   return buf;
}


char	*ltoa(x)
long	x;
{
static char buf[10];

   sprintf(buf, "%ld", x);
   return buf;
}



/**************************************************************************
TEM_DIR:
	Verifica se existe um diretorio.
**************************************************************************/
tem_dir(str)
char	*str;
{
DIR	*dp;

   dp = opendir(str);
   if (dp == NULL) return FALSE;
   closedir(dp);
   return TRUE;
}



/**************************************************************************
TEM_ARQ:
	Verifica se existe um arquivo.
**************************************************************************/
tem_arq(dir, arq, ext)
char	*dir, *arq, *ext;
{
DIR	*dp;
struct	dirent	*f;
char	buf[2*(NOME_LENGTH+1)], *p, *normaliza();

   p = normaliza(dir);
   dp = opendir(p);
   if (dp == NULL) return FALSE;
   strcpy(buf, normaliza(arq));
   strcat(buf, normaliza(ext));
   for (f = readdir(dp); f != NULL; f = readdir(dp))
     {
	if (strcmp(buf, f->d_name) == 0)
	       {
		   closedir(dp);
		   return TRUE;
		}
      }
   closedir(dp);
   return FALSE;
}



/*************************************************************************
NORMALIZA:
	Tira brancos do fim e do comeco de um string
*************************************************************************/
char	*normaliza(str)
char	*str;
{
static char	buf[NOME_LENGTH+1];
int	i = 0;

   while (*str <= ' ' && *str != '\0')
	str++;
   do {
	buf[i++] = *(str);
       } while (*str++ != '\0');
   i--;
   while (buf[i] <= ' ' && i >= 0)
        buf[i--] = '\0';
   return buf;
}



/*************************************************************************
ACHA_STR:
	Procura um string  dentro de outro string.
*************************************************************************/
acha_str(s1,s2)
char	*s1,*s2;
{

int	t1, i;

   t1 = strlen(s1);
   i = 0;
   while (strncmp(s1,s2, t1) && *s2 != '\0')
	{
	   s2++;
	   i++;
	}
   if (*s2 == '\0')
	 return -1;
   return i;
}

	 
acha_palavra(s1,s2)
char	*s1, *s2;
{
int	i, t, j;

   i = j = acha_str(s1, s2);
   t = strlen(s1);
   while ( j >= 0)
     {
	if (i == 0 && s2[i+t] <= ' ') 
	   return i;
	if (i > 0) 
	   if (s2[i-1] <= ' ' && s2[i+t] <= ' ')
		return i;
	j = acha_str(s1, &(s2[i+1]));
	i += j + 1;
      }
   return -1;
}



	   
/**************************************************************************
MONTA_NOME:
	Monta nome de um arquivo dados diretorio, nome e extensao.
**************************************************************************/

char	*monta_nome(dir, nome, ext)
char	*dir, *nome, *ext;
{
static	char	bufao[10][3*(NOME_LENGTH+1)];
static	int	next=0;
int	t;
char	*buf, *p;

   buf = bufao[next++];
   next %= 10;
   buf[0] = '\0';
   if (nome[0] == '\0')
	return buf; /* se nao tem nome, retorna vazio */
   strcpy(buf, normaliza(dir));
   t = strlen(buf);
   if (t > 0 && buf[t-1] != '/')
	 buf[t++] = '/'; 
   p = normaliza(nome);
   strcpy(&(buf[t]), p);
   strcat(buf, ext);
   return buf;
}


/***************************************************************************
GET_STR_EXEC:
	Esa funcao pega um string produzido pela execucao de um programa
de sistema.
Parametros:
	comando: string do comando a executar
	buf: buffer onde coloca o resultado.
	t: tamanho maximo do buffer.

***************************************************************************/

get_str_exec(comando, buf, t)
char	*comando,*buf;
int	t;
{
FILE	*fp;
int	i = 0;
char	c;

   fp = popen(comando, "r");
   buf[0] = '\0';
   if (fp == NULL) 
          {
		sprintf(bufaux, "Unable to Execute: %s", comando);
		msg(bufaux); 
		return ERRO;
	  }
   c = getc(fp);
   while (c != EOF)
     {
	if (i < t)
	  buf[i++] = c;
	c =getc(fp);
      }
   pclose(fp);
   return OK;
}


	   
/**************************************************************************
SUB_STR:
	Esta substitui um substring por outr.

Parametros:
	buf: buffer onde deve ser montado novo string.
	mod: string original.
	pro: substring a ser substituido
	sub: substring que substitui o primeiro
Obs: mod e buf podem ser o mesmo string.

***************************************************************************/
sub_str(buf, mod, pro, sub)
char	*buf, *mod, *pro, *sub;
{
int	i, t , j, k;

   strcpy(bufaux, mod);
   t = strlen(pro);
   buf[0] = '\0';
   for (j = i = 0, k = acha_palavra(pro, bufaux); k >= 0; k = acha_palavra(pro, &bufaux[i]))
     {
	i = i+k;
	bufaux[i] = '\0';
	strcat(buf, &bufaux[j]);
	strcat(buf, sub);
	i += t;
	j = i;
      }
   strcat(buf, &bufaux[j]);
}



/****************************************************************************
MONTA_DATA:
	monta data corrente no formato "Feb-09-93".
Autor: Delamaro
****************************************************************************/
char	*monta_data()
{
static char buf[11];
struct tm *localtime(), *l;
time_t	time(), t;

   t = time(NULL);
   l = localtime(&t);
   strftime(buf, 10, "%h-%d-%y",l);
   return buf;
}


/****************************************************************************
GRVA_IDENT:
	Grava header de identificacao.
Parametros:
	fp: file pointer do arquivo onde deve gravar
	str: string de identificacao
Autor: Delamaro
*****************************************************************************/
grava_ident(fp, str)
char	*str;
FILE	*fp;
{
int	i;
extern	char	*VERSAO;
char	*monta_data(),
	*p,
	buf[64];

   if (posiciona(fp, 0l) == ERRO)
	 return ERRO;

   if (fprintf(fp, "%s\n", str) == -1)
	 goto erro;

   if (fprintf(fp, "%s\n", p = monta_data()) == ERRO)
	 goto erro;

   if (fprintf(fp, "%s\n", VERSAO) == -1)
	 goto erro;

   for (i = 0; i < sizeof(buf); i++) buf[i] = ' ';
   i = OFFSET0 - 3 - strlen(str) - strlen(VERSAO) - strlen(p);
   buf[i-1] = '\n';
   if (gravarq(fp, buf, i) == ERRO)
	 return ERRO;
   return OK;

erro:
   d_msg(nomearq(fp), extarq(fp), "Error Writing File");
   return ERRO;
}


/**************************************************************************
CHECA_IDENT:
	Procura o string de identificacao no inicio do arquivo
Parametros:
	fp: file pointer do arquivo a checar
	str: string de identificacao a procurar
Autor: Delamaro
**************************************************************************/
checa_ident(fp, str)
char	*str;
FILE	*fp;
{
extern	char	*VERSAO;
static	char	buf[OFFSET0];
int	i;

   if (posiciona(fp, 0l) == ERRO) 
	 return ERRO;
   if (learq(fp, buf, OFFSET0) == ERRO)
	     {
		return ERRO;
	     }
   if (strncmp(buf, str, strlen(str)))
	     {
		d_msg(nomearq(fp), extarq(fp), "Invalid File");
		return ERRO;
	     }
   i = 0;

   while ( buf[i] != '\n' && i < OFFSET0) i++;
	i++;	/* here i points date of creation */

   while (i < OFFSET0 && buf[i] != '\n') i++;
	i++;	/* here i points version */

   if (strncmp(&buf[i], VERSAO, strlen(VERSAO)) )
   {
	d_msg(nomearq(fp), extarq(fp), "Invalid Version");
	return ERRO;
   }
   return OK;
}



/***************************************************************************
FHASH:
	Devolve unsigned long baseado num string e 2 inteiros.
Parametros:
	str: string a ser usado.
	k1, k2: inteiros para gerar long
Autor: Delamaro
***************************************************************************/
unsigned long fhash(str, k1, k2)
char	*str;
int	k1,k2;
{
int	l;
unsigned long s, t, *p;
union {
	char	str[100];
	unsigned long u;
      } al;

   strncpy(al.str, str, 99);
   al.str[100] = '\0';
   l = strlen(al.str) / sizeof (unsigned long);
   p = (unsigned long *) al.str;
   t = 0;
   s = 1;
   while (l > 0)
      {
	t = *p;
	t <<= 3;
	t >>= 6;
	t *= k1;
	t *= k2;
	s *= t;
        p++;
	l--;
       }
   s |= 1;
   return s;
}



/************************************************************************
SEPARA_PARAM:
	Pega um string e sepera em diversos parametros
************************************************************************/
separa_param(p, arg, shel)
char	*p, *arg[], *shel;
{
int     i,j;
FILE *fppipe;
char *q;

   if (p == NULL)
             {
                arg[0] = NULL;
                return 0;
              }
   q = p;
   if (shel != NULL)
   {
     sprintf(bufaux, "$PROTEUMIMHOME/%s %s", PROG_SEPARA, p);
     fppipe = popen(bufaux, "r");
     if (fppipe == NULL)
     {
        return ERRO;
     }
     while (fgets(p, 512, fppipe) != NULL)
     {
        p += strlen(p);
	*(p-1) = '\0';
     }
     *p = '\0';
     pclose(fppipe);
   }
   p = q;
   i = 0;
   j = 0;

   inic_from_buf(p);
   for (q = from_buf(); q != NULL; q = from_buf())
   {
       arg[i++] = q;
    }
   arg[i] = NULL;
   return i;
}



/***********************************************************************
TIMETOSEC:
	Transforma tempo, expresso pela estrutura:

struct timeval {
	long	tv_sec;		
	long	tv_usec;	
};

em um valor long, que representa alguma fracao do segundo
*************************************************************************/
long timetosec(x, n)
struct 	timeval	*x;
int	n;
{
long t, k;

   t = x->tv_sec * (long) n;
   k = 1000000L / (long) n;
   t += (x->tv_usec) / k;
   return t <= 0? 0: t;
}

/***********************************************************************
SECTOTIME:
	Transforma tempo, expresso numa fracao de segundos, na estrutura:

struct timeval {
	long	tv_sec;		
	long	tv_usec;
};

*************************************************************************/
long sectotime(l, x, n)
long	l;
struct 	timeval	*x;
int	n;
{
long t, k;

   
   x->tv_sec = l / (long) n;
   l %= (long) n;
   k = 1000000L / (long) n;
   x->tv_usec = l * k;
   return l;
}

/***********************************************************************
SUB_TIME:
	Subtracts 2 structures 'timeval'
***********************************************************************/
long sub_time(t2, t1, n)
struct timeval *t2, *t1;
int	n;
{
struct timeval t;

  if (t2->tv_usec < t1->tv_usec)
  {
	t.tv_usec = 1000000L - t1->tv_usec + t2->tv_usec;
	t2->tv_sec--;
  } 
  else
	t.tv_usec = t2->tv_usec - t1->tv_usec;

  t.tv_sec = t2->tv_sec - t1->tv_sec;

  return timetosec(&t, n);
}


/***********************************************************************
SET_XXX:
	Rotinas para manipular conjuntos, onde cada elemento e' um 
string.

Autor : Delamaro

**************************************************************************/

void set_new(s)	/* creates an empty set */
OSET	*s;
{
   s->n = 0;
   s->free = 0;
   s->p = NULL;
   s->sizeofp = 0;
}

int	set_new_inic(s, p)
OSET *s;
char *p;
{
char	*c;

   set_new(s);
   inic_from_buf(p);
   for (c = from_buf(); c != NULL; c = from_buf())
   {
      if (set_add(s, c) == ERRO)
	return ERRO;
   }
   return OK;
}


int	set_add(s, c)
char	*c;
OSET	*s;
{
char	*p;
int	k;

   k = strlen(c);
   if (k == 0)
	return OK;
   if (s->free+k >= s->sizeofp)
   {
	p = realloc(s->p, s->sizeofp+INCREMENT);
	if (p == NULL)
	   return ERRO;
	s->p = p;
	s->sizeofp += INCREMENT;
   }
   strcpy(&(s->p[s->free]), c);
   s->vet[s->n] = s->free;
   s->free += k + 1;
   s->n++;
   return OK;
}





char	*set_get(s, k)	/*devolve k-esimo elemento do conjunto */
OSET	*s;
int	k;
{

   return (k < s->n) ? &(s->p[s->vet[k]]) : NULL ;
}



int set_inn(s, c)
char	*c;
OSET	*s;
{
char	*p;
int	i;

   for (i = 0; i < s->n; i++)
   {
	p = set_get(s, i);
	if (p == NULL)
	   return  -1;
	if ( strcmp(p, c) == 0)
	   break;
   } 
   return (i < s->n) ? i : -1;
}


void set_del(s, k)
OSET *s;
int	k;
{
int i;
char *p;

   if (k >= s->n)
     return;	
   s->free = s->vet[k];

   for (i = k+1; i < s->n; i++)
   {
	p = set_get(s, i);
	strcpy(&(s->p[s->free]), p);
	s->vet[i-1] = s->free;
	s->free += strlen(&(s->p[s->free])) + 1;
   }
   s->n--;
}


void	set_free(OSET *p)
{
   if (p->p)
	free(p->p);
   set_new(p);
}


int	set_dup(OSET *p, OSET *q)
{
int i;
char	*c;

   set_new(q);
   for (c = set_get(p, i=0); c != NULL; c = set_get(p, ++i))
	if (set_add(q, c) == ERRO)
	   return ERRO;
   return OK;
}



/**********************************************************************
OrdenaDecr:
	Ordena um vetor de inteiros em ordem decrescente

Parametros:
	a:	vetor a ser ordenado
	n:	numero de elementos do vetor

***********************************************************************/
OrdenaDecr(a, n)
int	a[], n;
{
int	hold, j, pass;
int	switched;

    switched = TRUE;
    for (pass = 0; pass < n-1 && switched == TRUE; pass++)
    { /* loop externo controla o numero de passos */
	switched = FALSE; /* inicialmente, nenhuma troca fo
				i feita nesse passo*/
	for (j = 0; j < n - pass -1; j++)
	{ /* loop interno controla cada passo */
	    if (a[j] < a[j+1])
	    {
		switched = TRUE;
		hold = a[j];
		a[j] = a[j+1];
		a[j+1] = hold;
	    }
	}
    }
}




/***********************************************************************
GETTIMECHILD:
	Get CPU time used by child process.

Parameters:
struct	timeval	t:	struct to return time.

************************************************************************/
long gettimechild()
{
struct tms	ticks;
clock_t	i;

    i = times(&ticks);

    if (i < 0)
    {
	printf("times retornou -1. errno = %d", errno);
	return 0l;
    }

    return ticks.tms_cutime + ticks.tms_cstime;
}

long gettimedad()
{
struct tms      ticks;
clock_t i;
 
    i = times(&ticks);
 
    if (i < 0)
        return 0l;
 
    return ticks.tms_utime + ticks.tms_stime;
}



/***********************************************************************
TICK2TIMEVAL
	Transform ticks to struct timeval;

Parameters:
long tick:	Number of ticks
struct timeval*t:	Place to return corresponding timeval

**********************************************************************/
tick2timeval(tick, t)
long tick;
struct timeval *t;
{

   t->tv_sec = tick / CLOCKS_PER_SEC;
   t->tv_usec = (tick % CLOCKS_PER_SEC) * (1000000L / CLOCKS_PER_SEC );
} 

/*************************************************************************
WRITEN:
     Write n bytes to a fd
*************************************************************************/
ssize_t	writen(int fd, const void *vptr, size_t n)
{
	int		nleft, nwritten;
	const char	*ptr;

	ptr = vptr;	/* can't do pointer arithmetic on void* */
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0)
			return(nwritten);		/* error */

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n);
}


/*************************************************************************
COPY_FILE:
	This routine copies a piece of a file to another file
Parameters:
FILE	*fp1:	Source FILE
long	inic:	Initial offset from where to copy;
int	size:	Size of the piece to copy
FILE	*fp1:	Destiny file

**************************************************************************/
copy_file(fp1, inic, size, fp2)
int	size;
long	inic;
FILE	*fp1, *fp2;
{
int	auxsize;
char	*p;

   if (size <= 0)
	return OK;
   auxsize = size;
   do {
	p = (char *) malloc(auxsize);
	if (p != NULL)
	   break;
	auxsize /= 2;
   } while (auxsize > 1024);

   if (p == NULL)
   {
	msg("Insuficient Memory");
	return ERRO;
   }

   if (posiciona(fp1, inic) == ERRO)
   {
	erro:
	free(p);
	return ERRO;
   }

   while (size > 0)
   {
        if (learq(fp1, p, MINI(size, auxsize)) == ERRO)
           goto erro;
        if (gravarq(fp2, p, MINI(size, auxsize)) == ERRO)
           goto erro;
        size -= MINI(size, auxsize);
   }
   free(p);
   return OK;
}


copy_file_to_end(fp1, inic, fp2)
long	inic;
FILE	*fp1, *fp2;
{
int	auxsize, size;
char	*p;

   auxsize = 64 * 1024;
   do {
	p = (char *) malloc(auxsize);
	if (p != NULL)
	   break;
	auxsize /= 2;
   } while (auxsize > 1024);

   if (p == NULL)
   {
	msg("Insuficient Memory");
	return ERRO;
   }

   if (posiciona(fp1, inic) == ERRO)
   {
	erro:
	free(p);
	return ERRO;
   }

   do
   {
           if ((size = fread(p, 1, auxsize, fp1) ) < 0)
                break;
           if (gravarq(fp2, p, size) == ERRO)
		goto erro;
   } while (size > 0);
   free(p);
   return OK;
}




/**********************************************************************
EXEC_EXTERN:
	This routine executes an extern program, as "system".

Parameters:
	char	*cmd:	file to be executed (include parameters)
	int	*ret_code: out param that returns ret cod
	int	f_wait:	flag that determines if a wait must be executed
*************************************************************************/
exec_extern(cmd, ret_code, f_wait)
char	*cmd;
int	*ret_code, f_wait;
{
int	pid;

    pid = fork();
    if (pid == -1)
    {
	msg("Unable to fork");
	return ERRO;
    }

    if (pid == 0)
    {
	for (pid = maxfile(); pid > 2; pid--)
	   close(pid);
	execl(SHELL, SHELL, "-c", cmd, NULL);
	msg("Program not found");
	exit(1);
    }

    if (f_wait)
    {
	pid = wait(ret_code);
	if (pid < 0)
	   return ERRO;
    }
    return OK;
}
    
LeArqOp(dir, file, tab)
char    dir[], file[];
OPERADOR_MUTACAO        tab[];
{
FILE    *fp;
char    strop[10];
int     x, i, max;

   fp = abrearq(dir, file, "", 0);
   if (fp == NULL)
   {
        d_msg("Invalid operator file ", file, "");
        return ERRO;
   }
   while ( fscanf(fp, "%s %d %d", strop, &x, &max) == 3)
   {
        i = busca_op(strop);
        if (i >= 0)
        {
           do
           {
                if (x >= 0 && x <= 100)
                   tab[i].percent = x;
                else
                {
                   msg("Invalid Percentage in Operator File");
                   break;
                }
		tab[i].maximum = max;
                i = next_match(strop, i);
           } while (i >= 0);
        }
        else
           msg("Invalid Mutant Operator in Operator File");
   }
   return OK;
}


/*------------------------------------------------------------------------
Transforma varios strings separados por \0 num unuco string, substituindo
os \0 por space
-----------------------------------------------------------------------*/
char *build_param(char *x)
{
static char buf[1024];
char *p, *c;

   p = buf;
   buf[0] = 0;
   if (strlen(x) > sizeof(buf))
        return buf;
   inic_from_buf(x);
   for (c = from_buf(); c != NULL; c = from_buf())
   {
	strcpy(p,c);
	p += strlen(p);
	*p++ = ' ';
   }
   *p = '\0';
   return buf;
}



