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

#ifndef _PROTEUM_STRING_H
#define _PROTEUM_STRING_H

#include <string.h>
#include <stdbool.h>

char * normaliza(char * str);
bool isempty(char str[]);
int acha_str(char *s1, char *s2);
int acha_palavra(char *s1, char *s2);
void sub_str(char *buf, char *mod, char *pro, char *sub);

#endif
