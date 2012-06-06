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


#include	<curses.h>

#include	<string.h>
#include	<li/lib/li.h>
#include	<lib/gerais.h>
#include	<pteste/lib/pteste.h>
#include	<muta/lib/muta.h>
 


#define		W_ALIVE		1
#define		W_DEAD		2
#define		W_EQUIV		4
#define		W_ANOM		8
#define		W_INACTIVE	16
#define		W_ALL		31

/*------------------------------------------------------------------------
Variaveis que guardam os dados ligados as arquivo de  teste */

CAB_PTESTE	pteste_cab;

HAND_MUTANTE	HMuta;

char	   	ArqTeste[NOME_LENGTH+1],
		DirCorrente[NOME_LENGTH+1],
		ArqFonte[NOME_LENGTH+1];

WINDOW		*status, *orig, *muta, *options, *orig2, *muta2;

int	t= 0;

char	line[4096];


extern	OPERADOR_MUTACAO	g_tab_operador[];

DESCR_MUTANTE	*descr, *fim_descr();

int	what = W_ALL;

FILE	*fp_source;

main(argc, argv)
int	argc;
char	*argv[];
{
int	MenosD;

static char	BufPar[200],
		BufTcase[400];
char	*p, *from_buf();
int	n, i;

   if (argc < 2)
   {
	msg("Missing parameters");
	exit(1);
   }
   if (*argv[argc-1] == '-' )
   { 
	msg("Invalid Parameter");
	exit(1);
   }

   n = argc - 2;
   strcpy(ArqTeste, argv[argc-1]);
   
   MenosD = FALSE;
   strcpy(DirCorrente, ".");

   for (i = 1; i < argc-1; i++)
   {

	if  (strcmp(argv[i], "-D") == 0)
	{
	   if (chdir(argv[i+1]) < 0)
 	   {
		msg("Invalid directory, option -D");
		exit(1);
	   }
	   argv[i] = argv[i+1] = NULL;
	   n -=2;
         }
	else

   	if (strcmp(argv[i], "-f") == 0 )
	{
	   t = atoi(argv[i+1]);
	   argv[i] = argv[i+1] = "";
	   n -=2;
	}
    }



    if (n != 0)
   {
	msg("Invalid parameter");
	exit(1);
   }

/*----------------------------- Carrega arquivo de teste ---------*/
   if (carrega_arquivo_teste(DirCorrente, ArqTeste, &pteste_cab) == ERRO)
	exit(1);

   if (carrega_arquivo_mutante(&HMuta, DirCorrente, ArqTeste) == ERRO)
	exit(1);
   fp_source = abrearq(DirCorrente, pteste_cab.fonte_expa, SUFIXO_FONTE, 0);

   if (fp_source != NULL)
   	proc_muta_view();

   atualiza_mutantes(&HMuta);
   descarrega_arquivo_mutante(&HMuta);
   return 0;
}


proc_muta_view()
{
int op, current;
int	i, j, n;

   t = MAXI(t, 0);
   t = MINI(t, GERADOS(&HMuta)-1);

   if (t < 0)
   {
	msg("No Mutants to See");
	return;
   }

   current = t;
   op = 0;
   ini_windows();

   do
   {

	refresh_screen(current);

	op = wgetch(options);

	switch (op)
	{
	   case 'L':
		what = what ^ W_ALIVE;
		break;
	   case 'D':
		what = what ^ W_DEAD;
		break;
	   case 'E':
		what = what ^ W_EQUIV;
		break;
	   case 'A':
		what = what ^ W_ANOM;
		break;
	   case 'I':
		what = what ^ W_INACTIVE;
		break;
	   case '+':
		current = acha_proximo(current, 1);
		break;

	   case '-':
		current = acha_proximo(current, -1);
		break;

	   case 'n':
		n = getnum("Mutant number: ");
		current = MINI(n, GERADOS(&HMuta)-1);
		current = MAXI(current, 0);
		break;

	   case 'f':
		mostra_muta(current);

	   case 'x':
		break;

	   case 'e':
		set_equivalent(current);
		break;

	   case 'a':
		set_anomalous(current);
		break;

	}

    } while (op != 'x');
    endwin();

}
 

refresh_screen(current)
int	current;
{

   wclear(status); box(status, 0, 0);
   wclear(orig); box(orig, 0, '=');
   wclear(muta); box(muta, 0, '=');
   wclear(orig2); box(orig2, 0, 0);
   wclear(muta2); box(muta2, 0, 0);
   wclear(options); box(options, 0, 0);

   refresh_muta(current);

   refresh_options();
}



refresh_muta(current)
int	current;
{
int	i;
long 	il;
char	*p, *q;


   if (carrega_descr(&HMuta, current) == ERRO)
   {
	show_msg("Invalid mutant");
	return;
    }

    mvwprintw(status, 0, 35, " MUTANT # %d ", current);
 
   i = get_mutante_status(&HMuta);

   switch (i)
   {
	case VIVO:
		p = "Alive";
		break;
	case MORTO:
		p = "Dead";
		break;
	case ANOMALO:
		p = "Anomalous";
		break;
	case EQUIV:
		p = "Equivalent";
		break;
	default:
		p = "?????";
		break;
   }

   if ( get_mutante_inactive(&HMuta))
	q = "Inactive";
   else
	q = "Active";

   mvwprintw(status,1, 1, "Status: %s %s", p , q);
   if (REG(&HMuta).calling >= 0)
   {
      get_descr_func_l(&HMuta, REG(&HMuta).calling, line, NULL,&i, &i, &i, &il, &i);
      mvwprintw(status,1, 40, "Calling: %s", line); 
   }

   i = REG(&HMuta).operador;
   mvwprintw(status, 2, 1, "Operator: %s", g_tab_operador[i].nome);

   if (REG(&HMuta).called >= 0)
   {
      get_descr_func_l(&HMuta, REG(&HMuta).called, line, NULL, &i, &i, &i, &il, &i);
      mvwprintw(status,2, 40, "Called: %s", line); 
   }


   mvwprintw(orig, 0, 30, "***ORIGINAL PROGRAM***");
   mvwprintw(muta, 0, 35, "***MUTANT***");

   descr = &(DESCR(&HMuta));
   if (DESCR(&HMuta).tipo == CONNECT_DESCR)
   {
      read_string_out(line, fp_source, descr->offset, descr->tamanho);

      mvwprintw(orig, 1, 1, "%s", line);

      mvwprintw(muta, 1, 1, "%s", descr->string);
      descr = fim_descr(descr);
   }

   read_string_out(line, fp_source, descr->offset, descr->tamanho);

   mvwprintw(orig2, 1, 1, "%s", line);
   mvwprintw(muta2, 1, 1, "%s", descr->string);


   wrefresh(status);
   wrefresh(orig);
   wrefresh(orig2);
   wrefresh(muta);
   wrefresh(muta2);
}



refresh_options()
{
char	*str_what();

   wclear(options); box(options,0, 0);
   mvwprintw(options, 1, 1, "[+] Next        [-] Previous        [n]umber of a mutant");
   mvwprintw(options, 2, 1, "[a]nomalous     [e]quivalent        e[x]it ");
   mvwprintw(options, 2, 60, str_what());
   mvwprintw(options, 3, 1, "Choose an option ");
   wrefresh(options);
}



show_msg(x)
char	*x;
{
   wclear(options); box(options, 0, 0);
   mvwprintw(options, 1, 1,"%s",  x);
   wrefresh(options);
   wgetch(options);
   refresh_options();
}


getnum(x)
char	*x;
{
int	i;

   wclear(options); box(options, 0, 0);
   mvwprintw(options, 1, 1, "%s", x);
   wrefresh(options);
   wscanw(options, "%d", &i);
   return i;
}


mostra_muta()
{
}


ini_windows()
{
int	nc;
int	in;

   initscr(); cbreak(); noecho();
   if (LINES < 24 || COLS < 78)
   {
	printf("\nScreen must be be at least 24 lines X 78 colunnms. Sorry dude\n");
 	myexit();
   }
   in = (LINES - 8) / 4; 
   status = newwin(4, COLS, 0, 0);
   orig = newwin(in+1, COLS, nc=3, 0);
   orig2 = newwin(in+1, COLS, nc+=in, 0);

   muta = newwin(in+1, COLS, nc+=in, 0);
   muta2 = newwin(in+1, COLS, nc+=in, 0);
   options = newwin(5, COLS, nc+=in, 0);

   if (status == NULL || options == NULL || orig == NULL || muta == NULL)
   {
	printf("\nOps! Error creting windows (What the hell is that?!?!)\n");
	myexit();
   }
}


set_equivalent(i)
int	i;
{
int	j,s;

   j = get_mutante_status(&HMuta);

   if (j == VIVO)
	s = EQUIV;
   else
   if (j == EQUIV)
	s = VIVO;
   else
	return;

   if (set_mutante_status(&HMuta,s, 0, 0) == ERRO)
	return;

   if (rewrite_mutante(&HMuta) == ERRO)
	return;

   grava_cab_indice(&HMuta);
}


set_anomalous(i)
int	i;
{
int	j,s;

   j = get_mutante_status(&HMuta);

   if (j == VIVO)
	s = ANOMALO;
   else
   if (j == ANOMALO)
	s = VIVO;
   else
	return;

   if (set_mutante_status(&HMuta, s, 0, 0) == ERRO)
	return;
   if (rewrite_mutante(&HMuta) == ERRO)
        return;

   grava_cab_indice(&HMuta);
}



read_string_out(buf, fp, off7, len)
char	buf[];
FILE	*fp;
long	off7;
int	len;
{

   if (posiciona(fp, off7-1) == ERRO || learq(fp, buf, len) == ERRO)
	return ERRO;

   buf[len] = '\0';
   return XOK;
}




acha_proximo(cur, i)
int	cur, i;
{
int	j, k, l;

   j = cur + i;
   while (j < GERADOS(&HMuta) && j >= 0)
   {
	if (carrega_descr(&HMuta, j) == ERRO)
	   return ERRO;

	k = get_mutante_status(&HMuta);
	l = get_mutante_inactive(&HMuta);
	if ((( what & W_INACTIVE) || l == 0))
	if (( k == VIVO && (what & W_ALIVE)) ||
	    ( k == MORTO && (what & W_DEAD)) ||
	    (k == EQUIV && (what & W_EQUIV)) ||
	    (k == ANOMALO && (what & W_ANOM)))
		return j;
	j += i;
    }

    return cur;
}


char *str_what()
{
static char	s[5];

   s[0] = ' ';
   if (what & W_ALIVE)
	s[0] = 'L';
   s[1] = ' ';
   if (what & W_DEAD)
	s[1] = 'D';
    s[2] = ' ';
   if (what & W_EQUIV)
	s[2] = 'E';
    s[3] = ' ';
   if (what & W_ANOM)
	s[3] = 'A';
    s[4] = ' ';
   if (what & W_INACTIVE)
        s[4] = 'I';
   s[5] = '\0';
   return s;
}


myexit()
{
   endwin();
   exit(1);
}

/***************************************************************************
ATUALIZA_MUTANTES:
        This functtions goes thru mutants file and counts the number
of live, dead, anomalous, etc mutants.
**************************************************************************/
atualiza_mutantes(HMuta)
HAND_MUTANTE    *HMuta;
{
int     i;

   VIVOS(HMuta) =
   EQUIVS(HMuta) =
   INATIVOS(HMuta) =
   ANOMALOS(HMuta) = 0;

   for (i = 0; i < GERADOS(HMuta); i++)
   {
	if (carrega_descr(HMuta, i) == ERRO)
	   return ERRO;

        if (get_mutante_inactive(HMuta))
           (INATIVOS(HMuta))++;
       else
           switch (get_mutante_status(HMuta))
           {
                case EQUIV:
                   (EQUIVS(HMuta))++;
                   break;

                case ANOMALO:
                   (ANOMALOS(HMuta))++;
                   break;

                case VIVO:
                   (VIVOS(HMuta))++;
                   break;

           }
   }
   if (grava_cab_indice(HMuta) == ERRO)
        return ERRO;

   return XOK;
}

msg(x)
char *x;
{
   fprintf(stderr, "\nmuta-view: %s\n", x);
}

 
