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
#include	<tcase/lib/tcase.h>
#include	<muta/lib/muta.h>
#include	<pteste/lib/pteste.h>


/*------------------------------------------------------------------------
Variaveis que guardam os dados ligados ao arquivo de mutantes */



extern	HAND_MUTANTE	HMuta;		/* manipulador arq. mutantes */

/*------------------------------------------------------------------------
Variaveis que guardam os dados ligados as arquivo de casos de teste */

extern	HAND_TCASE	HTCase;

extern	CAB_PTESTE	pteste_cab;

extern	OPERADOR_MUTACAO g_tab_operador[];



/***************************************************************************
GERA_REPORT_TRACE:
	Esta funcao cria um arquivo onde eh gravado um relatorio sobre 
a situacao atual dos mutantes.

Autor: Delamaro
Data: 23-05-94
**************************************************************************/

gera_report_trace(DirRel, ArqRel, hb)
int	hb;
char	DirRel[], ArqRel[];
{
int	ntcase = NTCASE(&HTCase);
int	j, k,l, t;
int	cont_exec, cont_no_exec, cont_avoided, cont_kill;
int	s_exec, s_no_exec, s_avoided, s_kill; 
FILE	*fp;


   fp = criarq(DirRel, ArqRel, "");
   if (fp == NULL)
	return;

   fprintf(fp, "Mutant # | Executed | Avoided | Not Executed | Killed | Total");
   fprintf(fp, "\n-------------------------------------------------------------");
   s_exec = s_no_exec = s_avoided = s_kill = 0;

   for (j = 0; j < GERADOS(&HMuta); j++)
   {
	if (carrega_descr(&HMuta, j) == ERRO)
	   goto r1;
   	cont_exec = cont_no_exec = cont_kill = cont_avoided = 0;

	for (k = 1; k <= ntcase; k++)
	{
	   t = ltofis_tcase(&HTCase, k);
	   if  (TAB_FIS(&HTCase)[t].reg_log > REG(&HMuta).ultimo_tcase ||
		 TAB_FIS(&HTCase)[t].desabili)
		l = NO_EXEC;
	   else
	   	l = get_bitmap(REG(&HMuta).tcase, t); 

	   if (l >= EXEC_VIVO && l <= EXEC_MORTO_TRAP)
	   {
		cont_exec++;
		if ( l >= EXEC_MORTO_SAIDA )
		   cont_kill++;
	   }
	   else
	   if (l == AVOIDED)
		cont_avoided++;
	   else
		cont_no_exec++;
	}
	if (hb & 1)
	{
	   fprintf(fp, "\n%-8d | %-8d | %-7d | %-12d | %-6d | %d ", j, 
		cont_exec, cont_avoided, cont_no_exec, cont_kill, ntcase);
	}
	if (hb & 2)
	{
	   fprintf(fp, "\n");
	   for (k = 1; k <= ntcase; k++)
	   {
	      t = ltofis_tcase(&HTCase, k);
	      if  (TAB_FIS(&HTCase)[t].reg_log > REG(&HMuta).ultimo_tcase)
		  l = -1;
	      else
	   	  l = get_bitmap(REG(&HMuta).tcase, t);
	      fprintf(fp, "%d ", l);
	   }
	}
	 
	s_kill += cont_kill;  
	s_exec += cont_exec;
	s_avoided += cont_avoided;
	s_no_exec += cont_no_exec;

   }
   if (hb & 1)
      fprintf(fp, "\n-------------------------------------------------------------");
   fprintf(fp,  "\nTOTAL    | %-8d | %-7d | %-12d | %-6d | %d ", s_exec, 
			s_avoided, s_no_exec, s_kill, ntcase*GERADOS(&HMuta));
   fprintf(fp, "\n");
   r1:
   fclose(fp);
}


/***************************************************************************
GERA_REPORT_GERA:
Essa funcao mostra no sdtout, algumas informacoes gerais necessarias
para a utilizacao da interface grafica.
***************************************************************************/
gera_report_gera()
{
int	s;
float	score;
   printf("\n%d", NTCASE(&HTCase));
   printf("\n%d", GERADOS(&HMuta));
   printf("\n%d", GERADOS(&HMuta)-INATIVOS(&HMuta));
   printf("\n%d", EQUIVS(&HMuta));
   s = GERADOS(&HMuta) - INATIVOS(&HMuta) - 
	 ANOMALOS(&HMuta) - EQUIVS(&HMuta);

   score = (s == 0 ? 0.0 : (float) (s - VIVOS(&HMuta)) / (float) s);
   printf("\n%f", score);
   printf("\n%d", VIVOS(&HMuta));
   printf("\n%d", ANOMALOS(&HMuta));
   for (s = 0; s < NOPERADORES; s++)
	printf("\n%d",	(int) TOTAL(&HMuta)[s]);
}


