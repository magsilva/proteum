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
#include	<btree/btree.h>
 
#include	"opmuta.h"


#define		KS	(2*sizeof(long)+sizeof(int))

struct	tmpcon {
		long	called;
		long	offset;
		int	seq;
		long	calling;
		int	size;
		int	no;
		char	str[TAM_MAX_EXPR];
		};


BTREE	*tmpcon_bt;

char	tmpcon_file[3*NOME_LENGTH];

cmp_tmpcon(void *x, void *y)
{
long *px, *py;
int  *pix, *piy;

   px = (long *) x;
   py = (long *) y;

   if (*px < *py)
	return -1;
   if (*px > *py)
	return 1;
   px++; py++;

   if (*px < *py)
        return -1;
   if (*px > *py)
        return 1;

   px++; py++;
   pix = (int *) px;
   piy = (int *) py;

   if (*pix < *piy)
        return -1;
   if (*pix > *piy)
        return 1;
   return 0;
}

void print_tmpcon()
{
}



tmp_con_create(char *f)
{

   tmpcon_bt = BTREE_create(f, NULL, KS, cmp_tmpcon, print_tmpcon);
   if (tmpcon_bt == NULL)
   {
	msg("Error creating temporary file");
	return ERRO;
   }
   strcpy(tmpcon_file, f);
   return OK;
}


tmp_con_destroy()
{
   BTREE_close(tmpcon_bt);
   unlink(tmpcon_file);
}


tmp_con_add(long called, int seq, long calling, long offset, 
		int size, int no, char *str)
{
struct tmpcon x;
char *p;
int	k;

   x.called = called;
   x.seq = seq;
   x.calling = calling;
   x.offset = offset;
   x.size = size;
   x.no = no;
   strcpy(x.str, str);
   p = x.str + strlen(x.str) +1;
   k = (void *) p - (void *) &x;

   if (BTREE_insert_data(tmpcon_bt, &x, &x, k) < 0)
   {
	msg("Error in temporary file");
	return ERRO;
   }

   return OK;
}


int tmp_con_first(long called, long offset, int seq)
{
int	k;
struct tmpcon x;

   x.called = called;
   x.offset = offset;
   x.seq = seq;
   if (BTREE_seq_key(tmpcon_bt, &x, &k) == ERRO)
   {
	msg("Error in temporary file");
	return ERRO;
   }
   return k;
}

int tmp_con_get_nth(int n, long *called, int *seq, long *calling, long *offset,
		    int *size, int *no, char *str)
{
struct tmpcon x;
int	k;

   if (BTREE_get_nth_data(tmpcon_bt, n, &x, &x, &k) == ERRO)
   {
        if (btree_errno != REG_NUMBER)
	   msg("Error in temporary file");
        return ERRO;
   }
   if (k < 0)
	return ERRO;

   *called = x.called;
   *seq = x.seq;
   *calling  = x.calling;
   *offset = x.offset;
   *size = x.size;
   *no = x.no;
   strcpy(str, x.str);
   return OK;
}

   
