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

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "string.h"

/**
 * Normalize a string (trim whitespaces from beginning and ending).
 */
char *
normaliza(char * str)
{
	char * buffer;
	int i = 0;

	if (str == NULL) {
		return false;
	}

	buffer = malloc((strlen(str) + 1) * sizeof( char));
	while (*str <= ' ' && *str != '\0') {
		str++;
	}
	do {
		buffer[i] = *(str);
		i++;
		str++;
	} while (*str != '\0');

	while (buffer[i] <= ' ' && i >= 0) {
		buffer[i--] = '\0';
	}

	strcpy(str, buffer);
	free(buffer);
	return str;
}


/**
 * Check if a string is empty.
 *
 * @param str String to be verified
 * @return OK if empty, ERRO otherwise.
 */
bool
isempty(char str[])
{
	int i;
	for (i = 0; i < strlen(str); i++) {
		if (! isspace(str[i])) {
			return false;
		}
	}
	return true;
}

/**
 * Find a string within another string.
 *
 * @return -1 if not found, otherwise the position of the substring.
 */
int
acha_str(char *s1, char *s2)
{
	int t1, i;
	t1 = strlen(s1);
	i = 0;
	while (strncmp(s1,s2, t1) && *s2 != '\0') {
		s2++;
		i++;
	}
	if (*s2 == '\0') {
		return -1;
	}
	return i;
}


int	 
acha_palavra(char *s1, char *s2)
{
	int i, t, j;
	i = j = acha_str(s1, s2);
	t = strlen(s1);
	while (j >= 0) {
		if (i == 0 && s2[i+t] <= ' ') {
			return i;
		}
		if (i > 0)  {
			if (s2[i-1] <= ' ' && s2[i+t] <= ' ') {
				return i;
			}
		}
		j = acha_str(s1, &(s2[i+1]));
		i += j + 1;
	}
	return -1;
}


	   
/**************************************************************************
SUB_STR:
	Esta substitui um substring por outr.

Parametros:
	buf: buffer onde deve ser montado novo string.
	mod: string original.
	pro: substring a ser substituido
	sub: substring que substitui o primeiro
Obs: mod e buf podem ser o mesmo string.

***************************************************************************/
void
sub_str(char *buf, char *mod, char *pro, char *sub)
{
	char *buffer;
	int i, t , j, k;

	buffer = malloc((strlen(mod) + 1) * sizeof( char));
	strcpy(buffer, mod);
	t = strlen(pro);
	buf[0] = '\0';
	for (j = i = 0, k = acha_palavra(pro, buffer); k >= 0; k = acha_palavra(pro, &buffer[i])) {
		i = i+k;
		buffer[i] = '\0';
		strcat(buf, &buffer[j]);
		strcat(buf, sub);
		i += t;
		j = i;
	}
	strcat(buf, &buffer[j]);
	free(buffer);
}
