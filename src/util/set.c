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


#include "set.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define INCREMENT 128

/**
 * Create an empty set.
 *
 * @param s Reference to a set to be "created" (initialized with no content).
 */
void
set_new(OSET *s)
{
	s->n = 0;
	s->free = 0;
	s->p = NULL;
	s->sizeofp = 0;
}

/**
 * Create an empty set and initialize it with 'p'.
 *
 * @param s Reference to a set to be "created" (initialized with no content).
 * @param p String to be added to the empty set.
 */
bool
set_new_inic(OSET *s, char *p)
{
	set_new(s);
	if (set_add(s, p) == false) {
		return false;
	}
	return true;
}

/**
 * Add string to the set.
 *
 * @param s Reference to a set.
 * @param data String to be added to the set.
 *
 * @return True if no error was found, False otherwise
 */
bool
set_add(OSET *s, char *data)
{
	int data_size;

	if (data == NULL) {
		return false;
	}

	data_size = strlen(data);

	// If there isn't space availabe in the set, increase the set size.
	if ((s->free + data_size) >= s->sizeofp) {
		char *tmp = realloc(s->p, (s->sizeofp + ((1 + (data_size / INCREMENT)) * INCREMENT)));
		if (tmp == NULL) {
			return false;
		}
		s->p = tmp;
		s->sizeofp += INCREMENT;
	}

	strcpy(&(s->p[s->free]), data);
	s->vet[s->n] = s->free;
	s->free += data_size + 1;
	s->n++;

	return true;
}

/**
 * Get k-th element of the set (considering the insertion ordering.
 *
 * @param s Reference to a set.
 * @param k Element number.
 *
 * @return The k-th element or NULL if not found
 */
char *
set_get(OSET *s, int k)
{
	return (k < s->n) ? &(s->p[s->vet[k]]) : NULL;
}


/**
 * Verify if the element 'data' belongs to the set.
 *
 * @param s Reference to a set.
 * @param data String to be looked for in the set.
 *
 * @return -1 if not found, the element position otherwise.
 */
int
set_inn(OSET *s, char *data)
{
	char *p;
	int i;

	if (data == NULL) {
		return -1;
	}

	for (i = 0; i < s->n; i++) {
		p = set_get(s, i);
		if (strcmp(p, data) == 0) {
			return i;
		}
	}
	return -1; 
}

/**
 * Delete k-th element from the set.
 *
 * @param s Reference to a set.
 * @param k Position of the element to be deleted.
 *
 * @return OK if found and deleted, ERRO otherwise.
 */
bool
set_del(OSET *s, int k)
{
	int i;
	char *p;

	if (k >= s->n) {
		return false;
	}

	s->free = s->vet[k];
	for (i = k + 1; i < s->n; i++) {
		p = set_get(s, i);
		strcpy(&(s->p[s->free]), p);
		s->vet[i - 1] = s->free;
		s->free += strlen(&(s->p[s->free])) + 1;
	}
	s->n--;
	return true;
}


/**
 * Reset set.
 *
 * @param s Reference to a set.
 */
void
set_reset(OSET *p)
{
	if (p->p != NULL) {
		free(p->p);
	}
	set_new(p);
}


/**
 * Erase set.
 *
 * @param s Reference to a set.
 */
void
set_free(OSET *p)
{
	if (p->p != NULL) {
		free(p->p);
	}
	free(p);
}


/**
 * Duplicate set.
 *
 * @param src Reference to source set.
 * @param dest Reference to destination set.
 */
bool
set_dup(OSET *src, OSET *dest)
{
	int i = 0;
	char *data;

	set_new(dest);
	for (i = 0; i < set_card(src); i++) {
		data = set_get(src, i);
		if (set_add(dest, data) == false) {
			return false;
		}
	}
	return true;
}
