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
#include	<btree/btree.h>
 
#include	"muta.h"

extern  int     cmp_muta();
extern  void    print_muta();

extern  int     cmp_func();
extern  void    print_func();



/**************************************************************************
CRIA_ARQUIVO_MUTANTE:
	Cria arquivo de mutantes, vazio.
parametros:
	dir: diretorio onde arquivo deve estar
	nome: nome do arquivo (sem extensao)
	h: endereco da estrutura onde devem ser armazenados os dados sobre
	   o arquivo de mutantes criado
Autor: Delamaro
Alteracao:

Data: 06-10-94
	introduzir parametro h
****************************************************************************/
cria_arquivo_mutante(h, dir, nome)
char	*dir, *nome;
HAND_MUTANTE	*h;
{
int	i;

/*---------------- cria arquivo de indice e de descritores ----------*/
   if  ((h->fp_indice = criarq(dir, nome, SUFIXO_INDICE)) == NULL )
	     return ERRO;

   if (grava_ident(h->fp_indice, IDENT_INDICE) == ERRO )
	     return ERRO;
 
   delearq(dir, nome, SUFIXO_MUTANTE);

/*---------------- Inicializa cabecalho do arq. de indices --------*/
   h->indice_cab.gerado = 0;	/* numero de mutantes gerados */
   h->indice_cab.executado = 0;	/* numero de mutantes executados */
   h->indice_cab.vivo = 0;		/* numero de mutantes vivos */
   h->indice_cab.equiv = 0;	/* numero de mutantes equivalentess */
   h->indice_cab.anomalo = 0;	/* numero de mutantes anomalos */
   memset(h->indice_cab.total, 0, sizeof(h->indice_cab.total));
   memset(h->indice_cab.totativo, 0, sizeof(h->indice_cab.totativo));

/*------------------ Grava o cabecalho no arquivo -------------------*/
  if (grava_cab_indice(h) == ERRO)
	     return ERRO;

  if ((h->bt = BTREE_create(monta_nome(dir, nome, SUFIXO_MUTANTE),
		   monta_nome(dir, nome, SUFIXO_INDICE), 
		   CHAVE_SIZE , cmp_muta, print_muta)) < 0)
   {
	msg("Error creating mutatnts database");
	return ERRO;
   }

   delearq(dir, nome, SUFIXO_FUNTABLE);
   if ((h->btfun = BTREE_create(monta_nome(dir, nome, SUFIXO_FUNTABLE),
		   NULL, FUNC_KEY_SIZE, cmp_func, print_func)) < 0)
   {
       msg("Error creating function table");
       return ERRO;
   }

  return OK;
}


/***************************************************************************
CARREGA_ARQUIVO_MUTANTE:
	Carrega na memoria arquivo de mutantes.
parametros:
	dir: diretorio onde arquivo deve estar
	nome: nome do arquivo (sem extensao)
	h: endereco da estrutura onde devem ser armazenados os dados sobre
	   o arquivo de mutantes criado

Autor: Delamaro
Alteracao:

Data: 06-10-94
	introduzir parametro h


****************************************************************************/
carrega_arquivo_mutante(h, dir, nome)
char	*dir, *nome;
HAND_MUTANTE	*h;
{

/*---------------- abre arquivo descritores e arquivo de indices ----------*/
   if  ((h->fp_indice = abrearq(dir, nome, SUFIXO_INDICE, 1)) == NULL )
	     return ERRO;

   if (checa_ident(h->fp_indice, IDENT_INDICE) == ERRO )
	     return ERRO;

/*------------------ Le cabecalhos dos arquivos --------------------------*/

   if ( posiciona(h->fp_indice, OFFSET0) == ERRO)
	     return ERRO;

   if ( learq(h->fp_indice, &(h->indice_cab), sizeof(h->indice_cab)) == ERRO)
	     return ERRO;

   if ((h->bt = BTREE_open(monta_nome(dir, nome, SUFIXO_MUTANTE),
                   monta_nome(dir, nome, SUFIXO_INDICE),
                   CHAVE_SIZE , cmp_muta, print_muta)) == NULL)
   {
        msg("Error opening mutatnts database");
        return ERRO;
   }

   if ((h->btfun = BTREE_open(monta_nome(dir, nome, SUFIXO_FUNTABLE),
		   NULL, FUNC_KEY_SIZE, cmp_func, print_func)) == NULL)
   {
       msg("Error opening function table");
       return ERRO;
   }

   return OK;
}



/***************************************************************************
DESCARREGA_MUTANTE:
	Termina a manipulacao de um arquivo de mutantes, fechando-o.

Parametros:
	h: manipulador do arquivo

***************************************************************************/
descarrega_arquivo_mutante(h)
HAND_MUTANTE	*h;
{

/*------------------ Grava o cabecalho no arquivo -------------------*/
  if (grava_cab_indice(h) == ERRO)
	     return ERRO;
  fecharq(h->fp_indice);
  BTREE_close(h->bt);
  BTREE_close(h->btfun);
  return OK;
}


/**************************************************************************
CARREGA_DESCR:
	Carrega  descritor do mutante
Parametros:
	h: manipulador do arquivo
	n: numero do mutante a carregar
***************************************************************************/
carrega_descr(h, i)
HAND_MUTANTE	*h;
int	i;
{
int	k;

   i++;
   if (BTREE_get_nth_data(h->bt, i, &(CHAVE(h)), &(REG(h)), &k) < 0)
   {
	msg("Error in mutant database");
	return ERRO;
   }
   return OK;
}

/**************************************************************************
CARREGA_DESCR_KEY:
	Carrega  descritor do mutante usando uma chave
Parametros:
	h: manipulador do arquivo
	called, calling, oper, seq1, seq2: chave
***************************************************************************/
carrega_descr_key(h, calling, called, oper, seq1, seq2)
HAND_MUTANTE	*h;
long called, calling;
int oper, seq1, seq2;
{
int	k;
REG_MUTANTE x;

   x.called = called;
   x.calling = calling;
   x.operador = oper;
   x.seq1 = seq1;
   x.seq2 = seq2;
   if (BTREE_get_data(h->bt, &x, &(REG(h)), &k) < 0)
   {
	msg("Error in mutant database");
	return ERRO;
   }
   return OK;
}



/**************************************************************************
GRAVA_CAB_INDICE:
	Grava cabecalho do arquivo de indices
Parametros:
	fp: file pointer do arquivo onde gravar.
	cab: cabecalho a ser gravado.
*************************************************************************/
grava_cab_indice(h)
HAND_MUTANTE	*h;
{
   if ( posiciona(h->fp_indice, OFFSET0) == ERRO ||
	gravarq(h->fp_indice, &(h->indice_cab), sizeof(h->indice_cab)) == ERRO)
	     return ERRO;

   return OK;
}


/************************************************************************
INS_MUTANTE:
	Inclui um mutante, atualizando arquivo de indices e de 
descritores.
Parametros:
	h: manipulador do arquivo
	op: numero do operador de mutacao.
	desc: descritor do mutante.
	t: tamanho do descritor.
Autor: Delamaro

Alteracoes:

Data:	06-10-94
	Incluir parametro h
*************************************************************************/
ins_mutante(h, op, calling, called, seq1, seq2, desc, t)
int	op, t, seq1, seq2;
long	calling, called;
DESCR_MUTANTE	*desc;
HAND_MUTANTE	*h;
{
DWORD	ap;

   h->mutante_reg.tamanho = t;	/* tamanho do descritor */
   h->mutante_reg.status1 = 0;
   h->mutante_reg.status2 = 0;
   h->mutante_reg.ultimo_tcase = 0;
   h->mutante_reg.operador = op;
   h->mutante_reg.calling = calling;
   h->mutante_reg.called = called;
   h->mutante_reg.seq1 = seq1;
   h->mutante_reg.seq2 = seq2;
   h->mutante_reg.status = VIVO;
   h->mutante_reg.inativo = 0;
   memcpy(&(DESCR(h)), desc, t);
   memset(h->mutante_reg.tcase, 0, sizeof(h->mutante_reg.tcase));

   if (BTREE_insert_data(h->bt, &(CHAVE(h)), &(REG(h)), t+sizeof(REG(h))) < 0)
   {
	if (btree_errno != DUP_KEY)
	{
	   msg("Error including mutant in data base");
	   return ERRO;
	}
	   return OK;
   }

   h->indice_cab.gerado++;		/* incrementa numero de mutantes */
   (h->indice_cab.total[op])++;	/* incrementa numero de mutantes do operador */

  return grava_cab_indice(h);
}


int	rewrite_mutante(h)
HAND_MUTANTE	*h;
{
   if (BTREE_rewrite_data(h->bt, &(CHAVE(h)), &(REG(h)) ) < 0)
   {
	msg("Error writing mutant to database");
	return ERRO;
   }
}







/**************************************************************************
SET_MUTANTE_STATUS:
	Altera o status de um mutante.
Parametros:
	h: manipulador do arquivo
	s: status que deve assumir
	status1, status2: status auxiliares
**************************************************************************/
set_mutante_status(h, s, s1, s2)
int	s, s1, s2;
HAND_MUTANTE	*h;
{
int	i;

   REG(h).status = s;
   REG(h).status1 = s1;
   REG(h).status2 = s2;
   return OK;
}

/**************************************************************************
GET_MUTANTE_STATUS:
	Obtem o status de um mutante.
Parametros:
	h: manipulador do arquivo
	m: numero do mutante.
**************************************************************************/
get_mutante_status(h)
HAND_MUTANTE	*h;
{

   return REG(h).status;
}



/**************************************************************************
SET_MUTANTE_INACTIVE:
	Altera o status de ativo/inativo de um mutante.
Parametros:
	h: manipulador do arquivo
	s: status que deve assumir
**************************************************************************/
set_mutante_inactive(h, s)
int	s;
HAND_MUTANTE	*h;
{
int	i;

   REG(h).inativo = s;
   return OK;
}

/**************************************************************************
GET_MUTANTE_INACTIVE:
	Obtem o status de ativo/inativo de um mutante.
Parametros:
	h: manipulador do arquivo
	m: numero do mutante.
**************************************************************************/
get_mutante_inactive(h)
HAND_MUTANTE	*h;
{

   return REG(h).inativo;

}



/**************************************************************************
SET_BITMAP:
GET_BITMAP:
	Pega e coloca valor num bitmap dos casos de teste em relacao ao 
mutante.
Parametros:
	reg: descritor de um mutante
	i: numero do caso de teste a pegar o valor
	valor: valor a colocar
**************************************************************************/
get_bitmap(reg, i)
WORD 	*reg;
int	i;
{
int	j, k;
WORD	c;

   j = i / 5;
   k = i % 5;
   c = reg[j] >> (k*3);
   return (int) c & 0x7;

}

set_bitmap(reg, i, valor)
WORD	*reg;
int	i, valor;
{
int	j, k;
WORD	c, d;

   j = i / 5;
   k = i % 5;

   c = valor;
   reg[j] &= (~ (0x7 << (k*3)));
   reg[j] |= c << (k * 3);
}

/*************************************************************************
GET_CAUSA_MORTIS:
	Devolve qual o motivo da causa da morte de um mutante
*************************************************************************/
get_causa_mortis(reg)
REG_MUTANTE	*reg;
{
int	i;

   i = reg->status1;	/*pega numero do caso de teste responsavel pela morte */
   return get_bitmap(reg->tcase, i);
}



/****************************************************************************
SET_STATUS_MUTA:
	Altera o status de equivalente para vivo ou vice_versa.

Parametros:
	h: manipulador do arquivo
	i: numero do mutante. mutante so pode ser VIVO ou EQUIV

AUtor: Delamaro
****************************************************************************/
troca_status_muta(h, r)
int	r;
HAND_MUTANTE	*h;
{
int	j, s;

   j = get_mutante_status(h);

   switch(j)
   {
	case VIVO:
		s = EQUIV;
		break;
	case EQUIV:
		s = VIVO;
		break;
	default:
		return ERRO;
   }

   if (r != s)
	return ERRO;

   if (set_mutante_status(h, r, 0, 0) == ERRO)
	     return ERRO;

  return grava_cab_indice(h);
}





/****************************************************************************
GERA_DESCRITOR:
	Cria a parte variavel de um descritor de mutante

Parametros:
	buf: local onde deve ser montado o descritor
	inic: offset inicial da alteracao no fonte. (iniciando em 1)
	tamanho: tamanho do fonte a ser retirado.
	string: string a ser inserido no fonte.

Saida: 
	tamanho do descritor, incluindo (DWORD ) 0 apos o string.

*****************************************************************************/
gera_descritor(buf, tipo, inic, tamanho, no, string)
DESCR_MUTANTE	*buf;
long	inic;
int	tipo, tamanho, no;
char	*string;
{
int	t;
DESCR_MUTANTE	*p, *fim_descr();

   buf->offset = inic;
   buf->tamanho = (tamanho > 0) ? tamanho : 0;
   buf->no = no;
   buf->tipo = tipo;
   strcpy(buf->string, string);
   p = fim_descr(buf);
   t = ((char *) p - (char *) buf) + sizeof(buf->offset);
   p->offset = 0;
   return t;
}


/************************************************************************
FIM_DESCR:
	Devolve endereco do fim de uma parte do descritor.
************************************************************************/
DESCR_MUTANTE	*fim_descr(buf)
DESCR_MUTANTE	*buf;
{
int	t;
DESCR_MUTANTE	*p;
DWORD	*q;

   q = (DWORD *) buf;
   t = strlen(buf->string)+1;
   while (q <= (DWORD *) &buf->string[t])
	q++;

   return (DESCR_MUTANTE *) q;
}


more_descritor(buf, tipo, inic, tamanho, no, string)
DESCR_MUTANTE	*buf;
long	inic;
int	tamanho, no, tipo;
char	*string;
{
DESCR_MUTANTE	*p = buf;

   do {
	p = fim_descr(p);
      } while (p->offset != 0);
   return gera_descritor(p, tipo, inic, tamanho, no, string);
}


int	cmp_muta(void *p1, void *p2)
{
WORD	*x1, *x2;
DWORD	*dx1, *dx2;

    dx1 = (DWORD *) p1;
    dx2 = (DWORD *) p2;

    if (*dx1 < *dx2 )
	return -1;
    if (*dx1 > *dx2)
	return 1;
    dx1++; dx2++;

    if (*dx1 < *dx2 )
        return -1;
    if (*dx1 > *dx2)
        return 1;
    x1 = (WORD *) (dx1+1);
    x2 = (WORD *) (dx2+1);

    if (*x1 < *x2 )
        return -1;
    if (*x1 > *x2)
        return 1;
    x1++; x2++;

    if (*x1 < *x2 )
        return -1;
    if (*x1 > *x2)
        return 1;
    x1++; x2++;

    if (*x1 < *x2 )
        return -1;
    if (*x1 > *x2)
        return 1;
    x1++; x2++;

    return 0;
}


void	print_muta(void *x)
{
}

