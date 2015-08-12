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


/**
 * Split a string into several parameters.
 *
 * @return Array of param. The last param is NULL.
 */
char *
separa_param(char *param_string)
{
	char *p, *start_of_word;
	int c;
	enum states { DULL, IN_WORD, IN_STRING_DQUOTE, IN_STRING_SQUOTE } state = DULL;
	size_t argc = 0;
	char **argv = NULL;

	if (param_string == NULL) {
		return NULL;
	}

	// http://stackoverflow.com/a/26913698
	for (p = param_string; *p != '\0'; p++) {
	        c = (unsigned char) *p;
        	switch (state) {
		        case DULL:
		        	if (! isspace(c)) {
					if (c == '"' && state) {
						state = IN_STRING_DQUOTE;
						start_of_word = p + 1;
					} else if (c == '\'') {
						state = IN_STRING_SQUOTE;
						start_of_word = p + 1;
					} else {
						state = IN_WORD;
						start_of_word = p;
					}
					continue;
			case IN_STRING_DQUOTE:
				if (c == '"') {
					argv = realloc(argv, (argc + 1) * sizeof(char *));
					argv[argc] = malloc(sizeof(start_of_word) * sizeof(char));
					strcpy(argv[argc], start_of_word);
					argc++;
					state = DULL;
				}
				continue;
			case IN_STRING_SQUOTE:
				if (c == '\'') {
					argv = realloc(argv, (argc + 1) * sizeof(char *));
					argv[argc] = malloc(sizeof(start_of_word) * sizeof(char));
					strcpy(argv[argc], start_of_word);
					argc++;
					state = DULL;
				}
				continue;
			case IN_WORD:
				if (isspace(c)) {
					argv = realloc(argv, (argc + 1) * sizeof(char *));
					argv[argc] = malloc(sizeof(start_of_word) * sizeof(char));
					strcpy(argv[argc], start_of_word);
					argc++;
					state = DULL;
				}
				continue;
			}
		}
	}

	if (state != DULL) {
		free_array_str(argv);
		return NULL;
	} else {
		argv = realloc(argv, (argc + 1) * sizeof(char *));
		argv[argc] = NULL;
		return argv;
	}
}


/**
 * Free a null terminated array of strings.
 */
int
free_array_str(char *array[])
{
	char *p;
	for (p = array; *p != NULL; p++) {
		free(p);
	}
	free(array);
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

/**
 * Read file with mutation operators.
 */
int    
LeArqOp(char dir[], char file[], OPERADOR_MUTACAO tab[])
{
	FILE    *fp;
	char    strop[10];
	int     x, i;
	double  max;

	fp = abrearq(dir, file, "", 0);
	if (fp == NULL) {
		fprintf(stderr, "Invalid operator file %s", file);
		return ERRO;
	}
	while (fscanf(fp, "%s %lf %d", strop, &x, &max) == 3) {
		i = busca_op(strop);
		if (i >= 0) {
			do {
				if (x >= 0.0 && x <= 1.0) {
					tab[i].percentage = x;
				} else {
					fprintf(stderr, "Invalid percentage for operator %i in the file %s: %lf", i, file, x);
					return ERRO;
				}
				tab[i].maximum = max;
		                i = next_match(strop, i);
			} while (i >= 0);
		} else {
			fprintf(stderr, "Invalid mutant operator %d in the file %s", i, file);
			return ERRO;
		}
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


/*----------------------------------------------------------------------------
SHUFLE:
Embaralha alearoriamente os elementos de um vetor
Parametros:
   o vetor
   semente a ser usada na geração aleatoria
Author: Delamaro 10/jul/2013
----------------------------------------------------------------------------*/
void shufle(int v[], long seed, int n)
{
    int i, aux;
    long int k;

    srandom(seed);
    for (i = n - 1; i > 1; i--)
    {
        long int k;
        k = random();
        k %= i;
        aux = v[i];
        v[i] = v[k];
        v[k] = aux;
    }
}
