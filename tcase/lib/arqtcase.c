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

#include	"tcase.h"





/**
 * Create file with empty test set.
 *
 * @param HTCase structure to handle a test case file
 * @param dir Directory where the file should be stored
 * @param nome Name of the file to be created (without the extension)
 */
int
cria_arquivo_tcase(HAND_TCASE * HTCase, char * dir, char * nome)
{
	int i;

	memset(HTCase, 0, sizeof(*HTCase));

	// Create file
	if ((TFILE(HTCase) = criarq(dir, nome, SUFIXO_TCASE)) == NULL || (TFILEIO(HTCase) = criarq(dir, nome, SUFIXO_IO)) == NULL) {
		return ERRO;
	}
	if (grava_ident(TFILE(HTCase), IDENT_TCASE) == ERRO || grava_ident(TFILEIO(HTCase), IDENT_IO) == ERRO) {
		return ERRO;
	}

	
	// Initialize test set
	CONTLOG(HTCase) = 1;	/*numero do contador logico de casos de teste */
	NTCASE(HTCase) = 0;	/* numero de casos de teste presentes */
	for (i = 0; i < MAX_TCASE; i++) {
		SEQLOG(HTCase)[i] = 0;
		TAB_FIS(HTCase)[i].reg_log = 0;
		TAB_FIS(HTCase)[i].ocup = FALSE;
		TAB_FIS(HTCase)[i].desabili = FALSE;
		TAB_FIS(HTCase)[i].error = FALSE;
	}

	// Save test set to file
	if (grava_tcase_cab(HTCase) == ERRO) {
	     return ERRO;
	}

	return OK;
}


/***************************************************************************
CARREGA_ARQUIVO_TCASE:
	Carrega na memoria arquivo de caso de testes.
parametros:
        HTCase: structure to handle a test case file
	dir: diretorio onde arquivo deve estar
	nome: nome do arquivo (sem extensao)
Autor: Delamaro
****************************************************************************/
int
carrega_arquivo_tcase(HAND_TCASE * HTCase, char *dir, char *nome)
{
	memset(HTCase, 0, sizeof(*HTCase));
	/*---------------- cria arquivo E/S e arquivo de casos de teste ----------*/
	if  ((TFILE(HTCase) = abrearq(dir, nome, SUFIXO_TCASE, 1)) == NULL || (TFILEIO(HTCase) = abrearq(dir, nome, SUFIXO_IO, 1)) == NULL) {
		printf("\nCould not open file");
		return ERRO;
	}

	if ( checa_ident(TFILE(HTCase), IDENT_TCASE) == ERRO ||	checa_ident(TFILEIO(HTCase), IDENT_IO) == ERRO ) {
		printf("\nCould not identify file as a test set");
		return ERRO;
	}

	/*------------------ Le cabecalhos dos arquivos --------------------------*/
	if (posiciona(TFILE(HTCase), OFFSET0) == ERRO || posiciona(TFILEIO(HTCase), OFFSET0) == ERRO) {
		printf("\nCould not read header of test set file");
		return ERRO;
	}

	if ( learq(TFILE(HTCase), &(TCAB(HTCase)), sizeof(TCAB(HTCase))) == ERRO) {
		printf("\nCould not read test set file");
		return ERRO;
	}

	return OK;
}


/**
 * Save and close file with test set.
 *
 * @param HTCase Structure with test set.
 */
int descarrega_arquivo_tcase(HAND_TCASE * HTCase)
{
	if (grava_tcase_cab(HTCase) == ERRO || fecharq(TFILE(HTCase)) == ERRO || fecharq(TFILEIO(HTCase)) == ERRO) {
		return ERRO;
	}
	return OK;
}


/**
 * Prepare saving of test set to a file.
 *
 * @param HTCase Structure with test set.
 */
int grava_tcase_cab(HAND_TCASE *HTCase)
{
	if (posiciona(TFILE(HTCase), OFFSET0) == ERRO || gravarq(TFILE(HTCase), &(TCAB(HTCase)), sizeof(TCAB(HTCase))) == ERRO) {
		return ERRO;
	}
	return OK;
}

/*************************************************************************
GRAVA_TCASE_REG:
	Grava um registro no arquivo de casos de teste.
Parametros:
        HTCase: structure to handle a test case file
**************************************************************************/
grava_tcase_reg(HTCase, n)
HAND_TCASE	*HTCase;
int	n;
{

   if (posiciona(TFILE(HTCase), 
		OFFSET0+sizeof(CAB_TCASE)+n*sizeof(REG_TCASE)) == ERRO ||
	gravarq(TFILE(HTCase), &(TREG(HTCase)), sizeof(REG_TCASE)) == ERRO)
	     return ERRO;
   return OK;
}


/**************************************************************************
LE_TCASE_REG:
	Le um registro do arquivo de casos de teste.
Parametro:
        HTCase: structure to handle a test case file
	i: numero do registro fisico a ser lido
Autor: Delamaro
**************************************************************************/
le_tcase_reg(HTCase, i)
HAND_TCASE	*HTCase;
int	i;
{
   if (posiciona(TFILE(HTCase), 
		OFFSET0+sizeof(CAB_TCASE)+i*sizeof(REG_TCASE)) == ERRO
       || learq(TFILE(HTCase), &(TREG(HTCase)), sizeof(REG_TCASE)) == ERRO)
	     {
		return ERRO;
	      }
   return OK;
}




/***********************************************************************
DELETE_TCASE:
	Deleta uma sequencia de casos de teste.
Parametros:
        HTCase: structure to handle a test case file
	i: numero logico do primeiro caso de teste a deletar.
	k: numero logico do ultimo caso de teste a deletar
Autor: Delamaro
*************************************************************************/
delete_tcase(HTCase, i,k)
int	i,k;
HAND_TCASE *HTCase;
{
int	j, t, l;
REG_TCASE	reg;

   k = MINI(k, NTCASE(HTCase));
   i = MAXI(i, 1);

   i--; k--;
   l = NTCASE(HTCase);
   for (j = i; j <= k ; j++)
       {
	  t = SEQLOG(HTCase)[j];
	  if (le_tcase_reg(HTCase, t) == ERRO)
		break;
	  TAB_FIS(HTCase)[t].reg_log = 0;
	  TAB_FIS(HTCase)[t].ocup = FALSE;
	  TAB_FIS(HTCase)[t].desabili = FALSE;
	  TAB_FIS(HTCase)[t].error = FALSE;
	  (NTCASE(HTCase))--;
	}
   while (j < l)
	SEQLOG(HTCase)[i++] = SEQLOG(HTCase)[j++];

   return grava_tcase_cab(HTCase);
}




/**************************************************************************
LTOFIS:
	Retorna o numero do registro fisico correspondente ao numero 
logico de um caso de teste.
Parametro:
        HTCase: structure to handle a test case file
	i: numero logico do caso de teste
**************************************************************************/
ltofis_tcase(HTCase, i)
HAND_TCASE	*HTCase;
int	i;
{
   if (i < 1 || i > NTCASE(HTCase))
	     return ERRO;
   return (int) SEQLOG(HTCase)[i-1];
}




/**************************************************************************
FISTOL_TCASE:
	Retorna o numero logico  correspondente ao numero de um registro
fisico de um caso de teste.
Parametro:
        HTCase: structure to handle a test case file
	i: numero do registro fisico do caso de teste
**************************************************************************/
fistol_tcase(HTCase, k)
HAND_TCASE	*HTCase;
int	k;
{
int	i;

   for (i = 0; i < NTCASE(HTCase); i++)
 	if (SEQLOG(HTCase)[i] == k)
	   break;

   if (i <  NTCASE(HTCase))
	return i+1;
   return -1;
}





/************************************************************************
DISABLE_TCASE:
ENABLE_TCASE:
	Habilita e desabilita caso de teste.
Parametros:
        HTCase: structure to handle a test case file
	Numero do registro fisico do caso de teste.
Autor: Delamaro
************************************************************************/
disable_tcase(HTCase, i)
HAND_TCASE	*HTCase;
int	i;
{
int	j;

   j = TAB_FIS(HTCase)[i].desabili;
   TAB_FIS(HTCase)[i].desabili = TRUE;
   if (grava_tcase_cab(HTCase) == ERRO)
	     {
		TAB_FIS(HTCase)[i].desabili = j;
		return ERRO;
	      }
   return OK;
}

enable_tcase(HTCase, i)
int	i;
HAND_TCASE	*HTCase;
{
int	j;

   j = TAB_FIS(HTCase)[i].desabili;
   TAB_FIS(HTCase)[i].desabili = FALSE;
   if (grava_tcase_cab(HTCase) == ERRO)
	     {
		TAB_FIS(HTCase)[i].desabili = j;
		return ERRO;
	      }
   return OK;
}


/*************************************************************************
DELETE_IO:
	Delete registers from IO file
*************************************************************************/
delete_io(HTCase)
HAND_TCASE      *HTCase;
{
int     i, t;
long total, tt;
FILE *fp;


   fp = criarq(dirarq(TFILE(HTCase)), nomearq(TFILE(HTCase)), SUFIXO_TMP);
   if (fp == NULL)
        return ERRO;
   if (grava_ident(fp, IDENT_IO) == ERRO )
   {
        fecharq(fp);
        return ERRO;
   }

   for (i = 0; i < NTCASE(HTCase); i++)
   {
        t = SEQLOG(HTCase)[i];
        if (le_tcase_reg(HTCase, t) == ERRO)
                break;
        tt = ftell(fp);
        total = TREG(HTCase).tamanho_ent+ TREG(HTCase).tamanho_log +
                TREG(HTCase).tamanho_sai+ TREG(HTCase).tamanho_err;
        copy_file(TFILEIO(HTCase), TREG(HTCase).apont_ent, total, fp );
        TREG(HTCase).apont_ent = tt;
        grava_tcase_reg(HTCase, t);
   }

   fecharq(fp);
   fecharq(TFILEIO(HTCase));
   t = rename(monta_nome(dirarq(TFILE(HTCase)),
                         nomearq(TFILE(HTCase)),SUFIXO_TMP),
              monta_nome(dirarq(TFILE(HTCase)),
                         nomearq(TFILE(HTCase)),SUFIXO_IO));
   TFILEIO(HTCase) = abrearq(dirarq(TFILE(HTCase)),
                         nomearq(TFILE(HTCase)),SUFIXO_IO);
   if (t < 0)
        return ERRO;
}


/*********************************************************************
INSERE_TCASE:
	Insert a teste case. The content of the test case are spread
in 3 files.
*********************************************************************/
insere_tcase(HTCase, use_instrumentation)
HAND_TCASE	*HTCase;
int	use_instrumentation;
{
FILE	*fpi, *fpo, *fpe, *fps, *p;
int	i, ret;
long t;


/*---------------------- Procura um registro livre --------------------*/
   for (i = 0; i < MAX_TCASE; i++)
        if (! TAB_FIS(HTCase)[i].ocup)
                 break;
   if (i >= MAX_TCASE)
         return ERRO;
/* ---------- acerta cabecalho na memoria ----------------------------*/
   TAB_FIS(HTCase)[i].ocup = TRUE;
   TAB_FIS(HTCase)[i].desabili = FALSE;
   TAB_FIS(HTCase)[i].error = (TREG(HTCase).retcode == CODE_ERROR);
   TAB_FIS(HTCase)[i].reg_log = (CONTLOG(HTCase))++;
   SEQLOG(HTCase)[(NTCASE(HTCase))++] = i;
   p = TFILE(HTCase);

   fpi = abrearq(dirarq(p), nomearq(p), SUFIXO_INPUT, 0);
   fpo = abrearq(dirarq(p), nomearq(p), SUFIXO_OUTPUT, 0);
   fpe = abrearq(dirarq(p), nomearq(p), SUFIXO_ERRO, 0);
   if (use_instrumentation)
	fps = abrearq(dirarq(p), nomearq(p), SUFIXO_LOG, 0);
   else
	fps = NULL;
   if (fpi == NULL || fpo == NULL || fpe == NULL)
   {
	ret = ERRO;
	goto fim;
   }
   posifim(TFILEIO(HTCase));
   TREG(HTCase).apont_ent = ftell(TFILEIO(HTCase));
   posifim(fpi);
   TREG(HTCase).tamanho_ent = ftell(fpi);
   posifim(fpo);
   TREG(HTCase).tamanho_sai = ftell(fpo);
   posifim(fpe);
   TREG(HTCase).tamanho_err = ftell(fpe);

   if (	copy_file(fpi, 0, TREG(HTCase).tamanho_ent,TFILEIO(HTCase)) == ERRO ||
	copy_file(fpo, 0, TREG(HTCase).tamanho_sai,TFILEIO(HTCase)) == ERRO ||
	copy_file(fpe, 0, TREG(HTCase).tamanho_err,TFILEIO(HTCase)) == ERRO )
   {
	ret = ERRO;
	goto fim;
   }

   if (fps)
   {
	t = ftell(TFILEIO(HTCase));
	release_log(HTCase->log);
	if ( (HTCase->log = get_log_from_file(fps)) == NULL || 
		save_log(HTCase->log, TFILEIO(HTCase)) == ERRO)
	{
	   ret = ERRO;
	   goto fim;
	}
	TREG(HTCase).tamanho_log = ftell(TFILEIO(HTCase)) - t;
   }
   else
	TREG(HTCase).tamanho_log = 0;
	

   if ( grava_tcase_reg(HTCase, i) == ERRO || 
	grava_tcase_cab(HTCase) == ERRO)
   {
        TAB_FIS(HTCase)[i].ocup = FALSE;
        (NTCASE(HTCase))--;
        (CONTLOG(HTCase))--;
        ret = ERRO;
	goto fim;
   }
   ret = OK;

fim:
    fecharq(fpi); fecharq(fpo); fecharq(fpe); fecharq(fps);
    delearq(dirarq(p), nomearq(p), SUFIXO_INPUT);
    delearq(dirarq(p), nomearq(p), SUFIXO_OUTPUT);
    delearq(dirarq(p), nomearq(p), SUFIXO_ERRO); 
    delearq(dirarq(p), nomearq(p), SUFIXO_LOG);
    return ret;
}

 
/*************************************************************************
TCASE_GET_LOG: This function loads the current register's execution log

Parameters:
	HTCase: structure used to handle test case files
	v1, v2: functions for which the log should be loaded
*************************************************************************/
tcase_get_log(HTCase, v1, v2)
HAND_TCASE	*HTCase;
long	v1,v2;
{
int	k;
long l;

    if (HTCase->log != NULL)
	release_log(HTCase->log);

    HTCase->log = NULL;
    if ((k = TREG(HTCase).tamanho_log ) <= 0)
	return OK;
    l = TREG(HTCase).apont_ent + TREG(HTCase).tamanho_ent +
	TREG(HTCase).tamanho_sai + TREG(HTCase).tamanho_err;

    if (posiciona(TFILEIO(HTCase), l) == ERRO)
	return ERRO;
    HTCase->log = new_log();
    if (HTCase->log == NULL)
	return ERRO;

   return load_log(HTCase->log, TFILEIO(HTCase), v1, v2);
}
