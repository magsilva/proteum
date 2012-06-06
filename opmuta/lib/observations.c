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


GERAL: A ProteumIM e a Proteum nao geravam mutantes para diversos 
operadores quando o comando possuia uma expressao com cast do tipo

xx = (void *) 0;

Esse erro foi corrigido na versao atual, assim o numero de mutantes
para determinado operador pode ser superior aas versoes anteriores.

---------------------------------------------------------------------------
u-Cccr

Operador u-Cccr substitui constante por constante. Assim como na versao 
1.4, strings nao sao substituidos. 

Na versao 1.4, nao eram feitas substituicoes do tipo

x[1] = b;     --->      x['a'] = b;

Na versao 2.0, essa alteracao eh feita (pelo menos a principio)
 
----------------------------------------------------------------------------
u-Ccsr

O mesmo comentario de u-Cccr vale aqui.

----------------------------------------------------------------------------
u-CRCR

O numero de mutantes gerados difere da Proteum1.4.1. Isso acontece porque
as constantes requiridas sao diferentes. Nessa versao foram incluidas 
constantes como __MAXINT__, __MAXCHAR__, etc.
Abaixo segue a lista de constantes das duas versoes:

Proteum1.4.1:
   set_add("0", int_const);
   set_add("1", int_const); 
   set_add("-1", int_const); 
   set_add("0.0", float_const);
   set_add("1.0", float_const); 
   set_add("-1.0", float_const); 
   set_add("0.1", float_const); 
   set_add("-0.1", float_const); 
   set_add("' '", char_const); 
   set_add("'\\0'", char_const); 
   set_add("'\\177'", char_const); 
   set_add("'\\200'", char_const); 
   set_add("'\\377'", char_const); 

ProteumIM 2.0:
char	*int_const = "0\0001\0(-1)\0__MAXINT__\0__MININT__\0";

char	*uint_const = "0\0001\0__MAXUINT__\0";

char	*long_const = "0\0001\0(-1)\0__MAXLONG__\0__MINLONG__\0";

char	*ulong_const = "0\0001\0__MAXULONG__\0";

char	*char_const = "'\\0'\0'\\1'\0((char)(-1))\0__MAXCHAR__\0__MINCHAR__\0";

char	*float_const = "0.0\0001.0\0(-1.0)\0(10E10)\0(10E-10)\0(-10E10)\0(-10E-10)\0";

----------------------------------------------------------------------------
u-OAAA, u-OAAN, u-OABA, u-OABN, u-OAEA, u-OALN, u-OARN, u-OASA, u-OASN, u-OBAA, u-OBAN, u-OBBA, u-OBBN, u-OBEA, u-OBLN, u-OBRN, u-OBSA, u-OBSN, u-OEAA, u-OEBA, u-OESA, u-OLAN, u-OLBN, u-OLLN, u-OLRN, u-OLSN, u-ORAN, u-ORBN, u-ORLN, u-ORRN, u-ORSN, u-OSAA, u-OSAN ,u-OSBA, u-OSBN, u-OSEA, u-OSLN, u-OSRN, u-OSSA, u-OSSN, U_OIDO Geram exatos os mesmos mutantes da Proteum 1.4.1

----------------------------------------------------------------------------
u-OIPM
A implementacao da versao 1.4.1 estava incorreta. A versao atual
corrige e implementa o operador como descrito no relatorio tecnico
de Purdue, com a unica diferenca que os unicos operadores tratados
sao ++ e -- Uma expressao como *x[p] nao eh tratada.

Na versao atual, a expressao ***x++ ira gerar

**(*x)++ e **(++(*x))
*(**x)++ e *(++(**x))
(***x)++ e ++(***x)

conforme especificado no relatorio.

-----------------------------------------------------------------------------
u-OCOR
Implementado igual aa versao 1.4.1

-----------------------------------------------------------------------------
OLNG e OBNG 
Implementado igual aa versao 1.4.1

-----------------------------------------------------------------------------
OCNG 
Implementado igual aa versao 1.4.1

-----------------------------------------------------------------------------

u-SSDL
Implementado igual aa versao 1.4.1

-----------------------------------------------------------------------------
u-STRP
Implementado igual aa versao 1.4.1

-----------------------------------------------------------------------------
u-SBRC e u-SCRB
Implementados iguais aa versao 1.4.1

-----------------------------------------------------------------------------
u-SRSR
Implementado igual aa versao 1.4.1

-----------------------------------------------------------------------------
u-VARR
O operador varr foi subdividido em dois operadores: U-VGAR e U-VLAR que
substituem por variaveis globais e locais, repectivamente.
Assim para gerar os mesmos mutantes de varr deve-se utilizar esses dois novos
operadores. (ver Tec. Report)

A ordem dos mutantes nao eh a mesma.
-----------------------------------------------------------------------------
u-VSRR
O operador vsrr foi subdividido em dois operadores: U-VGSR e U-VLSR que
substituem por variaveis globais e locais, repectivamente.
Assim para gerar os mesmos mutantes de vsrr deve-se utilizar esses dois novos
operadores. (ver Tec. Report)

A ordem dos mutantes nao eh a mesma. Foram mantidas as mesmas regras da 
Proteum para manter os mesmos mutantes gerados, inclusive algumas que
parecem absurdas, como por exemplo, trocar um referencia a uma variavel
char por uma variavel float ou double.
-----------------------------------------------------------------------------
u-VPRR
O operador varr foi subdividido em dois operadores: U-VGPR e U-VLPR que
substituem por variaveis globais e locais, repectivamente.
Assim para gerar os mesmos mutantes de vprr deve-se utilizar esses dois novos
operadores. (ver Tec. Report)

A ordem dos mutantes nao eh a mesma.
-----------------------------------------------------------------------------
u-VTRR
O operador varr foi subdividido em dois operadores: U-VGTR e U-VLTR que
substituem por variaveis globais e locais, repectivamente.
Assim para gerar os mesmos mutantes de vtrr deve-se utilizar esses dois novos
operadores. (ver Tec. Report)

A ordem dos mutantes nao eh a mesma.
-----------------------------------------------------------------------------
u-SWDD e u-SDWD
Implementados como na versao 1.4.1

-----------------------------------------------------------------------------
u-SMTC e u-SMTT
Implementados como na versao 1.4.1

-----------------------------------------------------------------------------
u-VSCR
Implementado como na versao 1.4.1

-----------------------------------------------------------------------------
u-VDTR u-VTWD
Implementados como na versao 1.4.1

-----------------------------------------------------------------------------
u-STRI
Implementados como na versao 1.4.1

-----------------------------------------------------------------------------
u-SGLR
Implementado como em 1.4.1

----------------------------------------------------------------------------
u_SBRn e u-SCRn
Implementados como em 1.4.1. Precisa muito teste.



Falta:
u-SSOM
