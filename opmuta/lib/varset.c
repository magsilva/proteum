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


#include	<malloc.h>
#include	"opmuta.h"

#define		ADD_SIZE	128


void var_set_new(s) /* creates an empty set */
VAR_SET    *s;
{
   s->cont = 0;
   s->vet = NULL;
   s->size = 0;
}

int     var_set_add(s, c)
SIMBOLO    *c;
VAR_SET    *s;
{
SIMBOLO **p;
int     k;

   if (s->cont >= s->size)
   {
	p = malloc((s->size+ADD_SIZE)*sizeof(*(s->vet)));
	if (p == NULL)
	   return ERRO;
	if (s->vet)
	{
	   memcpy(p, s->vet, s->size*sizeof(*(s->vet)));
	   free(s->vet);
	}
	s->vet = p;
	s->size += ADD_SIZE;
   }
   s->vet[s->cont] = c;
   s->cont++;
   return OK;
}


SIMBOLO	*var_set_get(s, k)  /*devolve elemento do conjunto */
VAR_SET    *s;
int     k;
{

   return (k < s->cont) ? s->vet[k] : NULL ;
}



int var_set_inn(s, c)
char    *c;
VAR_SET    *s;
{
SIMBOLO	*p;
int     i;

   for (i = 0; i < s->cont; i++)
   {
        p = var_set_get(s, i);
        if (p == NULL)
           return  -1;
        if ( strcmp(p->nome, c) == 0)
           break;
   }
   return (i < s->cont) ? i : -1;
}


void var_set_del(s, k)
VAR_SET *s;
int     k;
{
int i;

   if (k >= s->cont)
     return;

   for (i = k+1; i < s->cont; i++)
   {
	s->vet[i-1] = s->vet[i];
   }
   s->cont--;
}




void    var_set_free(VAR_SET *p)
{
   if (p->vet)
        free(p->vet);
   var_set_new(p);
}


int     var_set_dup(VAR_SET *p, VAR_SET *q)
{
int i;
SIMBOLO    *c;

   var_set_new(q);
   for (c = var_set_get(p, i=0); c != NULL; c = var_set_get(p, ++i))
        if (var_set_add(q, c) == ERRO)
           return ERRO;
   return OK;
}




