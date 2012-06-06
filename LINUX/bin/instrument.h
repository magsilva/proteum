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


#include <stdlib.h>
#include <string.h>

typedef struct {
		int	nfunction;	/* number of visited functions */
		int	vetsize;	/* number of avaiable places in vet */
		struct vetfunc
		{
		   long		function; /* function identifier */
		   int		nnode;	/* number of nodes per functio */
		   char		*nodes; /* one bit per node */
		}		*vetfunc; /* array of functions */
		} TRACE_LOG;


static ins_node_func();
static set_log_bit();
static get_log_bit();
static double_vet();
static msg();


static TRACE_LOG	*new_log()
{
TRACE_LOG	*p;

   p = (TRACE_LOG *) malloc(sizeof(TRACE_LOG));
   if (p == NULL)
   {
	msg("Insuficient memory");
	return NULL;
   }
   p->nfunction = p->vetsize = 0;
   p->vetfunc = NULL;
   return p;
}


static int	ins_node_log(TRACE_LOG *log, long func, int node)
{
struct vetfunc *p;
int	i,j;

    if (log == NULL)
	return 0;

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
		if ( double_vet(log) == (-1))
		   return (-1);
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
		if ( double_vet(log) == (-1))
		   return (-1);
	for (j = log->nfunction; j > i; j--)
	   log->vetfunc[j] = log->vetfunc[j-1];

	p = &log->vetfunc[i];
	p->function = func;
	p->nnode = 0;
	p->nodes = NULL;
	log->nfunction++;
	return ins_node_func(p, node);
    }

    return 0;
}
	

static int	test_node_log(TRACE_LOG *log, long func, int node)
{
int	i;
struct vetfunc *p;

   if ( log == NULL)
	return 0;
   for (i = 0; i < log->nfunction; i++)
   {
	p = &(log->vetfunc[i]);
	if ( p->function == func )
	   return (node == 0 ? 1: get_log_bit(p->nodes, node)) ;
	if ( p->function > func )
	   break;
   }
   return 0;
}


static int	ins_node_func(struct vetfunc *p, int node)
{
char *s;
int k;

   if (node <= 0)
	return 0;
   if ( node > p->nnode )
   {
	k = (node -1 ) / 32 + 1;
	k *= 4;
	s = (char *) realloc(p->nodes, k);
	k *= 8;
	if (s == NULL)
	{
	   msg("Insuficient memory");
	   return (-1);
	}
	memset(&s[p->nnode/8], 0, (k - p->nnode) / 8);
	p->nodes = s;
	p->nnode = k;
    }
    set_log_bit(p->nodes, node, 1);
    return 0;
}


static set_log_bit(char *s, int node, int k)
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

static int get_log_bit(char *s, int node)
{
int j, i;
unsigned char c;

   j = (node - 1) / 8;
   i = (node - 1) % 8;
   c = 0x80 >> i;
   c = s[j] & c;
   return c != 0;
}
    

static int	double_vet(TRACE_LOG *log)
{
int size;
struct vetfunc *p;

   size = (log->vetsize == 0 ? 16: log->vetsize);
   p = (struct vetfunc *) realloc(log->vetfunc, 
			         (size+log->vetsize)*sizeof(struct vetfunc));
   if (p == NULL)
   {
	msg("Insuficient memory");
	return (-1);
   }
   log->vetfunc = p;
   memset(&(log->vetfunc[log->vetsize]), 0, size*sizeof(struct vetfunc));
   log->vetsize += size;
   return 0;
}


static msg(char *x)
{
   perror(x);
}
