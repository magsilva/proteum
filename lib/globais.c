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


/*#######################################################################
			G L O B A I S . C

	Variaveis utilizadas em varios programas


##########################################################################*/
#include	<stdlib.h>
#include	<string.h>

#include	"gerais.h"

char	*VERSAO = "V 1.1 - IM";

/* Versao:
	1.2 - Initial version with separated programs.
	1.2.1-	Was included in the test case file, a field for the
		total execution time (totaltempo) in addition to already
		existent field for cpu usage time (exetempo).
	1.2.2-  The lenght of maximum parameter for a test case was raisen
		to 255 instead of 60. A flag 'error' was included in
		the test case register.
		Program LI was completly replaced. Other programs
		thatt use the li as a library were also redone.
	1.2.3 - Changes to allow a program test to be incrmentally executed
		itt means, a lot of mutants each time.

	1.3.1 - The way of executing mutants was changed. Also the layout
		of tcase files.

	1.4.1 - Mutation file was changed. Now it is built using a B-Tree
		index approach.

	1.0 IM - Interface mutation

	1.1 IM - Includes trace of test cases to avoid mutatn execution

	1.1.2 IM - In this version the selection of N% of mutants keeps
		active the same mutants generated using N%

	2.0 IM   - Proteum and Proteum/IM in the same tool!!!

*/

/* -----------------------------------------------------------------------
   Tabela de classes e operadores de mutacao			        */

OPERADOR_MUTACAO g_tab_operador[] = 
{	
"I-CovAllEdg", "Coverage of Edges",
FALSE, 0, 0, 0, 0, NULL,
"I-CovAllNod", "Coverage of Nodes",
TRUE, 0, 0, 0, 0, NULL,
"I-DirVarAriNeg", "Inserts Arithmetic Negation at Interface Variables",
TRUE, 0, 0, 0, 0, NULL,
"I-DirVarBitNeg", "Inserts Bit Negation at Interface Variables",
TRUE, 0, 0, 0, 0, NULL,
"I-DirVarIncDec", "Increments and Decrements Interface Variables",
TRUE, 0, 0, 0, 0, NULL,
"I-DirVarLogNeg", "Inserts Logical Negation at Interface Variables",
TRUE, 0, 0, 0, 0, NULL,
"I-DirVarRepCon", "Replaces Interface Variables by Used Constants",
TRUE, 0, 0, 0, 0, NULL,
"I-DirVarRepExt", "Replaces Interface Variables by Extern Globals",
TRUE, 0, 0, 0, 0, NULL,
"I-DirVarRepGlo", "Replaces Interface Variables by Global Variables",
TRUE, 0, 0, 0, 0, NULL,
"I-DirVarRepLoc", "Replaces Interface Variables by Local Variables",
TRUE, 0, 0, 0, 0, NULL,
"I-DirVarRepPar", "Replaces Interface Variables by Formal Parameters",
TRUE, 0, 0, 0, 0, NULL,
"I-DirVarRepReq", "Replaces Interface Variables by Required Constants",
TRUE, 0, 0, 0, 0, NULL,
"I-IndVarAriNeg", "Inserts Arithmetic Negation at Non Interface Variables",
TRUE, 0, 0, 0, 0, NULL,
"I-IndVarBitNeg", "Inserts Bit Negation at Non Interface Variables",
TRUE, 0, 0, 0, 0, NULL,
"I-IndVarIncDec", "Increments and Decrements Non Interface Variables",
TRUE, 0, 0, 0, 0, NULL,
"I-IndVarLogNeg", "Inserts Logical Negation at Non Interface Variables",
TRUE, 0, 0, 0, 0, NULL,
"I-IndVarRepCon", "Replaces Non Interface Variables by Used Constants",
TRUE, 0, 0, 0, 0, NULL,
"I-IndVarRepExt", "Replaces Non Interface Variables by Extern Globals",
TRUE, 0, 0, 0, 0, NULL,
"I-IndVarRepGlo", "Replaces Non Interface Variables by Global Variables",
TRUE, 0, 0, 0, 0, NULL,
"I-IndVarRepLoc", "Replaces Non Interface Variables by Local Variables",
TRUE, 0, 0, 0, 0, NULL,
"I-IndVarRepPar", "Replaces Non Interface Variables by Formal Parameters",
TRUE, 0, 0, 0, 0, NULL,
"I-IndVarRepReq", "Replaces Non Interface Variables by Required Constants",
TRUE, 0, 0, 0, 0, NULL,
"I-RetStaDel", "Deletes return Statment",
TRUE, 0, 0, 0, 0, NULL,
"I-RetStaRep", "Replaces return Statment",
TRUE, 0, 0, 0, 0, NULL,

"II-ArgAriNeg", "Insert Arithmetic Negation on Argument",
TRUE, 0, 0, 0, 0, NULL,
"II-ArgBitNeg", "Insert Bit Negation on Argument",
TRUE, 0, 0, 0, 0, NULL,
"II-ArgDel", "Argument Deletion",
TRUE, 0, 0, 0, 0, NULL,
"II-ArgIncDec", "Argument Increment and Decrement",
TRUE, 0, 0, 0, 0, NULL,
"II-ArgLogNeg", "Insert Logical Negation on Argument",
TRUE, 0, 0, 0, 0, NULL,
"II-ArgRepReq","Argument Replacement by Required Constants", 
TRUE, 0, 0, 0, 0, NULL,
"II-ArgStcAli","Switch Compatible Arguments",
TRUE, 0, 0, 0, 0, NULL,
"II-ArgStcDif","Switch Non-compatible Arguments",
TRUE, 0, 0, 0, 0, NULL,
"II-FunCalDel", "Remove Function Call",
TRUE, 0, 0, 0, 0, NULL,

"u-Cccr","Constant for Constant Replacement", 
TRUE, 0, 0, 0, 0, NULL,
"u-Ccsr","Constant for Scalar Replacement ", 
TRUE, 0, 0, 0, 0, NULL,
"u-CRCR","Required Constant Replacement", 
TRUE, 0, 0, 0, 0, NULL,
"u-OAAA","Arithmetic Assignment Mutation", 
TRUE, 0, 0, 0, 0, NULL,
"u-OAAN","Arithmetic Operator Mutation", 
TRUE, 0, 0, 0, 0, NULL,
"u-OABA","Arithmetic Assignment by Bitwise Assignment ", 
TRUE, 0, 0, 0, 0, NULL,
"u-OABN","Arithmetic by Bitwise Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OAEA","Arithmetic Assignment by Plain Assignment", 
TRUE, 0, 0, 0, 0, NULL,
"u-OALN","Arithmetic Operator by Logical Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OARN","Arithmetic Operator by Relational Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OASA","Arithmetic Assignment by Shift Assignment", 
TRUE, 0, 0, 0, 0, NULL,
"u-OASN","Arithmetic Operator by Shift Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OBAA","Bitwise Assignment by Arithmetic Assignment", 
TRUE, 0, 0, 0, 0, NULL,
"u-OBAN","Bitwise Operator by Arithmetic Assignment", 
TRUE, 0, 0, 0, 0, NULL,
"u-OBBA","Bitwise Assignment Mutation", 
TRUE, 0, 0, 0, 0, NULL,
"u-OBBN","Bitwise Operator Mutation", 
TRUE, 0, 0, 0, 0, NULL,
"u-OBEA","Bitwise Assignment by Plain Assignment", 
TRUE, 0, 0, 0, 0, NULL,
"u-OBLN","Bitwise Operator by Logical Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OBNG","Bitwise Negation", 
TRUE, 0, 0, 0, 0, NULL,
"u-OBRN","Bitwise Operator by Relational Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OBSA","Bitwise Assignment by Shift Assignment", 
TRUE, 0, 0, 0, 0, NULL,
"u-OBSN","Bitwise Operator by Shift Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OCNG","Logical Context Negation", 
TRUE, 0, 0, 0, 0, NULL,
"u-OCOR","Cast Operator by Cast Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OEAA","Plain assignment by Arithmetic Assignment", 
TRUE, 0, 0, 0, 0, NULL,
"u-OEBA","Plain assignment by Bitwise Assignment", 
TRUE, 0, 0, 0, 0, NULL,
"u-OESA","Plain assignment by Shift Assignment", 
TRUE, 0, 0, 0, 0, NULL,
"u-Oido","Increment/Decrement Mutation", 
TRUE, 0, 0, 0, 0, NULL,
"u-OIPM","Indirection Operator Precedence Mutation", 
TRUE, 0, 0, 0, 0, NULL,
"u-OLAN","Logical Operator by Arithmetic Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OLBN","Logical Operator by Bitwise Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OLLN","Logical Operator Mutation", 
TRUE, 0, 0, 0, 0, NULL,
"u-OLNG","Logical Negation", 
TRUE, 0, 0, 0, 0, NULL,
"u-OLRN","Logical Operator by Relational Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OLSN","Logical Operator by Shift Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-ORAN","Relational Operator by Arithmetic Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-ORBN","Relational Operator by Bitwise Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-ORLN","Relational Operator by Logical Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-ORRN","Relational Operator Mutation", 
TRUE, 0, 0, 0, 0, NULL,
"u-ORSN","Relational Operator by Shift Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OSAA","Shift Assignment by Arithmetic Assignment", 
TRUE, 0, 0, 0, 0, NULL,
"u-OSAN","Shift Operator by Arithmetic Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OSBA","Shift Assignment by Bitwise Assignment",
TRUE, 0, 0, 0, 0, NULL,
"u-OSBN","Shift Operator by Bitwise  Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OSEA","Shift Assignment by Plain Assignment", 
TRUE, 0, 0, 0, 0, NULL,
"u-OSLN","Shift Operator by Logical Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OSRN","Shift Operator by Relational Operator", 
TRUE, 0, 0, 0, 0, NULL,
"u-OSSA","Shift Assignment Mutation", 
TRUE, 0, 0, 0, 0, NULL,
"u-OSSN","Shift Operator Mutation", 
TRUE, 0, 0, 0, 0, NULL,
"u-SBRC","break Replacement by continue", 
TRUE, 0, 0, 0, 0, NULL,
"u-SBRn","break Out to Nth Level", 
FALSE, 0, 0, 0, 0, NULL,
"u-SCRB","continue Replacement by break", 
TRUE, 0, 0, 0, 0, NULL,
"u-SCRn","continue Out to Nth Level", 
FALSE, 0, 0, 0, 0, NULL,
"u-SDWD","do-while Replacement by while", 
FALSE, 0, 0, 0, 0, NULL,
"u-SGLR","goto Label Replacement", 
TRUE, 0, 0, 0, 0, NULL,
"u-SMTC","n-trip continue", 
FALSE, 0, 0, 0, 0, NULL,
"u-SMTT","n-trip trap", 
FALSE, 0, 0, 0, 0, NULL,
"u-SMVB","Move Brace Up and Down", 
FALSE, 0, 0, 0, 0, NULL,
"u-SRSR","return Replacement", 
TRUE, 0, 0, 0, 0, NULL,
"u-SSDL","Statement Deletion", 
FALSE, 0, 0, 0, 0, NULL,
"u-SSWM","switch Statement Mutation", 
FALSE, 0, 0, 0, 0, NULL,
"u-STRI","Trap on if Condition", 
TRUE, 0, 0, 0, 0, NULL,
"u-STRP","Trap on Statement Execution", 
FALSE, 0, 0, 0, 0, NULL,
"u-SWDD","while Replacement by do-while", 
FALSE, 0, 0, 0, 0, NULL,
"u-VDTR","Domain Traps", 
TRUE, 0, 0, 0, 0, NULL,
"u-VGAR","Mutate Global Array References ", 
TRUE, 0, 0, 0, 0, NULL,
"u-VGPR","Mutate Global Pointer References ", 
TRUE, 0, 0, 0, 0, NULL,
"u-VGSR","Mutate Global Scalar References ", 
TRUE, 0, 0, 0, 0, NULL,
"u-VGTR","Mutate Global Structure References ", 
TRUE, 0, 0, 0, 0, NULL,
"u-VLAR","Mutate Local Array References ", 
TRUE, 0, 0, 0, 0, NULL,
"u-VLPR","Mutate Local Pointer References ", 
TRUE, 0, 0, 0, 0, NULL,
"u-VLSR","Mutate Local Scalar References ", 
TRUE, 0, 0, 0, 0, NULL,
"u-VLTR","Mutate Local Structure References ", 
TRUE, 0, 0, 0, 0, NULL,
"u-VSCR","Stucture Component Replacement", 
TRUE, 0, 0, 0, 0, NULL,
"u-VTWD","Twiddle Mutations", 
TRUE, 0, 0, 0, 0, NULL,

NULL, NULL, 0, 0, 0, 0, NULL
};


int	busca_op(x)
char	x[];
{
   return next_match(x, -1);
}


int	compara_op(x, y)
char	x[], y[];
{
int	i, k;

	k = strlen(x);
	if (y == NULL || k > strlen(y))
	   return 1;
	for (i = 0; i < k; i++)
	{
	   if (toupper(x[i]) == 'X')
		continue;
	   if (toupper(x[i]) <  toupper(y[i]))
		return -1;
	   if (toupper(x[i]) > toupper(y[i]))
		return 1;
	}
	return 0;
}

next_match(x, i)
char	x[];
int	i;
{
   while (i++ < NOPERADORES)
  	if (compara_op(x, g_tab_operador[i].nome) == 0)
	   return i;

   return -1;
}


#define         UNSLONG         (0x7fffffff)
long gera_rand(t)
long t;
{
   srand48(t);
   return lrand48();
}


epragerar(t, x)
unsigned long   t;
int     x;
{
unsigned long k;

   k = UNSLONG / 100 * x;
   return  (t < k);
}


 /*
#define         UNSLONG         (0x7fffffff/100*100)

unsigned long gera_rand(t)
unsigned long t;
{
   return (unsigned long) 1220703125 * t;
}


epragerar(t, x)
unsigned long	t;
int	x;
{
unsigned long k;

   k = (unsigned) -1;
   k = k / 100 * x;
   return  (t <= k);
}
*/

/**************************************************************************
SORTEIO:
        Decide, baseado numa porcentagem, se mutante deve ou nao ser
gerado.
Parametros:
        n: numero do operador de mutancao
Autor:
        Delamaro
***************************************************************************/
sorteio(n)
int     n;
{
unsigned long   t, j;
int     x;

    if ((x = g_tab_operador[n].percent) == 100)
          return TRUE;
    if (x == 0)
          return FALSE;

   j = UNSLONG / 100 * 100;
   t = g_tab_operador[n].semente;

   while ( t >= j)
   {
      t = gera_rand(t);
   }

    g_tab_operador[n].semente = gera_rand(t);
    return epragerar(t, x);
}

int op_rand_int(n, m)
int	n, m;
{
unsigned long t, j;
int i, k;

   j = UNSLONG / m * m;
   t = g_tab_operador[n].semente2;

   while ( t >= j)
   {
      t = gera_rand(t);
   }
   g_tab_operador[n].semente2 = gera_rand(t);
   return t % m;
}


