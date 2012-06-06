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
 
#include	"btree.h"
 

int     find_page0(FILE *);
int	write_page(BTREE *, void *);
int     write_page_zero(BTREE *);
void	*get_root(BTREE *);
void    *new_page(BTREE *);
long	new_reg(BTREE *, int);
int     insert_key(BTREE *, long , KEY *, ITEM *, KEY **);
long	address_of(BTREE *, void *);
int	delete_key(BTREE *, long, KEY *); 
int	seq_key(BTREE *, long, int , KEY *, int *);

 
long	bufpage[PAGESIZE/sizeof(long)];

BTREE	*BTREE_create(char *dfile, char *ifile, int keysize, 
		     int (*cmp_key)(), void (* print_key)())
{
BTREE	*p;
long	t;
int	k;
PAGE_ZERO *pz;

   p = (BTREE *) malloc(sizeof(BTREE));
   if (p == NULL)
   {
	btree_errno = MEMORY;
	return NULL;
   }

   memset(p, 0, sizeof(*p));

   p->datafile = fopen(dfile, "r+");
   if (p->datafile == NULL)
   {
   	p->datafile = fopen(dfile, "w+");
	if (p->datafile == NULL)
	{
	   btree_errno = DATA_WRITE;
	   l1:
	   free(p);
	   return NULL;
	}
   }

   if (ifile == NULL)
	p->indfile = p->datafile;
   else
   p->indfile = fopen(ifile, "r+");
   if (p->indfile == NULL)
   {
   	p->indfile = fopen(ifile, "w+");
	if (p->indfile == NULL)
	{
	   btree_errno = IND_WRITE;
	   l2:
	   fclose(p->datafile);
	   goto l1;
	}
   }

   fseek(p->indfile, 0, SEEK_END);
   t = ftell(p->indfile);

   while (t % PAGESIZE != 0)
   {
	fprintf(p->indfile, "%c", DUMMY_CHAR);
	t++;
   }

   p->pz = t;
   p->root = BTREE_NULL;
   p->realkeysize = keysize;
   p->keysize = (keysize / sizeof(void *) + 1) * sizeof(void *);
   p->totkeys = 0;
   pz = (PAGE_ZERO *) bufpage;
   pz->label = BTREE_LABEL;
   fwrite(pz, PAGESIZE, 1, p->indfile);
  
   if (write_page_zero(p) < 0)
   {
	fclose(p->indfile);
	goto l2;
   }

   k = PAGESIZE - sizeof(PAGE_ADDRESS);

   p->order = (k / p->keysize) / 2 * 2;

#ifdef DEBUG
   p->order = 4;
#endif

   if (p->order > MAXKEYS)
	p->order = MAXKEYS;

   p->compare_key = cmp_key;
   p->print_key = print_key;

   return p;
}



	   
void	BTREE_close(BTREE *bt)
{
int	i;

   if (bt == NULL)
	return;

   write_page_zero(bt);

   fclose(bt->datafile);
   if (bt->datafile != bt->indfile)
	fclose(bt->indfile);
   for (i = 0; i < NPAGEPOOL; i++)
   {
	if (bt->pool[i].buffer != NULL)
	   free(bt->pool[i].buffer);
   }

   free(bt);
}


BTREE   *BTREE_open(char *dfile, char *ifile, int keysize,
                     int (*cmp_key)(), void (* print_key)())
{
BTREE   *p;
int     k;
PAGE_ZERO *pz;

   p = (BTREE *) malloc(sizeof(BTREE));
   if (p == NULL)
   {
	btree_errno = MEMORY;
        return NULL;
   }

   memset(p, 0, sizeof(*p));

   p->datafile = fopen(dfile, "r+");
   if (p->datafile == NULL)
   {
	   btree_errno = DATA_READ;
           l1:
           free(p);
	   return NULL;
   }

   if (ifile == NULL)
        p->indfile = p->datafile;
   else
   p->indfile = fopen(ifile, "r+");
   if (p->indfile == NULL)
   {
           btree_errno = IND_READ;
           l2:
           fclose(p->datafile);
           goto l1;
   }

   if (find_page0(p->indfile) < 0)
   {
	l3:
	fclose(p->indfile);
	goto l2;
   }
   p->pz = ftell(p->indfile);

   if (get_page_zero(p) < 0)
	goto l3;

   k = PAGESIZE - sizeof(PAGE_ADDRESS);

   p->realkeysize = keysize;
   p->keysize = (keysize / sizeof(void *) + 1) * sizeof(void *);
   p->order = (k / p->keysize) / 2 * 2;

#ifdef DEBUG
   p->order = 4;
#endif

   if (p->order > MAXKEYS)
        p->order = MAXKEYS;

   p->compare_key = cmp_key;
   p->print_key = print_key;

   return p;
}



int	BTREE_insert_key(BTREE *bt, KEY *key, long address, int size)
{
void	*root, *new_root;
KEY	*hkey;
ITEM	u;
PAGE_ADDRESS	*p;
KEY	*pk;

   u.address = address;
   u.size = size;

   if (insert_key(bt, bt->root, key, &u, &hkey) < 0)
	return -1;

   if (hkey != NULL)
   {
	/* insert in the root */
	new_root = new_page(bt);
	if (new_root == NULL)
	   return -1;

	p = (PAGE_ADDRESS *) new_root;
	pk = nth_key(bt, p, 0);

	p->label = BTREE_LABEL;
	p->nkeys = 1;
	p->next0 = bt->root;
	p->s[0] = u;
	p->child0 = bt->totkeys - u.child;
	memcpy(pk, hkey, bt->realkeysize);
	bt->root = address_of(bt, p);
	if (write_page(bt, p) < 0 < 0)
	 	return -1;
   }

   bt->totkeys++;
   if (write_page_zero(bt)  < 0)
	return -1;
   return 0;
}


int	BTREE_insert_data(BTREE *bt, KEY *key, void *reg, int size)
{
long	l;

   l = new_reg(bt, size);
   if (l  < 0)
  	return -1;

   fseek(bt->datafile, l, SEEK_SET);
   if (fwrite(reg, size, 1, bt->datafile) != 1)
   {
	btree_errno = DATA_WRITE;
	return -1;
   }

   return BTREE_insert_key(bt, key, l, size);
}


int     BTREE_rewrite_data(BTREE *bt, KEY *key, void *reg)
{
long    l;
int size;

   if (BTREE_get_key(bt, key, &l, &size) < 0)
	return -1;

   fseek(bt->datafile, l, SEEK_SET);
   if (fwrite(reg, size, 1, bt->datafile) != 1)
   {
        btree_errno = DATA_WRITE;
        return -1;
   }

   return 0;
}

 

void	aux_print(BTREE *bt, long l, int k)
{
int	i;
PAGE_ADDRESS *p;
KEY	*pk;

   if (l == BTREE_NULL)
	return;
   p = (PAGE_ADDRESS *) get_page(bt, l);
   printf("\n");
   for (i = 0; i < k; i++)
   {
	printf("  ");
   }
   printf("%d - ", p->child0);
   for (i = 0; i < p->nkeys; i++)
   {
	pk = nth_key(bt, p, i);
	printf("(");
	bt->print_key(pk);
	printf(",%ld,%d,%d)", p->s[i].address, p->s[i].size, p->s[i].child);
   }
   aux_print(bt, p->next0, k+1);
   for (i = 0; i < p->nkeys; i++)
	aux_print(bt, p->s[i].next, k+1);
}

 
void	BTREE_print(BTREE *bt)
{
   aux_print(bt, bt->root, 1);
}


int	BTREE_get_key(BTREE *bt, KEY *key, long *address, int *size)
{
ITEM	u;
   
   if (search_key(bt, bt->root, key, &u) < 0)
	return -1;

   *address = u.address;
   *size = u.size;
   return 0;
}


int	BTREE_get_data(BTREE *bt, KEY *key, void *reg, int *size)
{
long	l;
int	i, k;

   i = BTREE_get_key(bt, key, &l, &k);
   if (i < 0)
	return i;

   if (k < 0)
   {
	*size = k;
	return 0;
   }

   fseek(bt->datafile, l, SEEK_SET);
   if (fread(reg, k, 1, bt->datafile) != 1)
   {
	btree_errno = DATA_READ;
	return -1;
   }

   *size = k;
   return 0;
}



int     BTREE_get_nth_key(BTREE *bt, int n, KEY *key, long *add, int *size)
{
ITEM u;
int     k;

    if (bt->totkeys < n)
    {
        *size = -1;
        *add = BTREE_NULL;
	btree_errno = REG_NUMBER;
        return -1;
    }

    if (get_nth(bt, bt->root, n, key, &u) < 0)
	return -1;

    *size = u.size;
    *add = u.address;
    return 0;
}


int     BTREE_get_nth_data(BTREE *bt, int n, KEY *key, void *reg, int *size)
{
long    l;
int     i, k;

   if (BTREE_get_nth_key(bt, n, key, &l, &k) < 0)
	return -1;

   fseek(bt->datafile, l, SEEK_SET);
   if (fread(reg, k, 1, bt->datafile) != 1)
   {
	btree_errno = DATA_READ;
        return -1;
   }

   *size = k;
   return 0;
}



void    BTREE_print_nth(BTREE *bt)
{
int	i;
long	l;
int	size;
KEY	*pk;

   pk = (KEY *) malloc(bt->keysize);
   if (pk == NULL)
	return;
   for (i = 1; i <= bt->totkeys; i++)
   {
	BTREE_get_nth_key(bt, i, pk, &l, &size);
	printf("\n");
	bt->print_key(pk);
        printf(",%ld,%d", l, size);
   }
   free(pk);
}

int     BTREE_delete_key(BTREE *bt, KEY *key)
{

   if (delete_key(bt, bt->root, key) < 0)
        return -1;

   return 0;
}


int     BTREE_delete_nth_key(BTREE *bt, int n)
{
KEY	*key;
ITEM	u;
int	k;

    if (bt->totkeys < n)
    {
        btree_errno = REG_NUMBER;
        return -1;
    }

    key = (KEY *) malloc(bt->keysize);
    if (key == NULL)
    {
	btree_errno = MEMORY;
 	return -1;
    }

    if (get_nth(bt, bt->root, n, key, &u) < 0)
    {
	free(key);
        return -1;
    }

    if (u.size < 0)
    {
	free(key);
	btree_errno = DEL_KEY;
	return -1;
    }

    k = BTREE_delete_key(bt, key);
    free(key);
    return k;
}


int	BTREE_seq_key(BTREE *bt, KEY *key, int *n)
{
   return seq_key(bt, bt->root, 0, key, n);
}


