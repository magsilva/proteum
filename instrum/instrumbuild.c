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


/*-----------------------------------------------------------------------------*/
/*   INSTRUMBUILD.C
/*   Functions to compile an instrumented source file.
/*   date: 01/31/2008
/*---------------------------------------------------------------------------*/

#include	<string.h> 
#include 	<sys/stat.h>

#include	"lib/gerais.h"
#include	"tcase/lib/tcase.h"
#include	"li/lib/li.h"
#include	"pteste/lib/pteste.h"

CAB_PTESTE	pteste_cab;	/* estrutura onde carregar arq. teste */

char Filename[NOME_LENGTH+1],        /* nli filename */
     Directory[NOME_LENGTH+1],       /* current directory */
     DirHome[NOME_LENGTH+1],		/* diretorio do proteum */
     buf[3*NOME_LENGTH+1],       /* auxiliar */
     ArqTeste[NOME_LENGTH+1];	/* nome do arquivo de teste */

extern char	bufaux[1024];

void instrum_build(argc, argv)
int     argc;
char    *argv[];
{
int tmp, i;
struct stat	sstat;
char c;

   if (argc < 4) {
      msg ("Missing Parameters");
      exit (1);
   }

   tmp = argc-4;

   strcpy(ArqTeste, argv[argc-1]);
   strcpy (Filename, argv[argc-2]);
   strcpy (Directory, ".");

   for (i=2; i<argc-2; i++) {
      if (strcmp (argv[i], "-D") == 0) {
         if (!tem_dir (argv[i+1])) {
            msg ("Invalid Directory on Parameter -D");
            exit (1);
         } else 
            strcpy (Directory, argv[i+1]);
         argv[i] = argv[i+1] = "";
         tmp-=2;
      }
   }
 
   if (tmp != 0) {
      msg ("Invalid Number of Parameters");
      exit (1);
   }
    
   if (getenv(STR_DIR_HOME) != NULL)
      strcpy(DirHome, getenv(STR_DIR_HOME));
   else
      DirHome[0] = '\0';

/*----------------------------- Carrega arquivo de teste ---------*/
   if (carrega_arquivo_teste(Directory, ArqTeste, &pteste_cab) == ERRO)
	exit(1);

/* substitui o nome do executavel no comando de compilação */

   strcpy(buf,  monta_nome(Directory, Filename, "_inst"));
   sub_str(bufaux, pteste_cab.comp, pteste_cab.exec, buf);

   sprintf(buf, "-I %s %s %s", DirHome, monta_nome(Directory, Filename, SUFIXO_INS),
           monta_nome(Directory, Filename, SUFIXO_PP));
   sub_str(bufaux, bufaux, monta_nome("", pteste_cab.fonte, SUFIXO_FONTE), buf);

   printf("\nExecutando: %s\n", bufaux);
   while (get_str_exec(bufaux, &c, 0) == ERRO );

   if ( !tem_arq(Directory, Filename, "_inst") ||
        stat(monta_nome(Directory, Filename, "_inst"), &sstat) < 0 ||
        ( ! (S_ISREG(sstat.st_mode) && (sstat.st_mode & S_IXUSR)) )
      )
   {
	msg("Fail to Create Instrumented Executable File"); 
	exit(1);
   }

   printf("\nInstrumented executable file: %s\n", monta_nome(Directory, Filename, "_inst"));
}