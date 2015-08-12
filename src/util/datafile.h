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


/**
 * Functions related to reading and writing file headers.
 *
 * @author Márcio Eduardo Delamaro
 * @created 30/09/1994
 */

#ifndef		_PROTEUM_DATAFILE_H
#define		_PROTEUM_DATAFILE_H

#include <stdio.h>
#include <stdbool.h>


#define		OFFSET0		64	/* tamanho do cabecalho de identificacao colocado nos arquivos de trabalho do proteum */


bool grava_ident(FILE * fp, char * str, char * version);

bool checa_ident(FILE *fp, char *str, char * version);

#endif

