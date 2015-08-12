/*
Copyright (C) 2012 -- Marcio Eduardo Delamaro and Jose Carlos Maldonado

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/


/**
 * Set library.
 *
 * @author Márcio Eduardo Delamaro
 * @created 30/09/1994
 */

#ifndef		_PROTEUM_SET_H_
#define		_PROTEUM_SET_H_

#include <stdbool.h>

#define MAXSET 512

/**
 * Definition of the SET. The element store in the set is a string (*p).
 */
typedef	struct {
	short int	n;
	short int	vet[MAXSET];
	short int	free;
	int		sizeofp;
	char		*p;
} OSET;


void set_new(OSET *);
bool set_new_inic(OSET *, char *);
bool set_add(OSET *, char *);
char * set_get(OSET *, int);
int set_inn(OSET *, char *);
bool set_del(OSET *, int);
bool set_dup(OSET *, OSET *);
void set_free(OSET *);
void set_reset(OSET *);
#define	set_card(x)	(x->n)

#endif
