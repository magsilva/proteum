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


#include	<lib/gerais.h> /* estrutura de dados + declaracao de variaveis*/
			       /* globais */

#include	<li/lib/li.h>

#include	"pteste.h" /* estruturas de dados + declaracao das variaveis */


/*************************************************************************
CRIA_ARQUIVO_PTESTE:
	Esta funcao cria um novo arquivo de teste.
Parametros:
	dir: diretorio onde deve criar.
	nome: nome do arquivo de teste.
	fonte: arquivo fonte usado no teste.
	exec:     "    executavel usado.
	comp: comando de compilacao.
	tipo: tipo do teste (T_TESTE ou T_RESEARCH)
	anomalias: tipo de anomalias a procurar.
	funcoes: apontador para nome das funcoes.
	cab: cabecalho do teste
*************************************************************************/

cria_arquivo_teste(dir, nome, fonte, exec, comp, tipo, anomalias, funcoes, cab)
char	*dir, *nome, *fonte, *exec, *comp, *funcoes;
int	tipo, anomalias;
CAB_PTESTE	*cab;
{
char	*p, *p1, str[20], *from_buf();
FILE	*fp_pteste;

   if ((fp_pteste = criarq(dir, nome, SUFIXO_PTESTE)) == NULL)
	     return ERRO;

 /*  if (lockarq(fp_pteste) == ERRO)
	     return ERRO; */

   if (grava_ident(fp_pteste, IDENT_PTESTE) == ERRO)
	     return ERRO;

   if ( cab != NULL)
   {
	strcpy(cab->dir, dir);
   	strcpy(cab->nome, nome);
   	strcpy(cab->fonte, fonte);
   	strcpy(cab->exec, exec);
   	strcpy(cab->comp, comp);
   	cab->tipo = tipo;
   	cab->anomalias = anomalias;
	sprintf(cab->fonte_expa, "__%s", fonte);
   }

   p = (tipo == T_TESTE) ? STR_TESTE: STR_RESEARCH;

   str[0] = '\0';
   p1 = str;
   if (anomalias == NONE) 
	     p1 = STR_NONE;
   	else {
   		if (anomalias & UR) 
			     { 
				strcat(str, STR_UR);
				strcat(str, " ");
	      		     }
   		if (anomalias & DU) 
			     { 
				strcat(str, STR_DU);
				strcat(str, " ");
	      		     }
   		if (anomalias & DD) 
			     { 
				strcat(str, STR_DD);
	      		     }
	       }

   if (fprintf(fp_pteste, "%s\n%s\n%s\n%s\n%s\n",
      	 cab->fonte, cab->exec, cab->comp, p, p1) == EOF)
	     goto erro;

   if (cab != NULL)
   {
	cab->funcoes[0] = '\0';
   	p1 = cab->funcoes;
   	cab->todas = (funcoes == NULL);
   }

   if (funcoes == NULL)
	     {
		p1 = STR_ALLFUNC;
		if (fprintf(fp_pteste, "%s\n", p1) == EOF)
			     goto erro;
	     }
   	else {
		inic_from_buf(funcoes);
		if (cab != NULL)
		   inic_to_buf(cab->funcoes, sizeof(cab->funcoes));
		while ((p1 = from_buf()) != NULL)
		  {
		    if (cab != NULL)
			to_buf(p1);
		    if (fprintf(fp_pteste, "%s\n", p1) == EOF)
			     goto erro;
		   }
	      }

   fecharq(fp_pteste);
   return OK;

erro:
   d_msg(nome, SUFIXO_PTESTE, "Error Writing File");
   fecharq(fp_pteste);
   return ERRO;
}



rcria_arquivo_teste(cab)
CAB_PTESTE	*cab;
{
   sprintf(cab->fonte_expa, "__%s", cab->fonte);
   return cria_arquivo_teste(cab->dir, cab->nome, cab->fonte, 
			     cab->exec,cab->comp,
			     cab->tipo, cab->anomalias,
			     cab->todas? NULL: cab->funcoes, NULL);
}





/*************************************************************************
CARREGA_ARQUIVO_PTESTE:
	Esta funcao carrega na memoria os dados de um arquivo de teste.
Parametros:
	dir: diretorio onde esta o arquivo.
	nome: nome do arquivo de teste.
*************************************************************************/
carrega_arquivo_teste(dir, nome, cab)
char	*dir, *nome;
CAB_PTESTE	*cab;
{
FILE	*fp_pteste;
char	str1[30], str2[30];
int	i;

   if ((fp_pteste = abrearq(dir, nome, SUFIXO_PTESTE, 1)) == NULL)
	     return ERRO;

/*   if (lockarq(fp_pteste) == ERRO)
 	goto l1 */

   if (checa_ident(fp_pteste, IDENT_PTESTE) == ERRO)
   {	l1:
	     fecharq(fp_pteste);
	     return ERRO;
   }

   if (posiciona(fp_pteste, OFFSET0) == ERRO)
	     goto l1;

   if ((i = fscanf(fp_pteste,  
	"%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n",
         cab->fonte, cab->exec, cab->comp, str1, str2, cab->funcoes)) != 6)
	     goto erro; 
   strcpy(cab->dir, dir);
   strcpy(cab->nome, nome);

   cab->tipo = (strcmp(str1, STR_TESTE)) ? T_RESEARCH: T_TESTE;

   cab->anomalias = NONE;
   if (str2 != STR_NONE)
	     {
		if (acha_palavra(STR_UR, str2) >= 0)
		        cab->anomalias += UR;
		if (acha_palavra(STR_DU, str2) >= 0)
		        cab->anomalias += DU;
		if (acha_palavra(STR_DD, str2) >= 0)
		        cab->anomalias += DD;
	     }

   cab->todas = ! strcmp(cab->funcoes, STR_ALLFUNC);

   if (! cab->todas)
	     {
		i = strlen(cab->funcoes) + 1;
		while (fscanf(fp_pteste, "%[^\n]\n", &cab->funcoes[i]) == 1)
		   {
			i += strlen(&cab->funcoes[i]) + 1;
			if (i >= sizeof(cab->funcoes))
			        goto erro;
		    }
		cab->funcoes[i] = '\0';
              }
   sprintf(cab->fonte_expa, "__%s", cab->fonte);

   fecharq(fp_pteste);
   return OK;

erro:
   d_msg(nome, SUFIXO_PTESTE, "Invalid Test File");
   fecharq(fp_pteste);
   return ERRO;
}


