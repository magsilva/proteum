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
GERA_REPORT_TCASE:
	Esta funcao cria um arquivo onde eh gravado um relatorio sobre
a situacao atual dos mutantes.

Autor: Delamaro
Data: 23-05-94
**************************************************************************/

gera_report_tcase(DirRel, ArqRel, h, hb)
int	h, hb;
char	DirRel[], ArqRel[];
{
int	*possivel_status[NO_EXEC_COMP+1];
int	ntcase = NTCASE(&HTCase);
int	i, j, k, t, l, b;
int	flag_efetivo, flg_erro;
FILE	*fp;


   for (i = 0; i <= NO_EXEC_COMP; i++)
   {
	possivel_status[i] = (int *) malloc((MAXI(1, ntcase))*sizeof(int));
	if (possivel_status[i] == NULL)
	{
	   msg("Insuficient Memory");
	   r1:
	   for (--i; i >=0 ; i--)
	   	free(possivel_status[i]);
	   return ERRO;
	}
	for (j = 0; j < ntcase; j++)
	   possivel_status[i][j] = 0;
   }

   for (j = 0; j < GERADOS(&HMuta); j++)
   {
	if (carrega_descr(&HMuta, j) == ERRO)
	   goto r1;
	k = get_mutante_status(&HMuta);
	if (k == ANOMALO || k == EQUIV)
	   continue;

	for (k = 1; k <= ntcase; k++)
	{
	   t = ltofis_tcase(&HTCase, k);
	   if  (TAB_FIS(&HTCase)[t].reg_log > REG(&HMuta).ultimo_tcase)
		l = NO_EXEC;
	   else
	   	l = get_bitmap(REG(&HMuta).tcase, t);
	   (possivel_status[l][k-1])++;
	 }
   }

   fp = criarq(DirRel, ArqRel, "");
   if (fp == NULL)
	goto r1;

   report_cabec(fp, h);

   flag_efetivo = hb & 1;	/* flag de "Efective Test Case Only" */
   hb >>= 1;

   for( i = 1; i <= ntcase; i++)
   {
	t = possivel_status[EXEC_MORTO_TRAP][i-1] +
	    possivel_status[EXEC_MORTO_TIMEOUT][i-1] +
	    possivel_status[EXEC_MORTO_RETCOD][i-1] +
	    possivel_status[EXEC_MORTO_SAIDA][i-1];

   	k = ltofis_tcase(&HTCase, i);
/*   	flg_erro = (TAB_FIS(&HTCase)[k].error); */

	if ( t == 0 && flag_efetivo)
	   continue;	/* se foi usado "Efective Test Case Only", nao mostra
				casos de teste que nao mataram mutants */


	fprintf(fp, "\n\nTeste Case # %d", i);
/*
	if (flg_erro)
	{
	   fprintf(fp, "\n\tThis test case have caused an execution error");
	   goto l1;
	} */

	b = hb;

	if (b & 1)
	   fprintf(fp, "\n\tNumber of Not Executed Mutants: %d",
 			possivel_status[NO_EXEC][i-1]);

	b >>= 1;
	if (b & 1)
	   fprintf(fp, "\n\tNumber of Alive Mutants: %d",
 			possivel_status[EXEC_VIVO][i-1]);

	b >>= 1;
	if (b & 1)
	{
	   fprintf(fp, "\n\tNumber of Mutants Dead by Stdout: %d",
 			possivel_status[EXEC_MORTO_SAIDA][i-1]);

	   fprintf(fp, "\n\tNumber of Mutants Dead by Retcode : %d",
 			possivel_status[EXEC_MORTO_RETCOD][i-1]);

	   fprintf(fp, "\n\tNumber of Mutants Dead by Timeout: %d",
 			possivel_status[EXEC_MORTO_TIMEOUT][i-1]);

	   fprintf(fp, "\n\tNumber of Mutants Dead by Trap: %d",
 			possivel_status[EXEC_MORTO_TRAP][i-1]);

	   fprintf(fp, "\n\tNumber of Mutants Avoided: %d",
                        possivel_status[AVOIDED][i-1]);
	}

	b >>= 1;
	if (b & 1)
	   fprintf(fp, "\n\tNumber of Dead Mutants: %d", t);

	b >>= 1;
	if (b & 1)
	{
	    fprintf(fp, "\n\t%s", TAB_FIS(&HTCase)[k].desabili ?
				  "Disabled" : "Enabled");
	}

	if (le_tcase_reg(&HTCase, k) == ERRO)
		continue;

	b >>=1;
	if (b & 1)
	{
	   fprintf(fp, "\n\tCPU Execution Time (1/100 sec.): %d",
						TREG(&HTCase).exetempo);
	   fprintf(fp, "\n\tTotal Execution Time (1/100 sec.): %d",
						TREG(&HTCase).totaltempo);
	   fprintf(fp, "\n\tRetorn Code: %d", TREG(&HTCase).retcode);
           fprintf(fp, "\n\tParameters: %s",
                                (char *) build_param(TREG(&HTCase).param));
	}

	b >>= 1;
	if (b & 1)
	{
	   fprintf(fp, "\n\tInput:\n");
	   copy_file(TFILEIO(&HTCase), TREG(&HTCase).apont_ent,
					TREG(&HTCase).tamanho_ent, fp);
	}

	b >>= 1;
	if (b & 1)
	{
	   fprintf(fp, "\n\tOutput:\n");
	   t = TREG(&HTCase).tamanho_ent;
           copy_file(TFILEIO(&HTCase), TREG(&HTCase).apont_ent+t,
                                        TREG(&HTCase).tamanho_sai, fp);
           fprintf(fp, "\n\tStderr:\n");
           t += TREG(&HTCase).tamanho_sai;
           copy_file(TFILEIO(&HTCase), TREG(&HTCase).apont_ent+t,
                                        TREG(&HTCase).tamanho_err, fp);
	}

	l1:
	fprintf(fp, "\n_________________________________________________\n");

   }

   for (i = 0; i <= NO_EXEC_COMP; i++)
   {
	free(possivel_status[i]);
    }
   fecharq(fp);



}


report_cabec(fp, flags)
FILE	*fp;
int	flags;
{
int	i, j, s;
float	score;

fprintf(fp, "\n[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]" );
fprintf(fp, "\n[]");
fprintf(fp, "\n[]                  PROGRAM TESTE: %s",  pteste_cab.nome);
fprintf(fp, "\n[]----------------------------------------------------------");
fprintf(fp, "\n[]   SOURCE FILE: %s", pteste_cab.fonte);
fprintf(fp, "\n[]");
fprintf(fp, "\n[]   TOTAL MUTANTS: %d", GERADOS(&HMuta));
fprintf(fp, "\n[]");
fprintf(fp, "\n[]   ANOMALOUS MUTANTS: %d", ANOMALOS(&HMuta));
fprintf(fp, "\n[]");
fprintf(fp, "\n[]   ACTIVE MUTANTS: %d",
				GERADOS(&HMuta)-INATIVOS(&HMuta));
fprintf(fp, "\n[]");
fprintf(fp, "\n[]   ALIVE MUTANTS: %d", VIVOS(&HMuta));
fprintf(fp, "\n[]");
fprintf(fp, "\n[]   EQUIVALENT MUTANTS: %d", EQUIVS(&HMuta));

s = GERADOS(&HMuta) - INATIVOS(&HMuta) -
	 ANOMALOS(&HMuta) - EQUIVS(&HMuta);

score = (s == 0 ? 0.0 : (float) (s - VIVOS(&HMuta)) / (float) s);

fprintf(fp, "\n[]");
fprintf(fp, "\n[]   MUTATION SCORE: %f", score);

if (flags & 1)     /* mostra operadores utilizados */
{
   fprintf(fp, "\n[]");
   fprintf(fp, "\n[]   OPERATORS:");
   j = 0;
   for (i = 0; i < NOPERADORES; i++)
   {
	if (TOTAL(&HMuta)[i] == 0)
	   continue;
        if (j == 0)
	{
	   fprintf(fp, "\n[]   ");
	}
	fprintf(fp, "%-15s %5d    ",g_tab_operador[i].nome,
					(int) TOTAL(&HMuta)[i]);
	j = (j + 1) % 3;
   }

}

fprintf(fp, "\n[]");
fprintf(fp, "\n[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]" );
}

