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
#include	<muta/lib/muta.h>
#include	<tcase/lib/tcase.h>

#define		DO_KNO		0
#define		NO_PASS		1
#define		PASS_CALLING	2
#define		PASS_BOTH	3

static LOG_TCASE	*vlog[MAX_TCASE];
static char		vflag[MAX_TCASE];

static	DWORD	calling, called;

clear_exec_log(HAND_TCASE *HTCase, HAND_MUTANTE *HMuta)
{
int i;

   for (i = 0; i < LEN(vlog); i++)
	release_log(vlog[i]);
}



check_exec_log(HAND_TCASE *HTCase, HAND_MUTANTE *HMuta, int k)
{
int	n, r;
DESCR_MUTANTE	*descr1, *fim_descr();

   if ( vflag[k] == DO_KNO )
   {
   	if ( le_tcase_reg(HTCase, k) == ERRO)
	   return ERRO;
	if ( TREG(HTCase).tamanho_log <= 0)
	   return TRUE;
	if ( tcase_get_log(HTCase, calling,called > 0? called: calling) == ERRO)
	   return ERRO;
	vlog[k] = HTCase->log;
	HTCase->log = NULL;
	if ( test_node_log(vlog[k], calling, 0 ) )
	   if (test_node_log(vlog[k], called, 0) )
		vflag[k] = PASS_BOTH;
	   else
		vflag[k] = PASS_CALLING;	
	else
	   vflag[k] = NO_PASS;
   }
   if ( vflag[k] == NO_PASS)
	return FALSE;
   descr1 = &(DESCR(HMuta));
   if ( descr1->tipo == CONNECT_DESCR )
   {
	if (vflag[k] != PASS_BOTH)
	   return FALSE;
	if ( ! test_node_log(vlog[k], calling, descr1->no) )
	   return FALSE;
	descr1 = fim_descr(descr1);
	while (descr1 != NULL && descr1->offset > 0)
	{
	   if ( test_node_log(vlog[k], called, descr1->no) )
		return TRUE;
	   descr1 = fim_descr(descr1);
	}
	return FALSE;
   }
   else
   {
	if (vflag[k] != PASS_CALLING && vflag[k] != PASS_BOTH)
	   return FALSE;
	while (descr1 != NULL && descr1->offset > 0)
 	{
	   if ( test_node_log(vlog[k], calling, descr1->no) )
		return TRUE;
	   descr1 = fim_descr(descr1);
	}
	return FALSE;
   }
}


init_exec_log(HAND_TCASE *HTCase, HAND_MUTANTE *HMuta)
{
int i;

   calling = REG(HMuta).calling;
   called = REG(HMuta).called;
   for (i = 0; i < LEN(vlog); i++)
   {
	vflag[i] = DO_KNO;
	vlog[i] = NULL;
   }
}
