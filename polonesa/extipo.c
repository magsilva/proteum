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


#include	<stdio.h>
#include	<string.h>

#include	"polonesa.h"
#include	<lib/gerais.h>



EXPRE_POS	expre_pos, aux_expre;


static	char	bufaux[1024];




/**************************************************************************
EXEC_TIPO:
	Calcula tipo a partir de expressao posfixa 

Parametros:
	e: endereco de expressao posfixa 
	k,l: inicio e final da (sub) expressa a avaliar.

Saida:
	endereco da expressao infixa montada
***************************************************************************/
exec_tipo(e, k, l)
EXPRE_POS	*e;
int	k,l;
{
int	i, j;
char	*c1, *c2, *pega_tipo(), *pega_ptar();
char	tipo[50], ptar[50];


   zera_expre(&aux_expre);
   for (i = k; i <= l; i++)
      {
	switch (e->no[i].tipo)
	{
	   case VARIAVEL:
		get_tipo_varr(NOME(e, i), tipo, ptar); 
		l1:
		ins_expre(&aux_expre, tipo, SUB_EXPRE);
		strcpy(&aux_expre.buf[aux_expre.livre], ptar);
		aux_expre.livre += strlen(ptar)+1;
		break;

	   case CONST_INT:
	   case CONST_ENUM:
		l2: 
		strcpy(tipo, "int");
		strcpy(ptar, "");
		goto l1;

           case CONST_FLOAT:
                strcpy(tipo, "float");
                strcpy(ptar, "");
                goto l1;

	   case CONST_CHAR:
                strcpy(tipo, "char");
                strcpy(ptar, "");
                goto l1;

		      
		
	   case STRING:
		strcpy(tipo, "char");
		strcpy(ptar, "*");
		goto l1;
		

	   case CAMPO:
		ins_expre(&aux_expre, &(e->buf[e->no[i].nome]), e->no[i].tipo);
		break;

	   case OP_1_BIT:
		if (! e_inteiro(pega_tipo(&aux_expre, aux_expre.topo),
				pega_ptar(&aux_expre, aux_expre.topo)))
			     return ERRO;
		break;

	   case OP_1_LOG:
		if (! e_struct(pega_tipo(&aux_expre, aux_expre.topo),
				pega_ptar(&aux_expre, aux_expre.topo)))
			     break;
		return ERRO;

	   case OP_1_ARIT:
	   case OP_POS1_ARIT:
		j = ex_op1_arit(NOME(e,i), 
				pega_tipo(&aux_expre, aux_expre.topo),
			        pega_ptar(&aux_expre, aux_expre.topo),
			        tipo, ptar);
		if (j == ERRO)
		         return ERRO;
		aux_expre.livre = aux_expre.no[aux_expre.topo--].nome;
		goto l1;


	   case OP_0_ARGUMENTO:
		goto l2;

	   case OP_1_SIZEOF:
		aux_expre.livre = aux_expre.no[aux_expre.topo--].nome;
		goto l2;

	   case OP_1_REF:
		aux_expre.buf[aux_expre.livre-1] = '*';
		aux_expre.buf[aux_expre.livre++] = '\0';
		break;

	   case OP_1_DEREF:
		j = ex_op1_deref(pega_tipo(&aux_expre, aux_expre.topo),
			     pega_ptar(&aux_expre, aux_expre.topo),
			     tipo, ptar);
		if (j == ERRO)
		         return ERRO;

		aux_expre.livre = aux_expre.no[aux_expre.topo--].nome;
		goto l1;

	   case OP_1_CAST:
	   	strcpy(bufaux, NOME(e, i));
		k = 0;
		while (bufaux[k] == '_' || isspace(bufaux[k]) || 
				isalnum(bufaux[k]))
			k++;
		strcpy(ptar, &bufaux[k]);
		while (isspace(bufaux[--k]) && k > 0);
		    k++;
		bufaux[k] = '\0';
		strcpy(tipo, bufaux);
		aux_expre.livre = aux_expre.no[aux_expre.topo--].nome;
		goto l1;

	   case NOME_TIPO:
	   	strcpy(bufaux, NOME(e, i));
		k = 0;
		while (bufaux[k] == '_' || isspace(bufaux[k]) || 
				isalnum(bufaux[k]))
			k++;
		strcpy(ptar, &bufaux[k]);
		bufaux[k] = '\0';
		strcpy(tipo, bufaux);
		goto l1;

	   case OP_ASS_BIT:
	   case OP_ASS_SHIFT:
	   case OP_2_SHIFT:
	   case OP_2_BIT:
		j = ex_op2_bit(pega_tipo(&aux_expre, aux_expre.topo-1),
			     pega_ptar(&aux_expre, aux_expre.topo-1),
			     pega_tipo(&aux_expre, aux_expre.topo),
			     pega_ptar(&aux_expre, aux_expre.topo),
			     tipo, ptar);
		if (j == ERRO)
		         return ERRO;

		aux_expre.topo--;
		aux_expre.livre = aux_expre.no[aux_expre.topo--].nome;
		goto l1;

	   case OP_2_RELA:
	   case OP_2_LOG:
		j = ex_op2_log(pega_tipo(&aux_expre, aux_expre.topo-1),
			     pega_ptar(&aux_expre, aux_expre.topo-1),
			     pega_tipo(&aux_expre, aux_expre.topo),
			     pega_ptar(&aux_expre, aux_expre.topo),
			     tipo, ptar);
		if (j == ERRO)
		         return ERRO;

		aux_expre.topo--;
		aux_expre.livre = aux_expre.no[aux_expre.topo--].nome;
		goto l1;

	   case OP_ASS_PLAIN:
		l3:
		j = ex_ass_plain(pega_tipo(&aux_expre, aux_expre.topo-1),
			     pega_ptar(&aux_expre, aux_expre.topo-1),
			     pega_tipo(&aux_expre, aux_expre.topo),
			     pega_ptar(&aux_expre, aux_expre.topo),
			     tipo, ptar);
		if (j == ERRO)
		         return ERRO;

		aux_expre.topo--;
		aux_expre.livre = aux_expre.no[aux_expre.topo--].nome;
		goto l1;


	   case OP_2_ARIT:
		j = ex_op2_arit(NOME(e,i),
			     pega_tipo(&aux_expre, aux_expre.topo-1),
			     pega_ptar(&aux_expre, aux_expre.topo-1),
			     pega_tipo(&aux_expre, aux_expre.topo),
			     pega_ptar(&aux_expre, aux_expre.topo),
			     tipo, ptar);
		if (j == ERRO)
		         return ERRO;
		aux_expre.topo--;
		aux_expre.livre = aux_expre.no[aux_expre.topo--].nome;
		goto l1;



	   case OP_ASS_ARIT:
		j = ex_op2_arit(NOME(e,i),
			     pega_tipo(&aux_expre, aux_expre.topo-1),
			     pega_ptar(&aux_expre, aux_expre.topo-1),
			     pega_tipo(&aux_expre, aux_expre.topo),
			     pega_ptar(&aux_expre, aux_expre.topo),
			     tipo, ptar);

		if (j == ERRO)
		         return ERRO;
		aux_expre.livre = aux_expre.no[aux_expre.topo--].nome;
		ins_expre(&aux_expre, tipo, SUB_EXPRE);
		strcpy(&aux_expre.buf[aux_expre.livre], ptar);
		aux_expre.livre += strlen(ptar)+1;
		goto l3;


	   case OP_2_SEQ:
	   case OP_2_ARGUMENTO:
		strcpy(tipo, pega_tipo(&aux_expre, aux_expre.topo));
		strcpy(ptar,pega_ptar(&aux_expre, aux_expre.topo));
		aux_expre.topo --;
		aux_expre.livre = aux_expre.no[aux_expre.topo--].nome;
		goto l1;	/*continua com tipo do 2.o param */

	   case OP_3:
		
		j = ex_op3(pega_tipo(&aux_expre, aux_expre.topo-1),
			     pega_ptar(&aux_expre, aux_expre.topo-1),
			     pega_tipo(&aux_expre, aux_expre.topo),
			     pega_ptar(&aux_expre, aux_expre.topo),
			     tipo, ptar);
		if (j == ERRO)
		         return ERRO;
		aux_expre.topo -= 2;
		aux_expre.livre = aux_expre.no[aux_expre.topo--].nome;
		goto l1;

	   case OP_POS2:
		
		j = ex_op_pos(NOME(e,i),
			     pega_tipo(&aux_expre, aux_expre.topo-1),
			     pega_ptar(&aux_expre, aux_expre.topo-1),
			     pega_tipo(&aux_expre, aux_expre.topo),
			     pega_ptar(&aux_expre, aux_expre.topo),
			     tipo, ptar);
		if (j == ERRO)
		         return ERRO;
		aux_expre.topo --;
		aux_expre.livre = aux_expre.no[aux_expre.topo--].nome;
		goto l1;



	 }
      }
      return OK;
}


char	*pega_ptar(e, i)
EXPRE_POS	*e;
int	i;
{
char	*p;

   p = NOME(e,i);
   return (p+strlen(p)+1);
}

char	*pega_tipo(e, i)
EXPRE_POS	*e;
int	i;
{
   return (NOME(e,i));
}




/**************************************************************************
Rotinas para validar as operacoes
**************************************************************************/

ex_op1_arit(op, t1, p1, tipo, ptar)
char	*op, *t1, *p1, *tipo, *ptar;
{


   strcpy(tipo, t1);
   strcpy(ptar, p1);

   if (e_escalar(t1, p1))
	     return OK;

   if (strcmp(op, "-") == 0 || strcmp(op, "+") == 0)
	     return ERRO;

   if ( e_pointer(t1, p1))
	     return OK;

   return ERRO;
}



ex_op1_deref(t1, p1, tipo, ptar)
char	*t1, *p1, *tipo, *ptar;
{
int	k;

   if (! e_pointer(t1, p1) && !e_array(t1, p1) )
	     return ERRO;

   strcpy(tipo, t1);
   strcpy(ptar, p1);
   k = strlen(ptar);
   ptar[k-1] = '\0';
   return OK;
}


ex_op2_bit(t1, p1, t2, p2, tipo, ptar)
char	*t1, *p1, *t2, *p2, *tipo, *ptar;
{
   if (! e_inteiro(t1, p1) ||
       ! e_inteiro(t2, p2)  )
	     return ERRO;
   strcpy(tipo, t1);
   strcpy(ptar, p1);
   return OK;
}



ex_op2_log(t1, p1, t2, p2, tipo, ptar)
char	*t1, *p1, *t2, *p2, *tipo, *ptar;
{
   if (e_struct(t1, p1) || e_struct(t2, p2)  )
	     return ERRO;

   strcpy(tipo, "int");
   strcpy(ptar, "");
   return OK;
}



ex_ass_plain(t1, p1, t2, p2, tipo, ptar)
char	*t1, *p1, *t2, *p2, *tipo, *ptar;
{
   strcpy(tipo, t1);
   strcpy(ptar, p1);

   if (e_escalar(t1, p1) && e_escalar(t2, p2))
	     return OK;

   if (e_pointer(t1, p1) && (e_pointer(t2,p2) || e_array(t2,p2)))
	     return OK;


  if (e_struct(t1, p1) && compara_tipos(t1, p1, t2, p2))
	     return OK;

   return ERRO;

}



ex_op2_arit(op, t1, p1, t2, p2, tipo, ptar)
char	*op, *t1, *p1, *t2, *p2, *tipo, *ptar;
{
   if (e_struct(t1, p1) || e_struct(t2, p2)  )
	     return ERRO;

   strcpy(tipo, t1);
   strcpy(ptar, p1);

   if (op[0] == '%')
	     {
		if (e_inteiro(t1, p1) && e_inteiro(t2, p2))
			     return OK;
			else return ERRO;
	      }
	else {
		if (e_escalar(t1, p1) && e_escalar(t2, p2))
			     
		if (e_inteiro(t2, p2))
			     return OK;
			else {
				strcpy(tipo, t2);
				strcpy(ptar, p2);
				return OK;
			     }
	      }

   if ( op[0] != '-' && op[0] != '+')
	     return ERRO;

   if (op[0] == '+')
	     {
		if ((e_pointer(t1, p1)|| e_array(t1, p1)) && e_inteiro(t2, p2))
			     return OK;
		if ((e_pointer(t2, p2)|| e_array(t2, p2)) && e_inteiro(t1, p1))
			     {
				strcpy(tipo, t2);
				strcpy(ptar, p2);
				return OK;
			     }
	      }

   	else {
		if ((e_pointer(t1, p1)|| e_array(t1, p1)) && e_inteiro(t2, p2))
			     return OK;
		if ((e_pointer(t2, p2)|| e_array(t2, p2)) && 
		    (e_pointer(t1, p1)|| e_array(t1, p1)) &&
		    (compara_tipos_sub(t1, p1, t2, p2)) )
			     {
				strcpy(tipo,"int");
				strcpy(ptar, "");
				return OK;
			     }
	      }
	
   return ERRO;
}




ex_op3(t1, p1, t2, p2, tipo, ptar)
char	*t1, *p1, *t2, *p2, *tipo, *ptar;
{
   strcpy(tipo, t1);
   strcpy(ptar, p1);

   if (e_escalar(t1, p1) && e_escalar(t2, p2) && ! e_inteiro(t2, p2))
	     {
		strcpy(tipo, t2);
		strcpy(ptar, p2);
	      }

   return OK;
}




ex_op_pos(op, t1, p1, t2, p2, tipo, ptar)
char	*op, *t1, *p1, *t2, *p2, *tipo, *ptar;
{
int k;

   strcpy(tipo, t1);
   strcpy(ptar, p1);
   k = strlen(ptar);

   if (strcmp(op, ")") == 0)
	     {
		if (! e_funcao(t1, p1))
		   return ERRO;
		ptar[k-1] = '\0';
		return OK;
	      }

   if (strcmp(op, "]") == 0)
	     {
		if ((! e_array(t1, p1) && ! e_pointer(t1, p1)) || ! e_inteiro(t2, p2))
			     return ERRO;
		ptar[k-1] = '\0';
		return OK;
	      }


   if (strcmp(op, "->") == 0)
	     {
		if (! e_pointer(t1, p1) && !e_array(t1, p1))
			     return ERRO;

		ptar[k-1] = '\0';
	     }
   if (! e_struct(tipo, ptar))
	     return ERRO;

   return get_tipo_campo(t1, t2, tipo, ptar);

}

