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



typedef	enum {  		/* possiveis tipos de cada no da expressao */
		VARIAVEL, 	/* variavel */
		CAMPO,		/* campo de estrutura */
		CONST_INT, 	/* constante numerica inteira (0x7b) */
		CONST_ENUM,	/* constante enumeration */
		CONST_FLOAT,	/* constante floatt como 1.2E-9 */
		CONST_CHAR,	/* constante por exemplo '\n' */
		STRING,		/* constante string */
		OP_1_ARIT,	/* operador unario */
		OP_1_LOG,	/* operador unario */
		OP_1_BIT,	/* operador unario */
		OP_1_REF,	/* operador unario & */
		OP_1_DEREF,	/* operador unario  * */
		OP_1_SIZEOF,	/* operador unario */
		OP_1_CAST,	/* operador unario */
		OP_2_ARIT,	/* operador binario */
		OP_2_BIT,	/* operador binario */
		OP_2_LOG,	/* operador binario */
		OP_2_SHIFT,	/* operador binario */
		OP_2_RELA,	/* operador binario */
		OP_2_SEQ,	/* operador binario */
		OP_2_ARGUMENTO,	/* lista de expressoes */
		OP_0_ARGUMENTO,	/* lista de expressoes vazia */
		OP_ASS_ARIT,	/* assinalamento binario (arithmetic assignment)*/
		OP_ASS_BIT,	/* assinalamento binario (bitwise assignment)*/
		OP_ASS_PLAIN,	/* assinalamento binario (plain assignment)*/
		OP_ASS_SHIFT,	/* assinalamento binario (shift assignment)*/
		OP_3,		/* operador ternario (expr. condicional'?')  */
		OP_POS1_ARIT,	/* operador unario posfixado */
		OP_POS2,	/* operador binario posfixado */
		NOME_TIPO,	/* nome de tipo, por exemplo "int *" */
		SUB_EXPRE,	/* sub expressao infixa */
		NOP		/* no vazio */
	     } TIPO_NO;


typedef struct {		/* tipo expressao-posfixa */
		int	topo;	/* apont. para fim da expressao */
		struct	no 
			{
			   short	nome;	/* apont para nome do no 
						dentro de BUF*/
			   TIPO_NO 	tipo;	/* tipo do no */
			   short	final;	/* apont para fim da sub expr */
			} no[300];
		short	livre;		/* posicao livre no buffer */
		char	buf[1024];
		} EXPRE_POS;


#define		NOME(x,i)	(&(x->buf[x->no[i].nome]))


#define		EH_CONST(x)	(x>=CONST_INT&&x<=STRING)
