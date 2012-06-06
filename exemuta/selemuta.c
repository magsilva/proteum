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


/***************************************************************************
				S E L E M U T A . C 
Este modulo marca mutantes como ativos/inativos

****************************************************************************/
#include        <string.h>
#include	<lib/gerais.h>
#include	<li/lib/li.h>
#include	<muta/lib/muta.h>
#include	<tcase/lib/tcase.h>
#include	<pteste/lib/pteste.h>



extern	OPERADOR_MUTACAO	g_tab_operador[];

extern	char	Funcoes[];

char	s[500], s1[500], buf[1024];

/***************************************************************************
SELE_BYOP_MUTANTE:
	Esta funcao seleciona uma porcentagem aleatoria de mutantes
para cada operador de mutacao

Autor: MArcio Delamaro
Data: 13-10-94

***************************************************************************/
sele_byop_mutante(HMuta, dequal, atequal, keep)
HAND_MUTANTE	*HMuta;
int	dequal, atequal, keep;
{
int	i, k, j, conec;
long	lcalled, lcalling, dl;
int	d;
char	*p, *q, *from_buf();

   if (GERADOS(HMuta) == 0)
	msg("No Mutants to Select");

   lcalled = lcalling = -1;
   conec = 0;
   i = dequal;
   while (i < atequal)
   {
        if (carrega_descr(HMuta, i) == ERRO)
            return;
	if (keep && get_mutante_inactive(HMuta))
	{
	  i++;
	  continue;
	}
	if (REG(HMuta).called != lcalled || REG(HMuta).calling != lcalling)
	{
	   lcalled = REG(HMuta).called;
	   lcalling = REG(HMuta).calling;
	   s[0] = s1[0] = '\0';
	   get_descr_func_l(HMuta, lcalling, s, NULL, &d,&d,&d,&dl,&d);
           if (lcalled >= 0)
               get_descr_func_l(HMuta, lcalled, s1, NULL, &d,&d,&d,&dl,&d);
	   inic_from_buf(Funcoes);
	   p = from_buf();
	   conec = 1;
	   if ( p != NULL)
	   {
		for ( ; p != NULL; p = from_buf())
	   	{
                	strcpy(buf, p);
                	q = strchr(buf, '$');
                	*q = '\0';
		   	if (strcmp(buf, "-all") == 0 || strcmp(buf, s) == 0)
			   if (strcmp(q+1,"-all") == 0 || strcmp(q+1,s1) == 0)
				break;
	   	}
	   	conec = (p != NULL);
	   }
	}


	if ( conec && sorteio(REG(HMuta).operador))
	   j = 0;
	else
	   j = INATIVO;
	set_mutante_inactive(HMuta,j);
	i++;
	rewrite_mutante(HMuta);
    }

   atualiza_mutantes(HMuta);
}



#define new_node(x,y,z)         x!=REG(HMuta).operador || y!=REG(HMuta).called\
                                || z!=DESCR(HMuta).no

#define new_stat(x)             x!=DESCR(HMuta).offset

/***************************************************************************
SELE_BYOP_MUTANTE:
        Esta funcao seleciona um numero maximo de comandos a serem mutados
por bloco da funcao


Autor: MArcio Delamaro
Data: 1-11-95

***************************************************************************/
sele_byblock_mutante(HMuta, dequal, atequal, mmaximo, nmaximo, keep)
HAND_MUTANTE    *HMuta;
int     dequal, atequal, mmaximo, nmaximo, keep;
{
}




