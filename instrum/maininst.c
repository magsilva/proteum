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


/*--------------------------------------------------------------------------*/
/*   MAININST.C
/*   This module "instrumenta" original programs.
/*   date: 08/05/96
/*   last atualization: 08/05/96
/*----------------------------------------------------------------------------*/

    
#include <stdio.h>
#include <string.h>


#include 	<lib/gerais.h>
#include	<tcase/lib/tcase.h>
#include	<opmuta/lib/opmuta.h>

 
FILE *fp_source,                     /* pointer for __<file>.c */
     *fp_li,                         /* pointer for __<file>.nli */
     *fp_instrum;                    /* pointer for __<file>_inst.c */

char Filename[NOME_LENGTH+1],        /* nli filename */
     Directory[NOME_LENGTH+1],       /* current directory */
     LogFileName[NOME_LENGTH+1];


/*----------------------------------------------------------------------------*/
/*   Main Function
/*   date: 08/05/96
/*   last atualization: 08/05/96
/*----------------------------------------------------------------------------*/

main (argc, argv)
int     argc;
char    *argv[];
{
   int  i, 
        tmp;

   if (argc < 2) {
      msg ("Missing Parameters");
      exit (1);
   }

   if ( strcmp(argv[1],"-build") == 0 )
   {
	instrum_build(argc,argv);
	exit(0);
   }

   if (*argv[argc-1] == '-') {
      msg ("Invalid Parameters");
      exit (1);
   }

   tmp = argc-2;

   strcpy (Filename, argv[argc-1]);
   strcpy (Directory, ".");
   strcpy (LogFileName, Filename);

   for (i=1; i<argc-2; i++) {
      if (strcmp (argv[i], "-D") == 0) {
         if (!tem_dir (argv[i+1])) {
            msg ("Invalid Directory on Parameter -D");
            exit (1);
         } else 
            strcpy (Directory, argv[i+1]);
         argv[i] = argv[i+1] = "";
         tmp-=2;
      }
      else
      if (strcmp (argv[i], "-EE") == 0) {
         strcpy (LogFileName, argv[i+1]);
         argv[i] = argv[i+1] = "";
         tmp-=2;
      }
   }
 
   if (tmp != 0) {
      msg ("Invalid Number of Parameters");
      exit (1);
   }

   /* verifies if exists __file.c */
   if (!tem_arq (Directory, Filename, ".c")) {
      fprintf (stderr, "\ninst: Does not exists %s/%s.c\n",Directory, Filename);
      exit (1);
   }

   /* verifies if exists __file.nli*/
   if (!tem_arq (Directory, Filename, SUFIXO_NLI)) {
      fprintf (stderr, "\ninst: Does not exists __%s/%s%s\n", Directory, 
               Filename, SUFIXO_NLI);
      exit (1);
   }

   if (instrumenta (Directory, Filename, LogFileName))
       exit (0);
   else exit (1);
}
  


/*----------------------------------------------------------------------------*/
/*   INSTRUMENTA ()
/*   Instruments the __<file>.c file.
/*   date: 08/05/96
/*   last atualization: 08/26/96
/*----------------------------------------------------------------------------*/

int instrumenta (dir, file, logfile) 
char dir[],
     file[],
     logfile[];
{  
   if ((fp_source = abrearq (dir, file, ".c", 0)) == NULL) {
      return FALSE;  
   }  

   if ((fp_li = abrearq (dir, file, SUFIXO_NLI, 0)) == NULL) {
      fecharq (fp_source);
      return FALSE;  
   } 

   if ((fp_instrum = criarq (dir, file, SUFIXO_INS)) == NULL) {
      msg ("Cannot create new file");
      fecharq (fp_source);
      fecharq (fp_li);
      return FALSE;
   }
  
   /* creates files with ponta_de_prova () */
   create_func_pp (monta_nome(dir, logfile, SUFIXO_LOG), dir, file);

   /* instruments source file */
   getsymbol();
   program(); 

   fprintf(fp_instrum, "\n\n");
   fecharq (fp_source);
   fecharq (fp_li);
   fclose (fp_instrum);

   return TRUE;
}
