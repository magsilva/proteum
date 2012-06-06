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


#include        <lib/gerais.h>
#include        <li/lib/li.h>
#include        <muta/lib/muta.h>
#include        <tcase/lib/tcase.h>
#include        <pteste/lib/pteste.h>

/*------------------------------------------------------------------------
Variaveis que guardam os dados ligados ao arquivo de mutantes e tcase*/

extern  HAND_MUTANTE    HMuta;  /* manipulador do arquivo de mutantes */



/* -------------------------------------------------------------------
variaveis do arquivo de teste (.PTM) */

extern  CAB_PTESTE      pteste_cab;


extern  char    DirHome[];      /* diretorio onde estah "proteum.h" */


DESCR_MUTANTE   *fim_descr();



FILE	*fp_m, *fp_f, *fp_o;

static char aux[2*NOME_LENGTH], buf[1024], 
	    buf_dcl1[FUNC_NAME+1], buf_dcl2[1024];

static char param[FUNC_NAME+1],
	    naome[FUNC_NAME+1];

/************************************************************************
MONTA_MUTANTE:
        A partir de um descritor de mutante, constroi o arquivo fonte
Parametros:


**************************************************************************/
char    *monta_mutante(HMuta, pteste_cab, i)
int     i;
CAB_PTESTE      *pteste_cab;
HAND_MUTANTE    *HMuta;
{
static  char    aux[2*NOME_LENGTH];
static  char buf[1024];
long    t, nbytes;
DWORD   offset;
DESCR_MUTANTE   *descr;

   if (carrega_descr(HMuta, i) == ERRO)
           return NULL;

   sprintf(aux, "muta%d_%s", i, pteste_cab->nome);

   fp_m = criarq(pteste_cab->dir, aux, SUFIXO_FONTE);
   if (fp_m == NULL)
             return NULL;

   fp_f = abrearq(pteste_cab->dir, pteste_cab->fonte_expa, SUFIXO_FONTE, 0);
   if (fp_f == NULL)
             {
                fecharq(fp_m);
                return NULL;
              }
   descr = &(DESCR(HMuta));
   fprintf(fp_m, "\n#include    \"%s\"\n",
                                monta_nome(DirHome, "proteum.h", ""));

   t = 0;

   while  (REG(HMuta).tamanho > 0 && descr->offset != 0)
   {
        offset = descr->offset - 1;
        while (t < offset)
           {
                nbytes = (offset - t > 1024) ? 1024: offset - t;
                if (learq(fp_f, buf, nbytes) == ERRO)
                        {
                           fecharq(fp_f);
                           fecharq(fp_m);
                           return NULL;
                        }
                if (gravarq(fp_m, buf, nbytes) == ERRO)
                        {
                           fecharq(fp_f);
                           fecharq(fp_m);
                           return NULL;
                        }
                t += nbytes;
            }
        fseek(fp_f, (long) descr->tamanho, SEEK_CUR);  /* avanca ponteiro no fonte */
        t += descr->tamanho;

        fprintf(fp_m, " %s ", descr->string); /* acrescenta string no mutante */
        descr =  fim_descr(descr);

   }

   for (t = fread(buf, 1, 1024, fp_f); t > 0; t = fread(buf, 1, 1024, fp_f))
        gravarq(fp_m, buf, t);
   fecharq(fp_f);
   fecharq(fp_m);

   return aux;
}



/*****************************************************************************
MONTA_N_MUTANTE:
        Monta um arquivo fonte para varios mutantes simultaneamente
Parametros:
        n:      numero de mutantes
        vet     vetor com o numero dos mutantes a serem executados
****************************************************************************/

char *monta_n_mutante(HMuta, pteste_cab, n, vet)
int     n, vet[];
HAND_MUTANTE    *HMuta;
CAB_PTESTE      *pteste_cab;
{
long    t, nbytes, ini_func, tam_func;
DWORD   offset;
int     ini_nome, tam_dcl, tam_cab, e_void, ttam;
DESCR_MUTANTE   *descr1, *descr;
int     i;

   if (carrega_descr(HMuta, vet[0]) == ERRO)
           return NULL;

   sprintf(aux, "muta%d_%s", vet[0], pteste_cab->nome);

   fp_m = criarq(pteste_cab->dir, aux, SUFIXO_FONTE);
   if (fp_m == NULL)
             return NULL;

   fp_f = abrearq(pteste_cab->dir, pteste_cab->fonte_expa, SUFIXO_FONTE, 0);
   if (fp_f == NULL)
             {
                fecharq(fp_m);
                return NULL;
              }

   descr = &(DESCR(HMuta));
   fprintf(fp_m, "\n#include    \"%s\"\n",
                                monta_nome(DirHome, "proteum.h", ""));
   if (descr->tipo == CONNECT_DESCR)
   {
	ini_func = REG(HMuta).called; /* pega deslocamento de onde comeca
                                           o corpo da funcao */
	descr1 = descr;
	descr = fim_descr(descr);
   }
   else
   {
	descr1 = NULL;
      	ini_func = REG(HMuta).calling; /* pega deslocamento de onde comeca
                                           o corpo da funcao */
   }

   if (get_descr_func_l(HMuta, ini_func, naome, param, 
			&ini_nome, &tam_dcl, &tam_cab, &tam_func, &e_void)
        == ERRO)
   {
        msg("Invalid function description");
	erro:
	fecharq(fp_f); fecharq(fp_m);
        return NULL;
   }

   t = 0;
   if (descr1 != NULL && REG(HMuta).calling < REG(HMuta).called)
   {
	copy_file(fp_f, 0, descr1->offset-1, fp_m);
	fprintf(fp_m, "%s", descr1->string);
	t = descr1->tamanho + descr1->offset-1;
   }


   ini_func -= ini_nome+1;

   copy_file(fp_f, t, ini_func-t, fp_m);


   if (learq(fp_f, buf_dcl1, ini_nome) == ERRO ||
        learq(fp_f, buf_dcl2, tam_dcl-ini_nome) == ERRO)
        goto erro;


   buf_dcl1[ini_nome] = '\0';
   buf_dcl2[tam_dcl-ini_nome] = '\0';


   for (i = 0; i < n; i++)
   {
        if (carrega_descr(HMuta, vet[i]) == ERRO)
           goto erro;

        t = ini_func+tam_dcl;
        fseek(fp_f, ini_func+tam_dcl, SEEK_SET);
        descr = &(DESCR(HMuta));
   	if (descr->tipo == CONNECT_DESCR)
	   descr = fim_descr(descr);

        fprintf(fp_m, "\n%s __%d%s\n", buf_dcl1, vet[i], buf_dcl2);
	ttam = 0;
        while (REG(HMuta).tamanho > 0 && descr->offset != 0)
        {
            offset = descr->offset -1;
	    copy_file(fp_f, t, offset-t, fp_m);
            fseek(fp_f, (long) descr->tamanho, SEEK_CUR);
	    if (offset > t)
		t = offset + descr->tamanho;
	    else
                t += descr->tamanho;
            fprintf(fp_m, " %s ",descr->string);/*acrescenta string no mutante*/
            descr =  fim_descr(descr);
        }
	copy_file(fp_f, t, ini_func + tam_func - t, fp_m);

   }

   fprintf(fp_m, "\n%s %s", buf_dcl1, buf_dcl2);

   copy_file(fp_f, ini_func+tam_dcl, tam_cab, fp_m);

   fprintf(fp_m, "\n{\nchar *getenv();");
   if (! e_void)
   {
	if (strlen(buf_dcl1) == 0)
           fprintf(fp_m, "\n%s __LOCAL_VAR__;", "int");
	else
	   fprintf(fp_m, "\n%s __LOCAL_VAR__;", buf_dcl1);
   }
   fprintf(fp_m, "\nstatic int __LOCAL_COUNTER__ = 0;\n\n");
   fprintf(fp_m, "\n\tIF_MUTA2(__LOCAL_COUNTER__++;, ;)");
   fprintf(fp_m, "\n\tswitch (_NUMERO_MUTANTE)\n\t{");

   for (i = 0; i < n; i++)
   {
        fprintf(fp_m, "\n\tcase %d:", vet[i]);
        if (! e_void)
	{
           fprintf(fp_m, "\n\t\t__LOCAL_VAR__ =  __%d%s%s;", vet[i], 
							 naome, param);
	   fprintf(fp_m, "\n\t\tbreak;\n");
	}
        else
           fprintf(fp_m, "\n\t\t __%d%s%s;\n\t\t break;\n", vet[i], 
							naome, param);
   }

   fprintf(fp_m, "\n\t}");
   fprintf(fp_m, "\n\tIF_MUTA2(__LOCAL_COUNTER__--;, ;)");
   fprintf(fp_m, "\n\tif (__LOCAL_COUNTER__ == 0) UNPREPARE_MUTA;");
   if (! e_void)
   {
	fprintf(fp_m, "\n\treturn __LOCAL_VAR__;");
   }
   fprintf(fp_m, "\n}");

   t = ini_func+tam_func;
   if (descr1 != NULL && REG(HMuta).calling > REG(HMuta).called)
   {
        copy_file(fp_f, t, descr1->offset-1-t, fp_m);
        fprintf(fp_m, "%s", descr1->string);
        t = descr1->tamanho + descr1->offset-1;
   }


   copy_file_to_end(fp_f, t, fp_m);
   fecharq(fp_f);
   fecharq(fp_m);

   return aux;
}


/*****************************************************************************
MONTA_VIEW_MUTANTE:
        Monta um arquivo fonte para visualizacao com um unico mutante
        n:      numero do mutante
****************************************************************************/

char *monta_view_mutante(HMuta, pteste_cab, n)
int     n;
HAND_MUTANTE    *HMuta;
CAB_PTESTE      *pteste_cab;
{
long    t, nbytes, ini_func, tam_func;
DWORD   offset;
int     ini_nome, tam_dcl, tam_cab, e_void;
DESCR_MUTANTE   *descr1, *descr;
int     i;

   if (carrega_descr(HMuta, n) == ERRO)
           return NULL;

   sprintf(aux, "muta%d_%s", n, pteste_cab->nome);

   fp_m = criarq(pteste_cab->dir, aux, SUFIXO_FONTE);
   fp_o = criarq(pteste_cab->dir, aux, "");

   if (fp_m == NULL || fp_o == NULL)
   	goto r1;

   fp_f = abrearq(pteste_cab->dir, pteste_cab->fonte_expa, SUFIXO_FONTE, 0);
   if (fp_f == NULL)
   {
	r1:
    	fecharq(fp_m);
	fecharq(fp_o);
        return NULL;
   }

   descr = &(DESCR(HMuta));
   if (descr->tipo == CONNECT_DESCR)	/* Group I */
   {
   descr1 = descr;
   descr = fim_descr(descr);
   ini_func = REG(HMuta).called;

   if (get_descr_func_l(HMuta, ini_func, naome, param, 
			&ini_nome, &tam_dcl, &tam_cab, &tam_func, &e_void)
        == ERRO)
   {
        msg("Invalid function description");
	goto erro;
   }

   ini_func -= ini_nome + 1;

   copy_file(fp_f, ini_func, descr->offset - 1 - ini_func, fp_o);
   copy_file(fp_f, ini_func, descr->offset- 1 - ini_func, fp_m);

   fprintf(fp_m, "@\n%s", descr->string);
   fprintf(fp_o, "@\n");
   copy_file(fp_f, descr->offset-1, descr->tamanho, fp_o)           ;
   fprintf(fp_m, "@\n");
   fprintf(fp_o, "@\n");


   copy_file(fp_f, descr->offset+descr->tamanho-1, 
		ini_func + tam_func - descr->offset - descr->tamanho + 1, fp_o);
   copy_file(fp_f, descr->offset+descr->tamanho-1,
		ini_func + tam_func - descr->offset - descr->tamanho + 1, fp_m);
   descr = descr1;

   fprintf(fp_m, "\n\n\n");
   fprintf(fp_o, "\n\n\n");


   ini_func = REG(HMuta).calling;
   if (get_descr_func_l(HMuta, ini_func, naome, param, 
			&ini_nome, &tam_dcl, &tam_cab, &tam_func, &e_void)
        == ERRO)
   {
        msg("Invalid function description");
	erro:
	fecharq(fp_f); fecharq(fp_m);
        return NULL;
   }

   ini_func -= ini_nome + 1;
   copy_file(fp_f, ini_func, descr->offset - 1 - ini_func, fp_o);
   copy_file(fp_f, ini_func, descr->offset- 1 - ini_func, fp_m);

   fprintf(fp_m, "@\n%s", descr->string);
   fprintf(fp_o, "@\n");
   copy_file(fp_f, descr->offset-1, descr->tamanho, fp_o);
   fprintf(fp_m, "@\n");
   fprintf(fp_o, "@\n");

   copy_file(fp_f, descr->offset+descr->tamanho-1,
		ini_func + tam_func - descr->offset - descr->tamanho + 1, fp_o);
   copy_file(fp_f, descr->offset+descr->tamanho-1,
		ini_func + tam_func - descr->offset - descr->tamanho + 1, fp_m);

   }
   else
   {
   ini_func = REG(HMuta).calling;
   if (get_descr_func_l(HMuta, ini_func, naome, param,
			&ini_nome, &tam_dcl, &tam_cab, &tam_func, &e_void)
        == ERRO)
   {
        msg("Invalid function description");
	fecharq(fp_f); fecharq(fp_m);
        return NULL;
   }

   ini_func -= ini_nome + 1;
   tam_func += ini_func;
   while (descr->offset > 0)
   {
        copy_file(fp_f, ini_func, descr->offset - 1 - ini_func, fp_o);
        copy_file(fp_f, ini_func, descr->offset - 1 -ini_func, fp_m);

        fprintf(fp_m, "@\n%s", descr->string);
        fprintf(fp_o, "@\n");
        copy_file(fp_f, descr->offset-1, descr->tamanho, fp_o);
        fprintf(fp_m, "@\n");
        fprintf(fp_o, "@\n");
        ini_func = descr->offset - 1 + descr->tamanho;
        descr = fim_descr(descr);
   }
   copy_file(fp_f, ini_func, tam_func - ini_func, fp_o);
   copy_file(fp_f, ini_func, tam_func - ini_func, fp_m);
   }


   fim:
   fecharq(fp_f);
   fecharq(fp_m);
   fecharq(fp_o);

   return aux;
}


