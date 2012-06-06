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


#include	<btree/btree.h>


#define		SUFIXO_INDICE	".IND"
#define		SUFIXO_MUTANTE	".MUT"
#define		SUFIXO_EXEC	""
#define         SUFIXO_FUNTABLE ".FTB"

//#define		MUTA_DESCR_SIZE	MAX_TCASE/5+1
#define		MUTA_DESCR_SIZE	512/5+1

#define		IDENT_INDICE	"PROTEUM - MUTANT INDEX FILE"

#define		IDENT_MUTANTE	"PROTEUM - MUTANT DESCRIPTOR FILE"


/********************** Possiveis Status dos mutantes *********************/
#define		VIVO		0
#define		MORTO		1
#define		EQUIV		2
#define		ANOMALO		3

#define		INATIVO		1


/*********************** Possiveis tipos de descritores *****************/
#define		CONNECT_DESCR	0
#define		OPER_DESCR	1


/************* Possiveis status dos casos de teste wrt mutantes **********/

#define		NO_EXEC			0	/* nao executado ainda*/
#define		EXEC_VIVO		1	/* executado e vivo */
#define		EXEC_MORTO_SAIDA	2 /* por resultados diferentes */
#define		EXEC_MORTO_RETCOD	3 /* por ret. code diferentes */
#define		EXEC_MORTO_TIMEOUT 	4 /* por timeout */
#define		EXEC_MORTO_TRAP		5 /* por funcao TRAP */
#define		AVOIDED			6 /* execution avoided by tcase log */
#define		NO_EXEC_COMP		7 /* mutante anomalo */

#define		EXEC_MORTO(x)		(x >= EXEC_MORTO_SAIDA && x <= EXEC_MORTO_TRAP)





/*************************************************************************
	Estruturas arquivo de mutantes					*/

#define		CHAVE_SIZE	(2*sizeof(DWORD)+3*sizeof(WORD))
#define         FUNC_NAME       256

#define         FUNC_KEY_SIZE   (sizeof(DWORD))

typedef struct {
		DWORD	inic_func;	/* offset inicial da funcao */
		WORD	inic_nome;	/* inicio do nome da funcao */
		WORD	tam_dcl;	/* tamanho da declaracao da funcao */
		WORD	tam_cab;	/* tamanho do cabecalho da funcao */
		DWORD	tam_func;	/* tamanho total da funcao era WORD foi mudado para DWORD 17/12/2008 */
		CHAR	e_void;		/* flag que indica se tipo eh void */
                char    name[FUNC_NAME+1]; /* nome da funcao */
                char    param[FUNC_NAME+1]; /* parametros da funcao */
		} DESCR_FUNCAO;


typedef	struct {
		int	gerado;		/* numero de mutantes gerados */
                int	executado;      /* numero de mutantes executados */
		int	vivo;		/* numero de vivos */
		int	equiv;		/* numero de equivalentes */
		int	anomalo;	/* numero de anomalo */
		int	inativo;	/* numero de inativos */
		int	total[NOPERADORES]; /*numero de mutantes por operador */
                int	totativo[NOPERADORES]; /* total de mutantes ativos */		} CAB_INDICE;


typedef struct {
                DWORD   calling;        /* inicio da funcao chamante */
		DWORD	called;		/*inicio da funcao chamada */
		WORD	seq1;		/* place inside calling func */
                WORD    operador;       /* operador que gerou */
		WORD	seq2;		/* place inside called func. */
		WORD	status:3;	/* vivo, morto, anomalo, equiv */
		WORD	inativo:1;	/* ativo / desativo */
		WORD	tamanho;	/* tamanho do descritor do registro */
		WORD	status1, status2; /* dois bytes de status */
		DWORD	ultimo_tcase;	/* ultimo caso de teste */
		WORD	tcase[MUTA_DESCR_SIZE];	/* bitmap dos casos de teste */
/*		CHAR	descritor[0];	 descricao dos mutantes */
		} REG_MUTANTE;

typedef struct	{			/*descritor variavel do mutante */
		DWORD	offset;		/* offset da alteracao */
		DWORD	tamanho;	/*tamanho do string a retirar */
		WORD	no;		/* noh do grafo de programa */
		WORD	tipo;
		char	string[1024];	/* string a acrescentar */
		} DESCR_MUTANTE;


 
typedef struct {
		FILE		*fp_indice;
		BTREE		*bt, *btfun;
		CAB_INDICE	indice_cab;
		REG_MUTANTE	mutante_reg;
		DESCR_MUTANTE	mutante_descr;
		} HAND_MUTANTE;



/***************** MACROS DE ACESSO AA ESTRUTURA HAND_MUTA *****************
Todas essas macros sao chamadas passando-se o endereco de uma estrutura
do tipo HANDO_MUTANTE */

#define		CABECALHO(x)	(x)->indice_cab
#define		INDICES(x)	(x)->indice_regs
#define		REG(x)		(x)->mutante_reg
#define		CHAVE(x)	(x)->mutante_reg
#define		DESCR(x)	(x)->mutante_descr


#define		GERADOS(x)	CABECALHO(x).gerado
#define		VIVOS(x)	CABECALHO(x).vivo
#define		ANOMALOS(x)	CABECALHO(x).anomalo
#define		EQUIVS(x)	CABECALHO(x).equiv
#define		INATIVOS(x)	CABECALHO(x).inativo
#define		PERCENT(x)	CABECALHO(x).percent
#define		TOTAL(x)	CABECALHO(x).total


int 	rewrite_mutante(HAND_MUTANTE *);
int	troca_status_muta(HAND_MUTANTE *, int);
int	cria_arquivo_mutante(HAND_MUTANTE *, char *, char *);
int	carrega_arquivo_mutante(HAND_MUTANTE *, char *, char *);
int	descarrega_arquivo_mutante(HAND_MUTANTE *);
int	carrega_descr(HAND_MUTANTE *, int);
int	ins_mutante(HAND_MUTANTE *, int, long, long, 
				int, int, DESCR_MUTANTE *, int);
int	carrega_descr_key(HAND_MUTANTE *, long, long,int,int, int);
int	rewrite_mutante(HAND_MUTANTE *);
int	set_mutante_status(HAND_MUTANTE *, int, int, int);
int	get_mutante_status(HAND_MUTANTE *);
int	set_mutante_inactive(HAND_MUTANTE *, int);
int	get_mutante_inactive(HAND_MUTANTE *);
int	troca_status_muta(HAND_MUTANTE *, int);
int	gera_descritor(DESCR_MUTANTE *, int, long, int, int, char *);
DESCR_MUTANTE   *fim_descr(DESCR_MUTANTE *);
int     more_descritor(DESCR_MUTANTE *, int, long, int, int, char *);

