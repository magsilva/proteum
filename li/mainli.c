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


#include	<string.h>

#include	<lib/gerais.h>
#include	"lib/li.h"

int	Menosl, debug;

char	DirCorrente[NOME_LENGTH+1];

int	MenosP;

char	ArqPreProcessa[NOME_LENGTH+1];

char	ArqFonte[NOME_LENGTH+1];

char	ArqLi[NOME_LENGTH+1];

int 	*hexpression;

main(argc, argv)
int	argc;
char	*argv[];
{
int	i, j, k, n, t, u;
char	*getenv(), *p, *get_func_loc();

   if (argc < 3)
   {
	msg("Missing parameters");
	exit(1);
   }

   if (*argv[argc-2] == '-' || *argv[argc-1] == '-')
   {
	msg("Invalid Parameter");
	exit(1);
   }

   Menosl = MenosP = FALSE;
   n = argc-3;
   strcpy(DirCorrente, ".");


   for (i = 1; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-l") == 0)
	{
	   Menosl = TRUE;
	   argv[i] = "";
           n--;
	}
	else

	if ( i < argc-3 && strcmp(argv[i], "-debug") == 0)
        {
           debug = atoi(argv[i+1]);
           argv[i] = argv[i+1] = "";
           n-= 2;
        }
        else


	if (i < argc-3 && strcmp(argv[i], "-P") == 0)
	{
	   MenosP = TRUE;
	   strcpy(ArqPreProcessa, argv[i+1]);
	   argv[i] = argv[i+1] = "";
	   n -= 2;
	}
	else

  	if (i < argc-3 && strcmp(argv[i], "-D") == 0)
	{
	   if (chdir(argv[i+1]) < 0)
	   {
		msg("Invalid directory on parameter -D");
		exit(1);
            }
	   argv[i] = argv[i+1] = "";
	   n -= 2;
	}
   }

   if (n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }


   strcpy(ArqFonte, argv[argc-2]);
   strcat(ArqFonte, SUFIXO_FONTE);
   strcat(ArqPreProcessa, SUFIXO_FONTE);
   strcpy(ArqLi, argv[argc-1]);

   p = MenosP ? ArqPreProcessa: NULL;
   if (cria_li(DirCorrente, ArqFonte, p, ArqLi) != ERRO)
	
   if (! Menosl)
   {
	printf("\n\n+------------------------------------------------------");
	printf("\n|%-30.30s %-7s %-11.11s", "FUNCTION", "LOC'S","CONNECTIONS");
	printf("\n+------------------------------------------------------");
   }
  t = u = k = 0;
  while ((p = get_func_loc(&i, &j) )!= NULL)
   {
	if (Menosl)
	{
	   printf("%s\n", p);
	}
	else
	{
	   printf("\n|%-30.30s %-7d %d", p, i, j);
	   printf("\n+------------------------------------------------------");
	   t += i;
	   u+= j;
	}
	k++;
   }

   if (! Menosl)
   {
	printf("\n|TOTAL (%-5d FUNCTIONS)        %-7d %d", k, t, u);
	printf("\n+------------------------------------------------------\n");
   }



fim:
    return 0;
}


msg(x)
char *x;
{
   fprintf(stderr, "\nli: %s\n", x);
}

