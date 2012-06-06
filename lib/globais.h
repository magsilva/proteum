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


/*************************************************************************
 				G L O B A I S . H

	Estruturas e definicoes usadas por globais.c

*************************************************************************/

#define		NOPERADORES	108 /* Numero de Operadores*/

typedef	struct	{
		   char		*nome;		/* mnemonico do op. */
		   char		*coment;	/* explicacao */
		   int		use_trace; 	/* diz se deve usar trace */
		   char		percent;	/*percentual a gerar */
		   int		maximum;	/* maximum per point */
		   unsigned	long	semente,
					semente2;/*usada pra geracao aleatoria*/
		   int		(*func_gera)();	/* funcao para geracao */
		} OPERADOR_MUTACAO;


