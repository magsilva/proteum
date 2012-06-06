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

#include	<search.h>

#include        <lib/gerais.h>
#include        <li/lib/li.h>
#include        <muta/lib/muta.h>
#include        "opmuta.h"

#define		MAX_FUNC	1000

#ifdef		SUNOS4

ENTRY		*hsearch(ENTRY, ACTION);
int		hcreate(unsigned);

#endif

LI_SIMBOLO	*list_func[MAX_FUNC];

int		nlist_func;

int	load_list_func(char *f)
{
FILE *fp;
LI_SIMBOLO symli, *p;
ENTRY	x;

   fp = abrearq("", f, "", 0);
   if (fp == NULL)
	return ERRO;
   if (hcreate(MAX_FUNC) == 0)
   {
	fclose(fp);
	return ERRO;
   }

   while (fscanf(fp, "%s %d %ld %d %d", symli.simbolo, 
                      &symli.inicio, &symli.comprimento, &symli.linha) == 4)
   {
	list_func[nlist_func++] = p = malloc(sizeof(LI_SIMBOLO));
	*p = symli;
	x.key = (void *) &(p->simbolo);
	x.data = (void *) p;
        if (hsearch(x, ENTER) == NULL)
	{
	   libera_list_func();
	   fecharq(fp);
	   msg("Error reading function table");
	   return ERRO;
	}
    }
    fecharq(fp);
    return OK;
}


libera_list_func()
{
int i;

   for (i = 0; i < nlist_func; i++)
   {
	free(list_func[i]);
   }
   hdestroy();
}


LI_SIMBOLO *get_list_func(char *f)
{
ENTRY *p, x;
LI_SIMBOLO y;

    x.key = (void *) f;
    x.data = (void *) &y;
    p = hsearch(x, FIND);
    if ( p == NULL)
	return NULL;

    return (LI_SIMBOLO *) p->data;
}

 

 
