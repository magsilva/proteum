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



/*-------------------------- Extensoes dos arquivos -----------------*/


#define		SUFIXO_IO	".IOL"
#define		SUFIXO_TCASE	".TCS"
#define		SUFIXO_INPUT	".STDIN.TMP"
#define         SUFIXO_OUTPUT	".STDOUT.TMP"
#define         SUFIXO_ERRO	".STDERR.TMP"
#define		SUFIXO_LOG	".LOG.TMP"

#define		PROG_TIMEOUT	"extimout"



/*-------------------------- Identificacao dos arquivos -----------------*/


#define		IDENT_TCASE	"PROTEUM - TEST CASE FILE"

#define		IDENT_IO	"PROTEUM - I/O LOG FILE"


/*---------------------------------------------------------------------
	Estruturas arquivo de casos de teste				*/

#define		MAX_TCASE	512	/* numero maximo de casos de teste */
#define		PARAMSIZE	1024	/* tamanho maximo do parametro */
#define		LABELSIZE	10


typedef	struct {
		DWORD	contlog;	/* contador logico dos casos de teste */
		WORD	ntcase;		/* contador de casos de teste */
		struct {
		        DWORD	reg_log; /* numero logico do caso de 
							teste */
			CHAR	ocup:1;	/* flag de ocupado */
			CHAR	desabili:1;	/* flag de desabilitado */
			CHAR	error:1;     /* tcase cause error termination */
			} tab_fis[MAX_TCASE];	/* tabela de reg fisicos */
		WORD	seqlog[MAX_TCASE];	/*tabela de sequencia */
		} CAB_TCASE;

typedef struct {
		long	exetempo;	/* tempo de CPU de execucao */
		long	totaltempo;	/* total execution time */
		int	retcode;	/* codigo retorno */
		unsigned int interativo:1; /* interactive input or not */	
		char    param[PARAMSIZE+1];	/* parametro inicial */
		char    label[LABELSIZE+1];
		int	tamanho_ent;	/* tamanho da entrada */
		int	tamanho_sai;	/* tamanho saida */
		int	tamanho_err;	/* tamanho da saida-erro */
		int	tamanho_log;	/* tamanho log de execucao */
		long	apont_ent;	/* apontador entrada */
		} REG_TCASE;

typedef struct {
		int	nfunction;	/* number of visited functions */
		int	vetsize;	/* number of avaiable places in vet */
		struct vetfunc
		{
		   long		function; /* function identifier */
		   int		nnode;	/* number of nodes per functio */
		   char		*nodes; /* one bit per node */
		}		*vetfunc; /* array of functions */
		} LOG_TCASE;

typedef struct {
                FILE            *fp_tcase,
                                *fp_io;
                CAB_TCASE       cab;
                REG_TCASE       reg;
		LOG_TCASE	*log;
                struct  
                {
                   long inic;
                   int  size;
                }               deleted;
                } HAND_TCASE;


#define		TCAB(x)		(x)->cab
#define		TREG(x)		(x)->reg
#define		TFILE(x)	(x)->fp_tcase
#define		TFILEIO(x)	(x)->fp_io
#define		TDELETED(x)	(x)->deleted

#define		SEQLOG(x)	TCAB(x).seqlog
#define		TAB_FIS(x)	TCAB(x).tab_fis
#define		CONTLOG(x)	TCAB(x).contlog
#define		NTCASE(x)	TCAB(x).ntcase


/*------------------------ CODES -----------------------------*/
#define		CODE_ERROR	(WORD) (-2)

/*--------------------------- FUNCTIONS ------------------------------*/
int 	cria_arquivo_tcase(HAND_TCASE *, char *, char *);
int	carrega_arquivo_tcase(HAND_TCASE *, char *, char *);
int	descarrega_arquivo_tcase(HAND_TCASE *);
int	grava_tcase_cab(HAND_TCASE *);
int	grava_tcase_reg(HAND_TCASE *, int);
int	le_tcase_reg(HAND_TCASE *, int);
int	ins_tcase_reg(HAND_TCASE *, REG_TCASE *);
int	delete_tcase(HAND_TCASE *, int, int);
int	ltofis_tcase(HAND_TCASE *, int);
int	fistol_tcase(HAND_TCASE *, int);
int	disable_tcase(HAND_TCASE *, int);
int	enable_tcase(HAND_TCASE *, int);

LOG_TCASE	*get_log_from_file(FILE *);
void		release_log(LOG_TCASE *);
int		save_log(LOG_TCASE *, FILE *);
LOG_TCASE	*new_log();
int 		load_log(LOG_TCASE *, FILE *, long , long);
int		test_node_log(LOG_TCASE *, long , int );


