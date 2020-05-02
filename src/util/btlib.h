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

#ifndef         __BTLIB__
#define         __BTLIB__

#include "btree.h"
#include <stdio.h>

extern long bufpage[];

int     find_page0(FILE *);
int	write_page(BTREE *, void *);
int     write_page_zero(BTREE *);
int     get_page_zero(BTREE *);
void	*get_root(BTREE *);
int     get_nth(BTREE *, long, int, KEY *, ITEM *);
void    *new_page(BTREE *);
long	new_reg(BTREE *, int);
int     insert_key(BTREE *, long , KEY *, ITEM *, KEY **);
long	address_of(BTREE *, void *);
int	delete_key(BTREE *, long, KEY *); 
int     search_key(BTREE *, long, KEY *, ITEM *);
int	seq_key(BTREE *, long, int , KEY *, int *);

void    reloca_pool(BTREE *, long);
void    *buffer_of(BTREE *, long);
long    address_of(BTREE *, void *);
int     page_of(BTREE *, long);
void    *get_root(BTREE *);
void    *get_page(BTREE *, long);

#endif


