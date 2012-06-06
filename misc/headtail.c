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
#include	<dirent.h>

extern int errno;

main(argc, argv)
int	argc;
char	*argv[];
{
long	l;
int	t, cont;
char	*p;
FILE	*fp;

   if (argc != 4)
   {
	msg("Invalid Parameters");
	exit(1);
   }

   p = argv[1];
   l = atol(argv[2]);
   t = atoi(argv[3]);
   cont = 0;

   for (fp = abrearq("", p, "", 0); fp == NULL && cont < 10; 
	fp = abrearq("", p, "", 0));
   if (fp == NULL)
	exit(2);

   if (t == 0)
   {
	posifim(fp);
	t = ftell(fp) - l;
	posiciona(fp, 0l);
   }

   if (copy_file(fp, l, t, stdout) == ERRO)
	exit(3);

   exit(0);
}


msg(char *x)
{
DIR *dirp;
struct dirent *direntp;

        fprintf(stderr, "\nheadtail: %s %d\n", x, strlen(x));
/*	dirp =  opendir( "." );
	if (dirp == NULL)
	   fprintf(stderr, "\nNULL DIR\n");
        while ( (direntp = readdir( dirp )) != NULL )
	{
	   if (strncmp(direntp->d_name, "Sort-A", 6) == 0)
                    (void)fprintf(stderr, "%s\n", direntp->d_name );
	}
        (void)closedir( dirp );
*/
}


