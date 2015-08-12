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

#include "hash.h"
#include <string.h>

/**
 * Get hash (unsigned long) based upon an string and two integers.
 *
 * @param str String be used for hash derivation.
 * @param k1 Number
 * @param k2 Number
 *
 * @author Márcio Eduardo Delamaro
 */
unsigned long fhash(char * str, int k1, int k2)
{
	int	l;
	unsigned long s, t, *p;
	union {
		char str[100];
		unsigned long u;
	} al;

	strncpy(al.str, str, 99);
	al.str[100] = '\0';
	l = strlen(al.str) / sizeof (unsigned long);
	p = (unsigned long *) al.str;
	t = 0;
	s = 1;
	while (l > 0) {
		t = *p;
		t <<= 3;
		t >>= 6;
		t *= k1;
		t *= k2;
		s *= t;
	        p++;
		l--;
	}
	s |= 1;
	return s;
}


