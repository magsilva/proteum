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
#include <string.h>

char	ret[500];
char	s[500];

main()
{
   while (1)
   {
	gets(s);
	muda_ptar(s);
	printf("Changed: %s\n", s);
	ret[0] = 0;
	ptar_to_str(s, ret);
        printf("Original: %s\n", ret);

   }
}


muda_ptar(s)
char    *s;
{
int     i = 0;

   ret[0] = 0;
   rec_ptar(s, &i);
   for (i = strlen(ret)-1 ; i >= 0; i--)
        *s++ = ret[i];
   *s = '\0';
/*   printf("\n%s", ret); */
}


rec_ptar(char *s, int *i)
{
int n = 0;

   
   while (s[*i] == '*') 
   {
	(*i)++;
	n++;
   }
   rec_dir(s, i);
   while(n-- > 0) 
       strcat(ret, "*");
}

rec_dir(char *s, int *i)
{
char	d[] = {0, 0};

   if (s[*i] == '\0')
	return;
   if (s[*i] == '{')
   {
	(*i)++;
	rec_ptar(s, i);
        (*i)++;
   }
   while (s[*i] == '(' || s[*i] == '[')
   {
	d[0] = s[*i];
	(*i)++;
	strcat(ret, d);
   }
}



ptar_to_str(p, s)
char    *s, *p;
{
char    *x;
static  char temp[200];

   x = &(p[strlen(p)-1]);
   if ( x >= p)
	strcat(s++, " ");
   while (x >= p)
   {
	if (*x ==  '(')
	   strcat(s, "()");
	else
	if (*x ==  '[')
           strcat(s, "[]");
	else
	{
	   sprintf(temp, "(*%s)", s);
	   strcpy(s, temp);
	}
	x--;
   }
}




