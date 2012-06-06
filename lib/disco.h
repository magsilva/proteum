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
				D I S C O . H

	Estruturas e definicoes usadas por disco.c

Autor: Marcio Eduardo Delamaro
Data: 02-09-94
Documentacao: 24-10-94 (Mirian)
*************************************************************************/





/**************************************************************************
	Estrutura para definir a tabela de arquivos abertos */

#define		MAX_ARQ		20	/*numero maximo de arquiovos */

typedef	struct	{
		   char		nome[NOME_LENGTH+1];
		   char		dir[2*NOME_LENGTH+1];
		   char		ext[NOME_LENGTH+1];
		   FILE		*fp;
		} ARQUIVO;




FILE	*abrearq(),
	*criarq();

char    *dirarq(),
        *nomearq();

int	fecharq(),
	lockarq(),
	delearq(),
	learq(),
	gravarq(),
	posiciona(),
	posifim(),
	d_msg();
