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


/*###########################################################################
			P O L O N E S A . C 

	Este modulo implementa as funcoes para construir e analisar 
expreesoes do C em notacao polonesa posfixa.

############################################################################*/


#include	"polonesa.h"

#include	<opmuta/lib/opmuta.h>
#include	<li/lib/li.h>
#include	<lib/gerais.h>





EXPRE_POS	expre_pos, aux_expre, aux_expre2;


static	char	bufaux[1024];



/*************************************************************************
ZERA_EXPRE:
	Inicializa uma expressao, deixando-a vazia;

**************************************************************************/
zera_expre(e)
EXPRE_POS	*e;
{

   e->topo = -1;
   memset(e->no, 0, sizeof(e->no));
   e->livre = 0;
   memset(e->buf, 0, sizeof(e->buf));
}


/************************************************************************
INS_EXPRE:
	Inclui um simbolo na expressao 

*************************************************************************/
ins_expre(e, s, k)
EXPRE_POS	*e;
char	s[];
TIPO_NO	k;
{
int	i, j;

   i = e->topo + 1;
   if (i >= LEN(e->no))
	     return;
   j = strlen(s);
   if (j + e->livre >= LEN(e->buf))
	     return;
   e->no[i].tipo = k;
   e->no[i].nome = e->livre;
   strcpy(&(e->buf[e->livre]), s);
   e->livre += j+1;
   e->topo = i;
}



sub_expre(e, k)
EXPRE_POS	*e;
TIPO_NO	k;
{
int	i;

   i = e->topo;
   e->no[i].tipo = k;
}



/************************************************************************
INS_EXPRE_APOS:
	Inclui um simbolo no meio de um aexpressao. 

Parameters:
EXPRE_POS e:	Postfix expression where the simbol will be inserted
char	s[]:	Name of the simbolo 
TIPO_NO k  :	Type of the simbol
int	n  :	Number of the no after what the simbol will be inserted
*************************************************************************/
ins_expre_apos(e, s, k, n)
EXPRE_POS	*e;
char	s[];
TIPO_NO	k;
int	n;
{
int	i, j;

   i = e->topo + 1;
   if (i >= LEN(e->no))
	     return;

   j = strlen(s);
   if (j + e->livre >= LEN(e->buf))
	     return;

   for (i = e->topo; i > n; i--)
	e->no[i+1] = e->no[i];
   e->topo++;

   i = n+1;
   e->no[i].tipo = k;
   e->no[i].nome = e->livre;
   strcpy(&(e->buf[e->livre]), s);
   e->livre += j+1;
}




/**************************************************************************
PRINT_EXPRE:
	Imprime expressao no formato posfixo

**************************************************************************/
print_expre(e)
EXPRE_POS	*e;
{
int	i;

   for (i = 0; i <= e->topo; i++)
	{
	   printf("%s  ", &(e->buf[e->no[i].nome]));
        }
   aux_expre2 = expre_pos;
   monta_in_expre(e, 0, e->topo);
   printf("\n%s\n", expre_pos.buf);
   expre_pos = aux_expre2;
}



/***********************************************************************
GERA_EXPRE:
	Chama analisador para montar polonesa de uma expressao.

Parametros:
	buf: endereco do buffer onde esta fonte.
Saida:
	Devolve na global expre_pos, a expressao.
***********************************************************************/
gera_expre(buf)
char	buf[];
{
char	c;
int	i, ret;

   i = strlen(buf);
   buf[i] = ' ';
   c = buf[i+1];
   buf[i+1] = '\0';
   if (mparser(EXPRESSION, buf))
	     ret = OK;
	else ret =  ERRO;
   buf[i] = '\0';
   buf[i+1] = c;
   return ret;
}


/**************************************************************************
COMPARA_EXPRE:
	Verifica se duas expressoes sao iguais

Parametros:
EXPRE_POS *e, *f:	Endereco das expressoes
***************************************************************************/
compara_expre(e,f)
EXPRE_POS	*e, *f;
{
int	i, j;

   for (i = j = 0; i <= e->topo && j <= f->topo; i++, j++)
     {
	while (e->no[i].tipo == NOP) i++;
	while (f->no[j].tipo == NOP) j++;
	if (e->no[i].tipo != f->no[j].tipo ||
	    strcmp(&(e->buf[e->no[i].nome]), &(f->buf[f->no[j].nome])))
	       break;
     }
   return (i > e->topo && j > f->topo);
}



/**************************************************************************
MONTA_IN_EXPRE:
	Monta expressao infixa a partir de expressao posfixa 

Parametros:
	e: endereco de expressao posfixa 
	k,l: inicio e final da (sub) expressa a avaliar.

Saida:
	expressao infixa montada na global expre_pos.buf 
***************************************************************************/
monta_in_expre(e, k, l)
EXPRE_POS	*e;
int	k,l;
{
int	i, m;
char	*c1, *c2, *p;
char	d;


   zera_expre(&aux_expre);
   for (i = k; i <= l; i++)
      {
	switch (e->no[i].tipo)
	{
	   case VARIAVEL:
	   case CAMPO:
	   case CONST_INT:
	   case CONST_ENUM:
	   case CONST_FLOAT:
	   case CONST_CHAR:
	   case STRING:
	   case OP_0_ARGUMENTO:
	   case SUB_EXPRE:
		ins_expre(&aux_expre, &(e->buf[e->no[i].nome]), e->no[i].tipo);
		break;
	   case OP_1_ARIT:
	   case OP_1_LOG:
	   case OP_1_BIT:
	   case OP_1_REF:
	   case OP_1_DEREF:
		strcpy(bufaux, "(");
	   	strcat(bufaux, NOME(e, i));
	 	strcat(bufaux, " ");
		strcat(bufaux, NOME((&aux_expre), aux_expre.topo));
		strcat(bufaux, ")");
		strcpy(NOME((&aux_expre), aux_expre.topo), bufaux);
		aux_expre.livre = aux_expre.no[aux_expre.topo].nome + 
						strlen(bufaux) + 1;
		aux_expre.no[aux_expre.topo].tipo = SUB_EXPRE;
		break;
	   case OP_1_SIZEOF:
	   	strcpy(bufaux, NOME(e, i));
		if (*(NOME((&aux_expre), aux_expre.topo)) != '(' ) 
			strcat(bufaux, "(");
		strcat(bufaux, NOME((&aux_expre), aux_expre.topo));
		if (*(NOME((&aux_expre), aux_expre.topo)) != '(' ) 
			strcat(bufaux, ")");
		strcpy(NOME((&aux_expre), aux_expre.topo), bufaux);
		aux_expre.livre = aux_expre.no[aux_expre.topo].nome + 
							strlen(bufaux) + 1;
		aux_expre.no[aux_expre.topo].tipo = SUB_EXPRE;
		break;
	   case OP_1_CAST:
		strcpy(bufaux, "((");
		m = 2;
		p = NOME(e, i);
		while (*p == '_' || isspace(*p) || isalnum(*p) )
			bufaux[m++] = *(p++);
		bufaux[m] = '\0';
		if ( *p != '\0' )
		   ptar_to_str(p, &bufaux[m]);

		strcat(bufaux, ") ");
		strcat(bufaux, NOME((&aux_expre), aux_expre.topo));
		strcat(bufaux, ") ");
		strcpy(NOME((&aux_expre), aux_expre.topo), bufaux);
		aux_expre.livre = aux_expre.no[aux_expre.topo].nome + 
							strlen(bufaux) + 1;
		aux_expre.no[aux_expre.topo].tipo = SUB_EXPRE;
		break;
	   case NOME_TIPO:
		strcpy(bufaux, "(");
	   	strcat(bufaux, NOME(e, i));
		strcat(bufaux, ") ");
		ins_expre(&aux_expre, bufaux, SUB_EXPRE);
		aux_expre.no[aux_expre.topo].tipo = SUB_EXPRE;
		break;
	   case OP_2_ARIT:
	   case OP_2_BIT:
	   case OP_2_LOG:
	   case OP_2_SHIFT:
	   case OP_2_RELA:
	   case OP_ASS_ARIT:
	   case OP_ASS_BIT:
	   case OP_ASS_PLAIN:
	   case OP_ASS_SHIFT:
		l2:
		strcpy(bufaux, "(");
		strcat(bufaux, NOME((&aux_expre), aux_expre.topo-1));
		strcat(bufaux, " ");
		strcat(bufaux, NOME(e, i));
		strcat(bufaux, " ");
		strcat(bufaux, NOME((&aux_expre), aux_expre.topo));
		strcat(bufaux, ")");
		aux_expre.topo--;
		strcpy(NOME((&aux_expre), aux_expre.topo), bufaux);
		aux_expre.livre = aux_expre.no[aux_expre.topo].nome + 
							strlen(bufaux) + 1;
		aux_expre.no[aux_expre.topo].tipo = SUB_EXPRE;
		break;
	   case OP_2_SEQ:
	   case OP_2_ARGUMENTO:
		strcpy(bufaux, NOME((&aux_expre), aux_expre.topo-1));
		strcat(bufaux, " ");
		strcat(bufaux, NOME(e, i));
		strcat(bufaux, " ");
		strcat(bufaux, NOME((&aux_expre), aux_expre.topo));
		aux_expre.topo--;
		strcpy(NOME((&aux_expre), aux_expre.topo), bufaux);
		aux_expre.livre = aux_expre.no[aux_expre.topo].nome + 
							strlen(bufaux) + 1;
		aux_expre.no[aux_expre.topo].tipo = SUB_EXPRE;
		break;
	   case OP_3:
		strcpy(bufaux, "(");
		strcat(bufaux, NOME((&aux_expre), aux_expre.topo-2));
		strcat(bufaux, " ? ");
		strcat(bufaux, NOME((&aux_expre), aux_expre.topo-1));
		strcat(bufaux, " : ");
		strcat(bufaux, NOME((&aux_expre), aux_expre.topo));
		strcat(bufaux, ")");
		aux_expre.topo -= 2;
		strcpy(NOME((&aux_expre), aux_expre.topo), bufaux);
		aux_expre.livre = aux_expre.no[aux_expre.topo].nome + 
							strlen(bufaux) + 1;
		aux_expre.no[aux_expre.topo].tipo = SUB_EXPRE;
		break;
	   case OP_POS1_ARIT:
		strcpy(bufaux, "(");
		strcat(bufaux, NOME((&aux_expre), aux_expre.topo));
	   	strcat(bufaux, NOME(e, i));
		strcat(bufaux, ")");
		strcpy(NOME((&aux_expre), aux_expre.topo), bufaux);
		aux_expre.livre = aux_expre.no[aux_expre.topo].nome +
							 strlen(bufaux) + 1;
		aux_expre.no[aux_expre.topo].tipo = SUB_EXPRE;
		break;
	   case OP_POS2:
		if (strcmp(NOME(e,i), ")") == 0)
		        {
			   c1 = "(";
			   c2 = ")";
			   l1:
			   strcpy(bufaux, NOME((&aux_expre), aux_expre.topo-1));
			   strcat(bufaux, c1);
			   strcat(bufaux, NOME((&aux_expre), aux_expre.topo));
			   strcat(bufaux, c2);
			   aux_expre.topo--;
			   strcpy(NOME((&aux_expre), aux_expre.topo), bufaux);
			   aux_expre.livre = aux_expre.no[aux_expre.topo].nome +
							strlen(bufaux) + 1;
			   aux_expre.no[aux_expre.topo].tipo = SUB_EXPRE;
			}
		    else 
		if (strcmp(NOME(e, i), "]") == 0)
		        {
			   c1 = "[";
			   c2 = "]";
			   goto l1;
			}
		else 
		{
                strcpy(bufaux, "(");
                strcat(bufaux, NOME((&aux_expre), aux_expre.topo-1));
                strcat(bufaux, NOME(e, i));
                strcat(bufaux, NOME((&aux_expre), aux_expre.topo));
                strcat(bufaux, ")");
                aux_expre.topo--;
                strcpy(NOME((&aux_expre), aux_expre.topo), bufaux);
                aux_expre.livre = aux_expre.no[aux_expre.topo].nome +
                                                        strlen(bufaux) + 1;
                aux_expre.no[aux_expre.topo].tipo = SUB_EXPRE;
		}
		break;
	   

	   case NOP:
		break;

	 }
      }
    expre_pos = aux_expre;
    if (expre_pos.buf[0] != '(')
    {
	strcpy(bufaux, "(");
	strcat(bufaux, expre_pos.buf);
	strcat(bufaux, ")");
	strcpy(expre_pos.buf, bufaux);
    }
}


/**************************************************************************
EXPRE_E_REFERENCIA:
Verifica se determina expressa e' uma referencia com
variavel: 	a
array:		a[i], a[i][j];
campo de struc:	a.x, a.x.y, a->x, a->y;
conteudo de apontador:	*a;
**************************************************************************/
expre_e_referencia(e, i)
int	i;
EXPRE_POS	*e;
{
int	j;
char	*p;

   j = e->no[i].tipo;
   p = NOME(e, i);
   if (j == VARIAVEL || j == OP_1_DEREF)
	     return TRUE;
   if (j == OP_POS2 && 
      (strcmp(p,"]") == 0 || strcmp(p,".") == 0 || strcmp(p, "->") == 0))
	     return TRUE;
   return FALSE;
}


int	root_of_ref(e, i)
int     i;
EXPRE_POS       *e;
{
int j;
char *p;

   switch(e->no[i].tipo)
   {
	case OP_1_DEREF:
	   j = i - 1;
	   break;
	case OP_POS2:
	   p = NOME(e, i);
	   if (strcmp(p,"]") == 0 || strcmp(p,".") == 0 || strcmp(p, "->") == 0)
	   {
		j = i-1;
		j = e->no[j].final - 1;
	   	break;
	   }
	default:
	   return i;
   }
   return root_of_ref(e, j);
}






/****************************************************************************
EXPRE_E_ESCALAR:
	Verifica se o tipo de um aexpressao e' escalar

****************************************************************************/
expre_e_escalar(e, i, tipo, ptar)
int	i;
EXPRE_POS	*e;
char	*tipo, *ptar;
{
int	t;

   t = e->no[i].final;	/* acha fim da sub_expressao */
   *tipo = *ptar = '\0';
   if (exec_tipo(e, t, i) == ERRO)
	     return FALSE;	/* calcula o tipo da sub expressao */
   t = strlen(aux_expre.buf) +1;
   strcpy(tipo, aux_expre.buf);
   strcpy(ptar, &aux_expre.buf[t]);
   return e_escalar(aux_expre.buf, &(aux_expre.buf[t]) );
}


/****************************************************************************
EXPRE_E_ARRAY:
	Verifica se o tipo de um aexpressao e' array

****************************************************************************/
expre_e_array(e, i, tipo, ptar)
int	i;
EXPRE_POS	*e;
char	*tipo, *ptar;
{
int	t;

   t = e->no[i].final;	/* acha fim da sub_expressao */
   *tipo = *ptar = '\0';
   if (exec_tipo(e, t, i) == ERRO)
	     return FALSE;	/* calcula o tipo da sub expressao */
   t = strlen(aux_expre.buf) +1;
   strcpy(tipo, aux_expre.buf);
   strcpy(ptar, &aux_expre.buf[t]);
   return e_array(aux_expre.buf, &(aux_expre.buf[t]) );
}



/****************************************************************************
EXPRE_E_POINTER:
	Verifica se o tipo de um aexpressao e' pointer

****************************************************************************/
expre_e_pointer(e, i, tipo, ptar)
int	i;
EXPRE_POS	*e;
char	*tipo, *ptar;
{
int	t;

   t = e->no[i].final;	/* acha fim da sub_expressao */
   *tipo = *ptar = '\0';
   if (exec_tipo(e, t, i) == ERRO)
	     return FALSE;	/* calcula o tipo da sub expressao */
   t = strlen(aux_expre.buf) +1;
   strcpy(tipo, aux_expre.buf);
   strcpy(ptar, &aux_expre.buf[t]);
    return e_pointer(aux_expre.buf, &(aux_expre.buf[t]) );
}




/****************************************************************************
EXPRE_E_ESTRUCT:
	Verifica se o tipo de um aexpressao e' estrutura

****************************************************************************/
expre_e_estruct(e, i, tipo, ptar)
int	i;
EXPRE_POS	*e;
char	*tipo, *ptar;
{
int	t;

   t = e->no[i].final;	/* acha fim da sub_expressao */
   *tipo = *ptar = '\0';
   if (exec_tipo(e, t, i) == ERRO)
	     return FALSE;	/* calcula o tipo da sub expressao */
   t = strlen(aux_expre.buf) +1;
   strcpy(tipo, aux_expre.buf);
   strcpy(ptar, &aux_expre.buf[t]);
    return e_struct(aux_expre.buf, &(aux_expre.buf[t]) );
}



/***************************************************************************
EXPRE_SUBSTITUI:
	Substitui uma sub expressao por um unico elemento (um no')
parametros:
	e: expressao pos fixa
	i: indice da sub_expre a ser substituida
	p: nome do no a substituir
	t: tipo do no

***************************************************************************/
expre_substitui(e, i, p, t)
EXPRE_POS	*e;
int	i;
char	*p;
TIPO_NO	t;
{
int	k, j, l, f;

   k = e->no[i].final;	/* acha fim da sub_expressao */
   for (j = k; j < i; j++)  /* acha tamanho da expressao (nome) */
	if (e->no[j+1].nome != e->no[j].nome+strlen(NOME(e,j))+1)
	         break;
   f = e->no[j].nome+strlen(NOME(e,j)); /* f <- final da sub expressao */
   j = e->no[k].nome;	/* j <- inicio da expressao */
   e->no[i].tipo = t;	/* coloca novo tipo */
   e->no[i].final = k;  /* final da sub expressao */
   if ((l = strlen(p)) > f -j)	/* coloca novo nome */
           {
		strcpy(&(e->buf[e->livre]), p);
		e->no[i].nome = e->livre;
		e->livre += l+1;
	    }
      else  {
		strcpy(&(e->buf[j]), p);
		e->no[i].nome = j;
		j += l+1;
	     }
   for ( ; k < i ; k++)	/* tira sub expressao antiga */
      {
	e->no[k].tipo = NOP;
	e->no[k].nome = j < f ? j++ : -1;
       }
}

   
/************************************************************************
EXPRE_COMPLETA:
	Completa expressao, acertando ponteiros par das sub expressoes
*************************************************************************/
expre_completa(e)
EXPRE_POS	*e;
{
    completa(e, e->topo);
}


completa(e, t)
EXPRE_POS	*e;
int	t;
{
int	i = t;

   switch (e->no[t].tipo)
     {
	   case VARIAVEL:
	   case CAMPO:
	   case CONST_INT:
	   case CONST_ENUM:
	   case CONST_FLOAT:
	   case CONST_CHAR:
	   case STRING:
	   case NOME_TIPO:
	   case OP_0_ARGUMENTO:
		while (e->no[i-1].tipo == NOP && i > 0)
		   i--;
		e->no[t].final = i;
		return i;		

	   case OP_1_ARIT:
	   case OP_1_LOG:
	   case OP_1_BIT:
	   case OP_1_REF:
	   case OP_1_DEREF:
	   case OP_1_SIZEOF:
	   case OP_POS1_ARIT:
	   case OP_1_CAST:
		e->no[t].final = i = completa(e, t-1);
		return i;
	   case OP_2_ARIT:
	   case OP_2_BIT:
	   case OP_2_LOG:
	   case OP_2_SHIFT:
	   case OP_POS2:
	   case OP_2_RELA:
	   case OP_2_SEQ:
	   case OP_2_ARGUMENTO:
	   case OP_ASS_ARIT:
	   case OP_ASS_BIT:
	   case OP_ASS_PLAIN:
	   case OP_ASS_SHIFT:
		i = completa(e, t-1);
		e->no[t].final = i = completa(e, i-1);
		return i;
	   case OP_3:
		i = completa(e, t-1);
		i = completa(e, i-1);
		e->no[t].final = i = completa(e, i-1);
		return i;
	   case SUB_EXPRE:	/* well, this should no be in the expression */
		break;
     }
}




expre_exec_tipo(e, k, l, tipo, ptar)
EXPRE_POS	*e;
int	k,l;
char	*tipo, *ptar;
{
int	t;

   *tipo = *ptar = '\0';
   if (exec_tipo(e, k, l) == ERRO)
	     return ERRO;
   t = strlen(aux_expre.buf) +1;
   strcpy(tipo, aux_expre.buf);
   strcpy(ptar, &aux_expre.buf[t]);
   return OK;
}




/*************************************************************************
EXPRE_VALIDA:
	Verifica se mutacao provocou mudanca invalida na expressao

**************************************************************************/
expre_valida(e)
EXPRE_POS	*e;
{
int	i, t;

   expre_completa(e);
   for (i = 0; i <= e->topo; i++)
      {
	switch (e->no[i].tipo)
	{
	   case OP_ASS_ARIT:	/* verifica se op da esquerda de uma */
	   case OP_ASS_BIT:	/* atribuicao e' uma referencia */
	   case OP_ASS_PLAIN:
	   case OP_ASS_SHIFT:
		t = e->no[i-1].final - 1;
		if (! expre_e_referencia(e, t))
			     return FALSE;
		break;
	   case OP_1_REF:
		l1:
		if (! expre_e_referencia(e, i-1))
			     return FALSE;
		break;
	   case OP_1_ARIT:
	   case OP_POS1_ARIT:
		if (strcmp(NOME(e,i),"++") == 0 || strcmp(NOME(e,i),"--") == 0)
		         goto l1;
		break;
	}
      }
   return (exec_tipo(e, 0, e->topo) != ERRO);
}


/***********************************************************************
E_PARAMETRO:
	Verifica se determinado elemento da expressao estah dentro
de uma lista de parametros de uma chamada de funcao.

***********************************************************************/
e_parametro(f, e, k)
EXPRE_POS	*e;
int	k;
char	*f;
{
int	i, j;

   for (i = k+1; i <= e->topo; i++)
   {
	j = e->no[i].final;
	if (e->no[i].tipo == OP_POS2 && j < k && e->no[j].tipo == VARIAVEL)
	   if ( *f == '\0' ||  strcmp(NOME(e, j), f) == 0)
	   	return TRUE;
   }
   return FALSE;
}


int	eh_const_expre(e , j, k)
EXPRE_POS       *e;
int     j, k;
{
  while (j <= k)
  {
	if (e->no[j].tipo != NOP && e->no[j].tipo != OP_1_CAST  && 
		! EH_CONST(e->no[j].tipo ))
	   return  FALSE;
	j++;
   }
   return TRUE;
}


int	get_nth_param(e, j, k)
EXPRE_POS       *e;
int     j, k;
{
int i, r;

   r = e->no[--j].final;
   switch (e->no[j].tipo)
   {
	case OP_0_ARGUMENTO:
	   return -1;
	case OP_2_ARGUMENTO:
	   j--; i = e->no[j].final-1;
	   while (i > r && e->no[i].tipo == OP_2_ARGUMENTO)
	   {
	     if (k == 1)
		return j;
	     j = i-1;
	     i = e->no[j].final-1;
	     k--;
	   }
	   if (k == 1)
		return j;
	   else
	   if (k == 2)
		return e->no[j].final - 1;
	   else
		return -1;
	default:
	   return (k == 1)? j: -1;
   }
}



int	get_max_param(e, j)
EXPRE_POS       *e;
int     j;
{
int	i, k;

   i = e->no[--j].final;
   if (e->no[j].tipo == OP_0_ARGUMENTO)
	return 0;
   if (e->no[j].tipo != OP_2_ARGUMENTO)
	return 1;
   j--; k = 0;
   while (j >= i)
   {
	j = e->no[j].final -1;
        if (e->no[j].tipo == OP_2_ARGUMENTO)
	   j--;
	k++;
   }
   return k;
}


expre_change_places(e, from, to)
EXPRE_POS *e;
int	to, from;
{
int	i, j, t, f;

   aux_expre = *e;
   if (from > to )
   {
	t = from;
	from = to;
	to = t;
   }
   f = e->no[from].final;
   t = e->no[to].final;

   for (i = from, j = to; i >= f; i--, j--)
   {
	e->no[j] = aux_expre.no[i];
   } 
   i = t-1;
   while (i > from)
   {
	e->no[j--] = aux_expre.no[i--];
   }
   i = to;
   while (i >= t)
   {
        e->no[j--] = aux_expre.no[i--];
   }
}

