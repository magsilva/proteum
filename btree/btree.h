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


#include	<stdio.h>

#ifndef		__BTREE__

#define		__BTREE__

#define		BTREE_NULL	(long) 0

#define		PAGESIZE	1024

#define		MAXKEYS		50

#define		BTREE_LABEL	0x41762345

#define		DUMMY_CHAR	'd'

#define		NPAGEPOOL	16

typedef	void 	KEY;

/*------------------ Error codes ---------------*/
#define		DATA_WRITE	1 /* writing data file */
#define		DATA_READ	2 /* reading data file */
#define		INTERN_MIST	3 /*intern error */
#define		REG_NUMBER	4 /* invalid reg. number */
#define		NON_KEY		5 /* non existing key */
#define		INV_IND		6 /* invalid index file */
#define		IND_WRITE	7 /* writing index file */
#define		IND_READ	8 /* reading index file */
#define		DUP_KEY		9 /* duplicate key */
#define		MEMORY		10 /* insufficient memory */
#define		DEL_KEY		11 /* key already deleted */

/*------------------- Macros --------------------------*/
#define		BTREE_nkeys(bt)	(bt->totkeys)
#define         nth_key(bt,p,n)         (((void*)(p+1))+(bt->keysize)*(n))


extern int	btree_errno;

typedef struct {
	long		next;
	long		address;
	int		size;
	int		child;
		} ITEM;


typedef struct {
	long		label;
	long		next0;
	int		nkeys;
	int		child0;
	ITEM		s[MAXKEYS];
		} PAGE_ADDRESS;

typedef struct {
	long		label;
	long		root;
	int		totkeys;
		} PAGE_ZERO;

typedef struct {
	FILE		*datafile;
	FILE		*indfile;
	long		pz;
	long		root;
	int		keysize;
	int		realkeysize;
	int		order;
	int		totkeys;
	int		(* compare_key)();
	void		(* print_key)();
	struct {
		long		pageaddress;
		void		*buffer;
		} pool[NPAGEPOOL];
	} BTREE;


BTREE	*BTREE_create(char *dfile, char *ifile, int keysize, 
		     int (*cmp_key)(), void (* print_key)());

void	BTREE_close(BTREE *bt);

BTREE   *BTREE_open(char *dfile, char *ifile, int keysize,
                     int (*cmp_key)(), void (* print_key)());

int	BTREE_insert_key(BTREE *, KEY *, long , int);

int     BTREE_insert_data(BTREE *, KEY *, void *, int);

int	BTREE_get_key(BTREE *, KEY *, long *, int *);

int	BTREE_get_data(BTREE *, KEY *, void *, int *);

void	BTREE_print(BTREE *);

void    BTREE_print_nth(BTREE *);

int     BTREE_get_nth_key(BTREE *, int , KEY *, long *, int *);

int     BTREE_get_nth_data(BTREE *, int, KEY *, void *, int *);

int	BTREE_delete_key(BTREE *, KEY *);

int	BTREE_delete_nth_key(BTREE *, int);

int	BTREE_rewrite_data(BTREE *, KEY *, void *);

int	BTREE_seq_key(BTREE *, KEY *, int *);

#endif

