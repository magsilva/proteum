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


#include	<string.h>
 
#include	<li/lib/li.h>


/* ----------- NUMBER FOR EACH OPERATOR -------------------*/

/* group I */
#define		COV_ALL_EDGE		0
#define		COV_ALL_NODE		(COV_ALL_EDGE+1)
#define		DIR_VAR_ARITH_NEG	(COV_ALL_NODE+1)
#define		DIR_VAR_BIT_NEG		(DIR_VAR_ARITH_NEG+1)
#define		DIR_VAR_INCR_DECR	(DIR_VAR_BIT_NEG+1)
#define		DIR_VAR_LOG_NEG		(DIR_VAR_INCR_DECR+1)
#define		DIR_VAR_REP_CONST	(DIR_VAR_LOG_NEG+1)
#define         DIR_VAR_REP_EXT         (DIR_VAR_REP_CONST+1)
#define		DIR_VAR_REP_GLOB	(DIR_VAR_REP_EXT+1)
#define         DIR_VAR_REP_LOC		(DIR_VAR_REP_GLOB+1)
#define         DIR_VAR_REP_PAR        	(DIR_VAR_REP_LOC+1)
#define		DIR_VAR_REP_REQ		(DIR_VAR_REP_PAR+1)
#define         IND_VAR_ARITH_NEG       (DIR_VAR_REP_REQ+1)
#define         IND_VAR_BIT_NEG         (IND_VAR_ARITH_NEG+1)
#define         IND_VAR_INCR_DECR       (IND_VAR_BIT_NEG+1)
#define         IND_VAR_LOG_NEG		(IND_VAR_INCR_DECR+1)
#define         IND_VAR_REP_CONST       (IND_VAR_LOG_NEG+1)
#define         IND_VAR_REP_EXT         (IND_VAR_REP_CONST+1)
#define         IND_VAR_REP_GLOB        (IND_VAR_REP_EXT+1)
#define         IND_VAR_REP_LOC         (IND_VAR_REP_GLOB+1)
#define         IND_VAR_REP_PAR         (IND_VAR_REP_LOC+1)
#define         IND_VAR_REP_REQ         (IND_VAR_REP_PAR+1)
#define		DEL_RET_STAT		(IND_VAR_REP_REQ+1)
#define		REP_RET_STAT		(DEL_RET_STAT+1)


/* group II */
#define		ARG_ARITH_NEG	(REP_RET_STAT+1)
#define		ARG_BIT_NEG	(ARG_ARITH_NEG+1)
#define		DEL_ARG		(ARG_BIT_NEG+1)
#define		ARG_INCR_DECR	(DEL_ARG+1)
#define		ARG_LOG_NEG	(ARG_INCR_DECR+1)
#define         REP_ARG_REQ     (ARG_LOG_NEG+1)
#define		SW_ALIKE_ARG	(REP_ARG_REQ+1) 
#define         SW_DIF_ARG    	(SW_ALIKE_ARG+1)
#define		DEL_FUNC_CALL	(SW_DIF_ARG+1)

/* grupo U (UNIDADE) */
#define		U_CCCR		(DEL_FUNC_CALL+1)
#define		U_CCSR		(U_CCCR+1)
#define 	U_CRCR		(U_CCSR+1)
#define		U_OAAA		(U_CRCR+1)
#define		U_OAAN		(U_OAAA+1)
#define		U_OABA		(U_OAAN+1)
#define		U_OABN		(U_OABA+1)
#define         U_OAEA          (U_OABN+1)
#define         U_OALN          (U_OAEA+1)
#define         U_OARN          (U_OALN+1)
#define         U_OASA          (U_OARN+1)
#define         U_OASN          (U_OASA+1)
#define         U_OBAA          (U_OASN+1)
#define         U_OBAN          (U_OBAA+1)
#define		U_OBBA		(U_OBAN+1)
#define		U_OBBN		(U_OBBA+1)
#define         U_OBEA          (U_OBBN+1)
#define         U_OBLN          (U_OBEA+1)
#define		U_OBNG		(U_OBLN+1)
#define         U_OBRN          (U_OBNG+1)
#define         U_OBSA          (U_OBRN+1)
#define         U_OBSN          (U_OBSA+1)
#define		U_OCNG		(U_OBSN+1)
#define		U_OCOR		(U_OCNG+1)
#define         U_OEAA          (U_OCOR+1) 
#define         U_OEBA		(U_OEAA+1)
#define		U_OESA		(U_OEBA+1)
#define		U_OIDO		(U_OESA+1)
#define		U_OIPM		(U_OIDO+1)
#define		U_OLAN		(U_OIPM+1)
#define		U_OLBN		(U_OLAN+1)
#define		U_OLLN		(U_OLBN+1)
#define		U_OLNG		(U_OLLN+1)
#define		U_OLRN		(U_OLNG+1)
#define		U_OLSN		(U_OLRN+1)

#define		U_ORAN		(U_OLSN+1)
#define		U_ORBN		(U_ORAN+1)
#define		U_ORLN		(U_ORBN+1)
#define 	U_ORRN		(U_ORLN+1)
#define		U_ORSN		(U_ORRN+1)
#define		U_OSAA		(U_ORSN+1)
#define		U_OSAN		(U_OSAA+1)
#define		U_OSBA		(U_OSAN+1)
#define		U_OSBN		(U_OSBA+1)
#define		U_OSEA		(U_OSBN+1)
#define		U_OSLN		(U_OSEA+1)
#define		U_OSRN		(U_OSLN+1)
#define		U_OSSA		(U_OSRN+1)
#define		U_OSSN		(U_OSSA+1)

#define		U_SBRC		(U_OSSN+1)
#define		U_SBRn		(U_SBRC+1)
#define		U_SCRB		(U_SBRn+1)
#define		U_SCRn		(U_SCRB+1)
#define		U_SDWD		(U_SCRn+1)
#define		U_SGLR		(U_SDWD+1)
#define		U_SMTC		(U_SGLR+1)
#define		U_SMTT		(U_SMTC+1)
#define		U_SMVB		(U_SMTT+1)
#define		U_SRSR		(U_SMVB+1)
#define		U_SSDL		(U_SRSR+1)
#define		U_SSWM		(U_SSDL+1)
#define		U_STRI		(U_SSWM+1)
#define		U_STRP		(U_STRI+1)
#define		U_SWDD		(U_STRP+1)

#define		U_VDTR		(U_SWDD+1)
#define		U_VGAR		(U_VDTR+1)
#define		U_VGPR		(U_VGAR+1)
#define		U_VGSR		(U_VGPR+1)
#define		U_VGTR		(U_VGSR+1)
#define		U_VLAR		(U_VGTR+1)
#define		U_VLPR		(U_VLAR+1)
#define		U_VLSR		(U_VLPR+1)
#define		U_VLTR		(U_VLSR+1)
#define		U_VSCR		(U_VLTR+1)
#define		U_VTWD		(U_VSCR+1)


#define		SUFIXO_NLI	".nli"
#define		SUFIXO_GFC	".gfc"
 
/**************************************************************************/
/*simbolo da LI */
typedef	struct  {
			char	simbolo[256];
			int	no;
			int	comprimento;
			int	linha;
			long	inicio;
		   } LI_SIMBOLO;

#define		MAX_LIN_LI	64536

#define		TAM_MAX_EXPR	4*4096	/* tamanho maximo de uma expressao */



/************************************************************************/
/* set of simbols (variables) */

typedef struct {
		SIMBOLO		**vet;
		int		cont;
		int		size;
		} VAR_SET;


/************************************************************************
macros par averificacao dos tkens da LI					*/

#define	e_abre(x)	(strcmp(x.simbolo, "{") == 0)

#define	e_fecha(x)	(strcmp(x.simbolo, "}") == 0)

#define e_par(x)	(e_abre(x) || e_fecha(x))

#define	e_if(x)		(strcmp(x.simbolo, "$IF") == 0)

#define	e_else(x)	(strcmp(x.simbolo, "$ELSE") == 0)

#define	e_case(x)	(strcmp(x.simbolo, "$CASE") == 0)

#define	e_rotc(x)	(strcmp(x.simbolo, "$ROTC") == 0 ||  strcmp(x.simbolo, "$ROTD") == 0)

#define	e_for(x)	(strcmp(x.simbolo, "$FOR") == 0)

#define	e_while(x)	(strcmp(x.simbolo, "$WHILE") == 0)

#define	e_repeat(x)	(strcmp(x.simbolo, "$REPEAT") == 0)

#define	e_until(x)	(strcmp(x.simbolo, "$UNTIL") == 0)

#define	e_goto(x)	(strcmp(x.simbolo, "$GOTO") == 0)

#define	e_break(x)	(strcmp(x.simbolo, "$BREAK") == 0)

#define	e_continue(x)	(strcmp(x.simbolo, "$CONTINUE") == 0)

#define	e_return(x)	(strcmp(x.simbolo, "$RETURN") == 0)

#define	e_label(x)	(strpbrk(x.simbolo, "$@{}") == NULL)

#define	e_sequencial(x)	(strncmp(x.simbolo, "$S", 2) == 0)

#define	e_cond(x)	(strncmp(x.simbolo, "$C(", 3) == 0 || strcmp(x.simbolo, "$CC") == 0 || strncmp(x.simbolo, "$NC(", 4) == 0)

#define	e_dcl(x)	(strcmp(x.simbolo, "$DCL") == 0)

#define	e_func(x)	x.simbolo[0] == '@'



/*---------------------------------------------------------------------------
Funcoes do modulo:
---------------------------------------------------------------------------*/
LI_SIMBOLO	*pli_getsym();	/* reads a simbol from LI file */

char	*label_muta();		/* return a label to insert in muttant */



void    var_set_new(VAR_SET *);
int     var_set_add(VAR_SET *, SIMBOLO *);
SIMBOLO	*var_set_get(VAR_SET *, int);
int     var_set_in(VAR_SET *, char *);
void    var_set_del(VAR_SET *, int);
int     var_set_dup(VAR_SET *, VAR_SET *);
void    var_set_free(VAR_SET *);
#define	var_set_card(x)		(x->cont)

