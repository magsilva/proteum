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
#include	<muta/lib/muta.h>
#include	<tcase/lib/tcase.h>

HAND_MUTANTE	HMuta;	/* manipulador do arquivo de mutantes */

HAND_TCASE	HTCase;

char	ArqMuta[NOME_LENGTH+1],		/* nome do arquivo de mutantes */
	DirCorrente[NOME_LENGTH+1]; 	/* dir corrente */

#define		MAXINTEGER	((unsigned) -1 >> 1)

main(argc, argv)
int	argc;
char	*argv[];
{
int	i, n, t, f;
int	Menost, Menosf, percent;

   Menost = Menosf = FALSE;
   f = MAXINTEGER;
   t = 0;

   if (argc < 3)
   {
	msg("Missing parameters");
	exit(1);
   }

  if ( *argv[argc-1] == '-')
   {
	msg("Invalid Parameter");
	exit(1);
   }

   n = argc-2;
   strcpy(ArqMuta, argv[argc-1]);

   strcpy(DirCorrente, ".");
   for (i = 1; i < argc-2; i++)
   {
	if (strcmp(argv[i], "-D") == 0)
	{
		if (chdir(argv[i+1]) < 0)
		{
			msg("Invalid directory on parameter -D");
			exit(1);
            	}
		argv[i] = argv[i+1] = "";
		n-=2;
	}
	else

	if (strcmp(argv[i], "-r") == 0)
	{
		percent = atoi(argv[i+1]);
		if ( percent < 0 || percent > 100 )
		{
			msg("Invalid parameter -r");
			exit(1);
		}
		argv[i] = argv[i+1] = "";
		n-=2;
	}
	else
	  	
	if (strcmp(argv[i], "-f") == 0)
	{
		t = MAXI(atoi(argv[i+1]), t);
		Menost = TRUE;
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
	else

	if (strcmp(argv[i], "-t") == 0)
	{
		f = MINI(atoi(argv[i+1]), f);
		Menosf = TRUE;
		argv[i] = argv[i+1] = "";
		n -= 2;
	}
   }

   if ( n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }
/*------------------------------- Carrega arquivo de teste -------------*/

   if (carrega_arquivo_tcase(&HTCase,DirCorrente, ArqMuta) == ERRO)
	exit(1);

/*------------------------- Carrega arquivo de mutantes --------- */

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqMuta) == ERRO)
	exit(1);

   f = MINI(f, GERADOS(&HMuta)-1);
   check_equiv(&HMuta, &HTCase, t, f, percent);
   fim:		
   descarrega_arquivo_mutante(&HMuta);
   descarrega_arquivo_tcase(&HTCase);
   return 0;
}

int	vet_cont[MAX_TCASE+1];

check_equiv(HAND_MUTANTE *HMuta, HAND_TCASE *HTCase, int f, int t, int percent)
{
int i, j, k, cont, scont, nmuta, n, nequiv;
long last_tcase;

    n = NTCASE(HTCase);
    for (i = f, nmuta = nequiv = 0;  i <= t; i++)
    {
	if (carrega_descr(HMuta, i) == ERRO)
		return ERRO;
	if (get_mutante_inactive(HMuta) )
		continue;
	nmuta++;
	k = get_mutante_status(HMuta);
        if (k != VIVO)
	{
	   if (k == EQUIV) nequiv++;
	   continue;
	}

	last_tcase = REG(HMuta).ultimo_tcase;
	for ( j = 1, cont = 0; j <= n; j++)
	{
	    k = ltofis_tcase(HTCase, j);
	    if ((! TAB_FIS(HTCase)[k].desabili) && 
		last_tcase > TAB_FIS(HTCase)[k].reg_log &&
		get_bitmap(REG(HMuta).tcase, k) == EXEC_VIVO)
		   cont++;
	}
	vet_cont[cont]++;
   }
   if (nmuta == 0)
	return OK;
	
   scont = 0;
   for (i = MAX_TCASE, k = ((percent * nmuta) / 100)-nequiv; i >= 0; i--)
      if ( scont + vet_cont[i] > k)
	break;
      else
	scont += vet_cont[i];

    printf("\n%d equivalent mutants (%d %%)", nequiv+scont, 
					(nequiv+scont)*100/nmuta);
    printf("\n%d marked before, %d marked now\n", nequiv, scont);

    scont = i+1;
    for (i = f, nmuta = 0;  i <= t; i++)
    {
	if (carrega_descr(HMuta, i) == ERRO)
		return ERRO;
	if (get_mutante_inactive(HMuta) )
		continue;
	if (get_mutante_status(HMuta) != VIVO)
	   continue;
	last_tcase = REG(HMuta).ultimo_tcase;
	for ( j = 1, cont = 0; j <= n; j++)
	{
	    k = ltofis_tcase(HTCase, j);
	    if ((! TAB_FIS(HTCase)[k].desabili) && 
		last_tcase > TAB_FIS(HTCase)[k].reg_log &&
		get_bitmap(REG(HMuta).tcase, k) == EXEC_VIVO)
		   cont++;
	}
	if ( cont >= scont)
	{
	   set_mutante_status(HMuta, EQUIV, 1, 0);
	   if (rewrite_mutante(HMuta) == ERRO)
		return ERRO;
	}
    }
    return OK;
}

 
msg(char *x)
{
   fprintf(stderr, "\ncheck-equiv: %s\n", x);
}
