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


#include <stdio.h>

#include "li.h"

extern struct tableout saida;
extern FILE *arqli;
extern long ini_token;
extern long comp_token;
extern long comp_ant;
extern long lin_token;
extern int  flg_gera;

int  pred, seq, cond;

extern int	pred, nnodes;

int loc;

void grava_li  (arg,s) 
int arg;
char *s;
{
char buf[30];

 if  (flg_gera)
 {
  switch (arg) {
  case 1:
    sprintf(buf, "%s", s);
    break;
  case 2:
    sprintf(buf, "%s%d", s, seq++);
    break;
  case 3:
    sprintf(buf, "%s(%d)%d", s, pred, cond++);
    break;
  default: printf("erro nos argumentos de grava_li\n");
	   break; 
  }
  fprintf (arqli,"%-15s%6d %10ld %6d %6d\n", 
			buf,nnodes, ini_token,comp_token,lin_token);
  ini_token = 0;
 }
/*
 if (!strcmp(s,"$IF") || 
    !strcmp(s,"$S") || !strcmp(s,"$REPEAT") || !strcmp(s,"$FOR") ||
    !strcmp(s,"$WHILE") || strcmp(s,"$CASE") || !strcmp(s,"$BREAK") ||
    !strcmp(s,"$CONTINUE") || !strcmp(s,"$GOTO")) 
			++loc;
 
 if (strcmp(s,"$DCL") && strcmp(s,"$C") && strcmp(s,"$UNTIL") && strcmp(s,"$ELSE") && strcmp(s,"{") && strcmp(s,"}"))
		++loc; */
}

void calcula_posicao ()
{
 if (ini_token == 0)
   {
    ini_token=saida.inicio;
    lin_token=saida.linha;
   }
 comp_ant = comp_token;
 comp_token=((int)(saida.inicio-ini_token))+saida.comp;
}
