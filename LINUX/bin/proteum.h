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


#ifndef		_PROTEUM_


#define		_PROTEUM_

/*---------------- Unit Operator Defines ------------------------- */

#define		__TRAP__		kill(getpid(), 9)

#define		TRAP(x)			(__FlgEnableMutation__? __TRAP__: x)

#define		TRAP_ON_STAT(x)		if (__FlgEnableMutation__) __TRAP__ ; \
					else x

#define		TRAP_ON_TRUE(x)	(__FlgEnableMutation__ ? (x? __TRAP__: 0)\
							 : x)

#define         TRAP_ON_FALSE(x) (__FlgEnableMutation__ ? (x? 1: __TRAP__)\
                                                         : x)

#define		TRAP_ON_CASE()	 if (__FlgEnableMutation__) __TRAP__; \
				 __PROTEUM__LABEL__:;

#define		UTRAP_ON_STAT()		__TRAP__

#define		UTRAP_ON_TRUE(x)		((x)? __TRAP__: 0)

#define		UTRAP_ON_FALSE(x)	((x)? 1: __TRAP__)

#define		UTRAP_ON_CASE()		__TRAP__

#define		TRAP_ON_NEGATIVE(x)	((x) < 0? __TRAP__: (x))

#define		TRAP_ON_POSITIVE(x)	((x) > 0? __TRAP__: (x))

#define		TRAP_ON_ZERO(x)		((x) == 0? __TRAP__: (x))

#define		PRED(x)			(x-1)

#define		SUCC(x)			(x+1)

#define		TRAP_AFTER_N_INTER(x)	{ if( _PROTEUM_LOCAL_VAR_++ >= (x)) __TRAP__; }

#define		FALSE_AFTER_N_INTER(x)	{ if( _PROTEUM_LOCAL_VAR_++ > (x)) continue;}

#define		BREAK_OUT_TO_N_LEVEL(x)		goto x

#define		CONTINUE_OUT_TO_N_LEVEL(x)	goto x

#define		_NUMERO_MUTANTE		atoi((char *) getenv("MUTANTTOEXEC"))

/*---------------- Interface Operator Defines ---------------------*/

#define		__MAXINT__		0x7fffffff

#define		__MININT__		0x80000000

#define		__MAXUINT__		0xffffffff

#define		__MAXLONG__		__MAXINT__

#define		__MINLONG__		__MININT__

#define		__MAXULONG__		__MAXUINT__

#define		__MAXCHAR__		'\177'

#define		__MINCHAR__		'\377'

#define		PREPARE_MUTA(x)		((__FlgEnableMutation__=1),(x))

#define		PREPARE2_MUTA(x)	((__FlgEnableMutation__=1)?(x):(x))

#define		UNPREPARE_MUTA		__FlgEnableMutation__ = 0

#define		IF_MUTA(x,y)		(__FlgEnableMutation__ ? (x) : (y))

#define		IF_MUTA2(x,y)		if (__FlgEnableMutation__) x else y

#define		IF_NOT_MUTA(x)		IF_MUTA2(;,x)


/*----------------- Flag used to enable connection mutation ------------*/

int	__FlgEnableMutation__;

#endif
