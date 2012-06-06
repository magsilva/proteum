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


#include	<lib/gerais.h>

#include	"btree.h"
 

void    reloca_pool(BTREE *, long);
void	*buffer_of(BTREE *, long);
long	address_of(BTREE *, void *);
int	page_of(BTREE *, long);
void	*get_root(BTREE *);
void	*get_page(BTREE *, long);

extern long	bufpage[];

int     find_page0(FILE *fp)
{
long    l;

   fseek(fp, 0, SEEK_SET);

   while (fread(&l, sizeof(l), 1, fp) == 1)
   {
        fseek(fp, - sizeof(l), SEEK_CUR);
        if (l == BTREE_LABEL)
           return 0;
        fseek(fp, PAGESIZE, SEEK_CUR);
   }
   btree_errno = INV_IND;
   return -1;
}


int	write_page(BTREE *bt, void *page)
{
int	i;
long	l;

   l = address_of(bt, page);
   if (l < 0)
	return -1;

   fseek(bt->indfile, l, SEEK_SET);
   if (fwrite(page, PAGESIZE, 1, bt->indfile) != 1)
   {
	   btree_errno = IND_WRITE;
	   return -1;
   }
   return 0;
}


int	write_page_zero(BTREE *bt)
{
PAGE_ZERO	*p;

   fseek(bt->indfile, bt->pz, SEEK_SET);
   if (fread(bufpage, PAGESIZE, 1, bt->indfile) != 1)
   {
	btree_errno = IND_READ;
	return -1;
   }

   p = (PAGE_ZERO *) bufpage;
   if (p->label != BTREE_LABEL)
   {
	btree_errno = INV_IND;
	return -1;
   }

   p->label = BTREE_LABEL;
   p->root = bt->root;
   p->totkeys = bt->totkeys;

   fseek(bt->indfile, bt->pz, SEEK_SET);
   if (fwrite(bufpage, PAGESIZE, 1, bt->indfile) != 1)
   {
	btree_errno = IND_WRITE;
        return -1;
   }

   return 0;
}

int     get_page_zero(BTREE *bt)
{
PAGE_ZERO       *p;

   fseek(bt->indfile, bt->pz, SEEK_SET);
   if (fread(bufpage, PAGESIZE, 1, bt->indfile) != 1)
   {
        btree_errno = IND_READ;
        return -1;
   }

   p = (PAGE_ZERO *) bufpage;
   if (p->label != BTREE_LABEL)
   {
        btree_errno = INV_IND;
        return -1;
   }

   bt->root = p->root;
   bt->totkeys = p->totkeys;

   return 0;
}


void	*new_page(BTREE *bt)
{
long	l;
int	i;
void 	*p;

   fseek(bt->indfile, 0, SEEK_END);
   l = ftell(bt->indfile);
   i = NPAGEPOOL-1;
   if (bt->pool[i].buffer != NULL)
	p = bt->pool[i].buffer;
   else
	p = (void *) malloc(PAGESIZE);

   if (p == NULL)
   {
	btree_errno = MEMORY;
	return NULL;
   }

   memset(p, 0, PAGESIZE);
   reloca_pool(bt, BTREE_NULL);
   bt->pool[0].pageaddress = l;
   bt->pool[0].buffer = p;
   return p;
}


long	new_reg(BTREE *bt, int size)
{
long l;

   fseek(bt->datafile, 0, SEEK_END);
   l = ftell(bt->datafile);
   return l;
}






void 	reloca_pool(BTREE *bt, long address)
{
int	i;

   if (address == BTREE_NULL)
	i = NPAGEPOOL - 1;
   else
	i = page_of(bt, address);

   for (; i > 0; i--)
   {
	bt->pool[i] = bt->pool[i-1];
   }
}


void	*buffer_of(BTREE *bt, long l)
{
int	i;

   for (i = 0; i < NPAGEPOOL; i++)
   {
	if (bt->pool[i].pageaddress == l)
	   return bt->pool[i].buffer;
   }
   btree_errno = INTERN_MIST;
   return NULL;
}


long	address_of(BTREE *bt, void *l)
{
int     i;

   for (i = 0; i < NPAGEPOOL; i++)
   {
        if (bt->pool[i].buffer== l)
           return bt->pool[i].pageaddress;
   }
   btree_errno = INTERN_MIST;
   return -1;
}

int	page_of(BTREE *bt, long l)
{
int     i;

   for (i = 0; i < NPAGEPOOL; i++)
   {
        if (bt->pool[i].pageaddress == l)
           return i;
   }
   btree_errno = INTERN_MIST;
   return -1;
}


void	*get_root(BTREE *bt)
{

   return (get_page(bt, bt->root));
}


void	*get_page(BTREE *bt, long l)
{
void *p;
int i;

   p = buffer_of(bt, l);
   if (p != NULL)
   {
	reloca_pool(bt, l);
	bt->pool[0].buffer = p;
	bt->pool[0].pageaddress = l;
	return p;
   }

   i = NPAGEPOOL-1;
   if (bt->pool[i].buffer != NULL)
        p = bt->pool[i].buffer;
   else
        p = (void *) malloc(PAGESIZE);

   if (p == NULL)
   {
	btree_errno = MEMORY;
        return NULL;
   }
   reloca_pool(bt, BTREE_NULL);
   bt->pool[0].buffer = p;
   bt->pool[0].pageaddress = l;
   fseek(bt->indfile, l, SEEK_SET);
   if ( fread(p, PAGESIZE, 1, bt->indfile) != 1)
   {
	btree_errno = IND_READ;
	return NULL;
   }
   if (((PAGE_ADDRESS *)p)->label != BTREE_LABEL)
   {
	btree_errno = INV_IND;
	return NULL;
   }
   return p;
}
   


int	insert_key(BTREE *bt, long page, KEY *key, ITEM *u, KEY **hkey)
{
PAGE_ADDRESS *p, *b;
KEY *pk, *pkk, *vkey;
long	nx;
int	k, l, r, n;
ITEM	v;

   *hkey = NULL;
   if (page == BTREE_NULL)
   {
	u->next = page;
	u->child = 0;
	*hkey = (KEY *) malloc(bt->keysize);
	memcpy(*hkey, key, bt->realkeysize);
        return 0;
   }

   p = (PAGE_ADDRESS *) get_page(bt, page);
   pk = (KEY *) (p + 1);

   l = 0; r = p->nkeys-1;
   do {
	k = (l + r) / 2;
	pkk = nth_key(bt, p, k);

	if (bt->compare_key((void *) key, pkk) < 0) r = k - 1;
	else

	if (bt->compare_key((void *) key, pkk) > 0) l = k + 1;
	else
	   break;
   } while (r >= l);

   if (r >= l)
   {
	if (p->s[k].size < 0) /* chave foi deletada */
	{
	   p->s[k].size = u->size;
	   p->s[k].address = u->address;
	   memcpy(nth_key(bt, p, k), key, bt->realkeysize);
	   return 0;
	} 
	btree_errno = DUP_KEY;
	return -1;	/* chave jah existe */
   }

   if (r < 0)
      	nx = p->next0;
   else
	nx = p->s[r].next;

   v = *u;
   if ((k = insert_key(bt, nx, key, &v, &vkey)) < 0)
	return k;

   if (vkey == NULL)
   {
     if ( r < 0)
     {
	p->child0 += v.child;
     }
     else
     {
	p->s[r].child += v.child;
     }
     *u = v;
     write_page(bt, p);
     return 0;
   }	

   p->s[r].child -= v.child;

   if (p->nkeys < bt->order)
   {
	for (k = p->nkeys; k > r+1; k--)
	{
	   p->s[k] = p->s[k-1];
	   pkk = nth_key(bt, p, k);
	   pk = nth_key(bt, p, k-1);
	   memcpy(pkk, pk, bt->realkeysize);
	}
	memcpy(nth_key(bt, p, k), vkey, bt->realkeysize);
	p->s[k] = v;
	p->nkeys++;
	u->child = 1;
	free(vkey);
	write_page(bt, p);
	return 0;
   }

   b = (PAGE_ADDRESS *) new_page(bt);
   b->label = BTREE_LABEL;
   n = bt->order / 2 - 1;
   if (r <= n)
   {
	if (r != n)
	{
	   *u = p->s[n];
	   *hkey = (KEY *) malloc(bt->keysize);
	   memcpy(*hkey, nth_key(bt, p, n), bt->realkeysize);
	   for (k = n ; k > r+1; k--)
	   {
	   	p->s[k] = p->s[k-1];
	   	pkk = nth_key(bt, p, k);
	   	pk = nth_key(bt, p, k-1);
	   	memcpy(pkk, pk, bt->realkeysize);
	   }
	   memcpy(nth_key(bt, p, k), vkey, bt->realkeysize);
	   p->s[k] = v;
	   free(vkey);
	}
	else
	{
	   *u = v;
	   *hkey = vkey;
	}
	n++;
	
	for (k = 0; k < n; k++)
	{
	   b->s[k] = p->s[k+n];
	   memcpy(nth_key(bt, b, k), nth_key(bt, p, k+n), bt->realkeysize);
	}
   }
   else
   {
	r = r - n - 1;
	*u = p->s[n+1];
	*hkey = (KEY *) malloc(bt->keysize);
	memcpy(*hkey, nth_key(bt, p, n+1), bt->realkeysize);
	for (k = 0; k < r; k++)
	{
	   
	   b->s[k] = p->s[k+n+2];
	   memcpy(nth_key(bt, b, k), nth_key(bt, p, k+n+2), bt->realkeysize);
	}
	memcpy(nth_key(bt, b, k), vkey, bt->realkeysize);
	b->s[k] = v;
	free(vkey);
	n++;
	for (k = r+1; k < n; k++)
	{
	   
	   b->s[k] = p->s[k+n];
	   memcpy(nth_key(bt, b, k), nth_key(bt, p, k+n), bt->realkeysize);
	}
   }
   p->nkeys = b->nkeys = n;
   b->next0 = u->next;
   b->child0 = u->child;
   u->next = address_of(bt, b);
   u->child = n + b->child0;
   for (k = 0; k < n; k++)
      u->child += b->s[k].child;
   write_page(bt, p);
   write_page(bt, b); 
   return 0;
}




int     search_key(BTREE *bt, long page, KEY *key, ITEM *u)
{
PAGE_ADDRESS *p;
KEY *pk, *pkk;
long    nx;
int	l, r, k;

   if (page == BTREE_NULL)
   {
        u->next = page;
	u->address = BTREE_NULL;
	u->size = -1;
	btree_errno = NON_KEY;
        return -1;
   }

   p = (PAGE_ADDRESS *) get_page(bt, page);
   pk = (KEY *) (p + 1);

   l = 0; r = p->nkeys-1;
   do {
        k = (l + r) / 2;
        pkk = nth_key(bt, p, k);

        if (bt->compare_key((void *) key, pkk) < 0) r = k - 1;
        else

        if (bt->compare_key((void *) key, pkk) > 0) l = k + 1;
        else
           break;
   } while (r >= l);

   if (r >= l)
   {
	*u = p->s[k];
        return 0;      /* chave jah existe */
   }

   if (r < 0)
        nx = p->next0;
   else
        nx = p->s[r].next;

   return search_key(bt, nx, key, u);
}


int     delete_key(BTREE *bt, long page, KEY *key)
{
PAGE_ADDRESS *p;
KEY *pk, *pkk;
long    nx;
int     l, r, k;

   if (page == BTREE_NULL)
   {
        btree_errno = NON_KEY;
        return -1;
   }

   p = (PAGE_ADDRESS *) get_page(bt, page);
   pk = (KEY *) (p + 1);

   l = 0; r = p->nkeys-1;
   do {
        k = (l + r) / 2;
        pkk = nth_key(bt, p, k);

        if (bt->compare_key((void *) key, pkk) < 0) r = k - 1;
        else

        if (bt->compare_key((void *) key, pkk) > 0) l = k + 1;
        else
           break;
   } while (r >= l);

   if (r >= l)
   {
	if (p->s[k].size < 0)
	{
	   btree_errno = DEL_KEY;
	   return -1;
	}
	p->s[k].size *= -1;
        return 0;      
   }

   if (r < 0)
        nx = p->next0;
   else
        nx = p->s[r].next;

   return delete_key(bt, nx, key);
}


int     get_nth(BTREE *bt, long page, int n, KEY *key, ITEM *u)
{
PAGE_ADDRESS *p;
int     i, cont;

   if (page == BTREE_NULL)
   {
        btree_errno = INTERN_MIST;
        return -1;
   }

   p = (PAGE_ADDRESS *) get_page(bt, page);

   if (p->child0 >= n)
        return get_nth(bt, p->next0, n, key, u);

   n -= p->child0;
   i = 0;
   while (i < p->nkeys-1)
   {
        if (1 == n)
        {
           l1:
           memcpy(key, nth_key(bt, p, i), bt->realkeysize);
           *u = p->s[i];
           return 0;
        }
        n--;
        if (n <= p->s[i].child)
           return get_nth(bt, p->s[i].next, n , key, u);
        n-= p->s[i].child;
        i++;
   }
   if (n == 1)
        goto l1;

   return get_nth(bt, p->s[i].next, n-1 , key, u);
}

int     seq_key(BTREE *bt, long page, int m, KEY *key, int *t)
{
PAGE_ADDRESS *p;
KEY *pk, *pkk;
long    nx;
int     l, r, k;

   if (page == BTREE_NULL)
   {
	*t = m + 1;
        return 0;
   }

   p = (PAGE_ADDRESS *) get_page(bt, page);
   pk = (KEY *) (p + 1);

   l = 0; r = p->nkeys-1;
   do {
        k = (l + r) / 2;
        pkk = nth_key(bt, p, k);

        if (bt->compare_key((void *) key, pkk) < 0) r = k - 1;
        else

        if (bt->compare_key((void *) key, pkk) > 0) l = k + 1;
        else
           break;
   } while (r >= l);

   if (r >= l)
   {
	m += p->child0+1;
	for (r = 0; r < k; r++)
	   m += p->s[r].child + 1;
	*t = m;
        return 0;      /* chave existe */
   }

   if (r < 0)
        nx = p->next0;
   else
   {
        nx = p->s[r].next;
        m += p->child0 + 1;
	for (k = 0; k < r; k++)
	   m += p->s[k].child + 1;
   }

   return seq_key(bt, nx, m, key, t);
}


