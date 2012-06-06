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


#include	<stdlib.h>

#include	<lib/gerais.h>

#include	"tcase.h"

int		ins_node_log(LOG_TCASE *, long , int );
void 		set_log_bit(char *, int , int );
int 		get_log_bit(char *, int );


LOG_TCASE *get_log_from_file(FILE *fp)
{
int	i, node;
long	func;
LOG_TCASE *pr;

   i = fscanf(fp, "%ld", &func);
   pr = new_log();
   if (pr == NULL)
	   return NULL;

   if ( i == 0)
      return OK;
   if ( func >= 0)
   {
	msg("invalid log file");
	erro:
	release_log(pr);
	return NULL;
   }
   do {
	i = fscanf(fp, "%d", &node);
	if (i <= 0)
	   break;
	if ( node <= 0)
	{
	   func = node;
	   continue;
	}
	if (ins_node_log(pr, -func, node) == ERRO)
	   goto erro;
   } while (func != 0);

   return pr;
}


LOG_TCASE	*new_log()
{
LOG_TCASE	*p;

   p = malloc(sizeof(LOG_TCASE));
   if (p == NULL)
   {
	msg("Insuficient memory");
	return NULL;
   }
   p->nfunction = p->vetsize = 0;
   p->vetfunc = NULL;
   return p;
}


int	ins_node_log(LOG_TCASE *log, long func, int node)
{
struct vetfunc *p;
int	i,j;

    if (log == NULL)
	return OK;

   i = 0;
   p = log->vetfunc;
   while ( i < log->nfunction && func > p->function)
   {
	i++;
	p++;
   }

   if ( i == log->nfunction ) /* did not find function, insert at the end */
   {
	if ( log->nfunction == log->vetsize )
		if ( double_vet(log) == ERRO)
		   return ERRO;
	p = &log->vetfunc[i];
	p->function = func;
	p->nnode = 0;
	p->nodes = NULL;
	log->nfunction++;
	return ins_node_func(p, node);
    }

    if ( func == p->function)
  	return ins_node_func(p, node);
    else
    {
	if (log->nfunction == log->vetsize )
		if ( double_vet(log) == ERRO)
		   return ERRO;
	for (j = log->nfunction; j > i; j--)
	   log->vetfunc[j] = log->vetfunc[j-1];

	p = &log->vetfunc[i];
	p->function = func;
	p->nnode = 0;
	p->nodes = NULL;
	log->nfunction++;
	return ins_node_func(p, node);
    }

    return OK;
}
	

int	test_node_log(LOG_TCASE *log, long func, int node)
{
int	i;
struct vetfunc *p;

   if ( log == NULL)
	return FALSE;
   for (i = 0; i < log->nfunction; i++)
   {
	p = &(log->vetfunc[i]);
	if ( p->function == func )
	   return (node == 0 ? TRUE: get_log_bit(p->nodes, node)) ;
	if ( p->function > func )
	   break;
   }
   return FALSE;
}


int	ins_node_func(struct vetfunc *p, int node)
{
char *s;
int k;

   if (node <= 0)
	return OK;
   if ( node > p->nnode )
   {
	k = (node -1 ) / 32 + 1;
	k *= 4;
	s = realloc(p->nodes, k);
	k *= 8;
	if (s == NULL)
	{
	   msg("Insuficient memory");
	   return ERRO;
	}
	memset(&s[p->nnode/8], 0, (k - p->nnode) / 8);
	p->nodes = s;
	p->nnode = k;
    }
    set_log_bit(p->nodes, node, 1);
    return OK;
}


void set_log_bit(char *s, int node, int k)
{
int j, i;
unsigned char c;

   j = (node - 1) / 8;
   i = (node - 1) % 8;
   c = 0x80 >> i;
   if ( k )
      	s[j] |= c;
   else
 	s[j] &= ~c;
}

int get_log_bit(char *s, int node)
{
int j, i;
unsigned char c;

   j = (node - 1) / 8;
   i = (node - 1) % 8;
   c = 0x80 >> i;
   c = s[j] & c;
   return c != 0;
}
    

int	double_vet(LOG_TCASE *log)
{
int size;
struct vetfunc *p;

   size = (log->vetsize == 0 ? 16: log->vetsize);
   p = realloc(log->vetfunc, (size+log->vetsize)*sizeof(struct vetfunc));
   if (p == NULL)
   {
	msg("Insuficient memory");
	return ERRO;
   }
   log->vetfunc = p;
   memset(&(log->vetfunc[log->vetsize]), 0, size*sizeof(struct vetfunc));
   log->vetsize += size;
   return OK;
}


void	release_log(LOG_TCASE *log)
{
int i;
struct vetfunc *p;

   if (log == NULL)
	return;
   for (i = 0; i < log->nfunction; i++)
   {
	p = &(log->vetfunc[i]);
	if (p->nodes != NULL)
	   free(p->nodes);
   }
   free(log->vetfunc);
}


int save_log(LOG_TCASE *log, FILE *fp)
{
int 	i, j, k;
struct vetfunc *p;
int	*pi;

    if (log->nfunction == 0)
	return OK;
    j = fprintf(fp, "%d\n", log->nfunction);
    if (j <= 0)
    {
	erro:
	msg("Error writing tcase log");
	return ERRO;
    }

    for (i = 0; i < log->nfunction; i++)
    {
	p = &(log->vetfunc[i]);
	j = fprintf(fp, "%ld %d \n", p->function, p->nnode);
	for ( k = 0, pi = (int *) p->nodes; j > 0 && k < p->nnode / 32; 
							k++, pi++)
	   j = fprintf(fp, "%x ", *pi);
	if (j <= 0)
	   goto erro;
    }
    return OK;
}


int load_log(LOG_TCASE *log, FILE *fp, long v1, long v2)
{
int	i, j, k, l, r, m, n, nodes, cont;
unsigned int *pi, u;
long	func;
struct vetfunc *p;

    i = fscanf(fp, "%d", &n);
    if (i != 1)
    {
	erro:
	msg("Error reading log file");
	return ERRO;
    }
    if ( v2 < v1 )
    {
	func = v1;
	v1 = v2;
	v2 = func;
    }
    cont = (v1 < 0 ? -1 : 2);
    for (i = m = 0; i < n && cont != 0; i++)
    {
	j = fscanf(fp, "%ld %d", &func, &nodes);
	if (j != 2)
	   goto erro;
	if ( v1 > 0 && func > v2 )
	   break;
	k = (v1 < 0) || (v1 == func) || (v2 == func);
	l = nodes / 8;
	if ( k )
	{
	   if ( ins_node_log(log, func, 0) == ERRO)
		goto erro;
	   p = &(log->vetfunc[m]);
	   p->nodes = malloc(l);
	   if (p->nodes == NULL)
	   {
		msg("Insuficient memory");
		return ERRO;
	   }
	   cont--;
	   m++;
	   pi = (unsigned int *) p->nodes;
	   p->nnode = nodes;
	}
	for (j = 0; j < nodes/32; j++, pi++)
	{
	    r = fscanf(fp, "%x", &u);
	    if (r != 1)
		goto erro;
	    if (k)
		*pi = u;
	}
    }
    return OK;
}
