# Copyright (C) 2012 -- Marcio Eduardo Delamaro and Jose Carlos Maldonado and Elisa Yumi Nakagawa
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
#


set VERSION "PROTEUM - PROgram TEsting Using Mutants - V 2.0"

set maxtcase 10;        # Max number of test cases # OBS: PRECISA MESMO DESSA VARIAVEL???

set MAXFUNCTIONS 0;
set MAXTCASES    0;

# Set used font in output labels

if { [expr $demo == 0] } {
# Fonts to labes-pnud
set f1 "helvetica 12"
set f2 "helvetica 12 bold "
#set FIXEDFONT "fixed 12"
set FIXEDFONT "fixed 10"
} else {
# Large fontes
set f1 "helvetica 16 bold"
set f2 "helvetica 16 bold"
set FIXEDFONT "fixed 14 bold"
#set FIXEDFONT "Times 14 bold"
}


# Sets fonts
option add *Label.Font	     $f2
option add *Entry.Font       $f1
option add *Menu.Font        $f1
option add *Menubutton.Font  $f1
option add *Button.Font      $f2
option add *Checkbutton.Font $f1
option add *Radiobutton.Font $f1
option add *Text.Font        $f1
option add *Listbox.Font     $f1


option add *Scrollbar.Width 12


#--------------- Globals Variables -----------------#

 
set test(dir)      {};     # directory
set test(tfile)    {};     # program test
set test(src)      {};     # source file
set test(exec)     {};     # executable file
set test(comp)     {};     # compilation command
set test(type)     {};     # test type (test or research)
set test(func)     {};     # functions for test (all or select)
set test(anom)     {};     # anomalous
 
set muta(status0)  {};     # Status of mutant 
set muta(status1)  {};     # Status of mutant 
set muta(dsize)    {};     # Descriptor size
set muta(begin)    {};     # Where the function start
set muta(node)     {};     # Program graph node
set muta(tcase)    {};     # Last used test case 
set muta(num_op)   {};     # Number of operator that generate this mutant
set muta(name_op)  {};     # Name of operator that generate this mutant
set muta(d_init0)  {};
set muta(d_init1)  {};     # Mutation Descriptor
set muta(d_init2)  {};


set tcase(status)  {};     # Test case status (enable/disable)
set tcase(param)   {};     # Parameter
set tcase(exec)    {};     # Execution Time (1/100 Sec)
set tcase(rcode)   {};     # Return code
set tcase(insize)  {};     # Input Size
set tcase(outsize) {};     # Output Size
set tcase(stdsize) {};     # Error Size
set tcase(in)      {};     # Input
set tcase(out)     {};     # Output
set tcase(err)     {};     # Error

set in(dir)        {}
set in(tfile)      {}
set in(src)        {}
set in(exec)       {};     # Variables for entry datas
set in(comp)       {}
set in(type)       {}
set in(anom)       {}
set in(tout)       {}
set in(from)       {}
set in(to)         {}

set out(totmut)	{}
set out(anomut) {}
set out(actmut) {}
set out(alimut) {}
set out(livmut) {}
set out(equmut) {}
set out(score)  {}



set ddirectory     {};     # default directory
set timeout        {};     # timeout for mutation execution



# Number of mutation operators for each class
set NGROUP1 24
set NGROUP2  9

set NOPERA   46
set NSTAT    15
set NCONST    3
set NVAR     11

set group1_data {{CovAllEdg - Coverage of all edges}			   \
	         {CovAllNod - Coverage of all nodes}                       \
	         {DirVarAriNeg - Inserts arithmetic negation at interface variable}      \
	         {DirVarBitNeg - Inserts bit negation at interface variable}             \
	         {DirVarIncDec - Inserts/removes ++ and -- at interface variable}     \
	         {DirVarLogNeg - Inserts logical negation at interface variable}         \
	         {DirVarRepCon - Replaces interface variable by set C}      \
	         {DirVarRepExt - Replaces interface variable by set E}      \
	         {DirVarRepGlo - Replaces interface variable by set G}      \
	         {DirVarRepLoc - Replaces interface variable by set L}      \
		 {DirVarRepPar - Replaces interface variable by set P}      \
	         {DirVarRepReq - Replaces interface variable by set R}      \
	         {IndVarAriNeg - Inserts arithmetic negation at non interface variable}  \
	         {IndVarBitNeg - Inserts bit negation at non interface variable}         \
	         {IndVarIncDec - Inserts/removes ++ and -- at non interface variable} \
	         {IndVarLogNeg - Inserts logical negation at non interface variable}     \
	         {IndVarRepCon - Replaces non interface variable by set C}  \
	         {IndVarRepExt - Replaces non interface variable by set E}  \
	         {IndVarRepGlo - Replaces non interface variable by set G}  \
	         {IndVarRepLoc - Replaces non interface variable by set L}  \
	         {IndVarRepPar - Replaces non interface variable by set P}  \
	         {IndVarRepReq - Replaces non interface variable by set R}  \
	         {RetStaDel - Deletes return statement}                    \
	         {RetStaRep - Replaces return statement}                   }

set group2_data {{ArgAriNeg - Inserts arithmetic negation at arguments} \
	         {ArgBitNeg - Inserts bit negation at arguments}        \
   	         {ArgDel - Remove argument}                             \
	         {ArgIncDec - Argument Increment and Decrement}         \
   	         {ArgLogNeg - Inserts logical negation at arguments}    \
		 {ArgRepReq - Replaces arguments by set R}              \
	         {ArgStcAli - Switches arguments of compatible type}    \
	         {ArgStcDif - Switches arguments of non compatible type}    \
	         {FunCalDel - Removes function call} }

set oper_data { {OAAA - Arithmetic Assignment Mutation} \
{OAAN - Arithmetic Operator Mutation} \
{OABA - Arithmetic Assignment by Bitwise Assignment} \
{OABN - Arithmetic by Bitwise Operator} \
{OAEA - Arithmetic Assignment by Plain Assignment} \
{OALN - Arithmetic Operator by Logical Operator} \
{OARN - Arithmetic Operator by Relational Operator} \
{OASA - Arithmetic Assignment by Shift Assignment} \
{OASN - Arithmetic Operator by Shift Operator} \
{OBAA - Bitwise Assignment by Arithmetic Assignment} \
{OBAN - Bitwise Operator by Arithmetic Assignment} \
{OBBA - Bitwise Assignment Mutation} \
{OBBN - Bitwise Operator Mutation} \
{OBEA - Bitwise Assignment by Plain Assignment} \
{OBLN - Bitwise Operator by Logical Operator} \
{OBNG - Bitwise Negation} \
{OBRN - Bitwise Operator by Relational Operator} \
{OBSA - Bitwise Assignment by Shift Assignment} \
{OBSN - Bitwise Operator by Shift Operator} \
{OCNG - Logical Context Negation} \
{OCOR - Cast Operator by Cast Operator} \
{OEAA - Plain assignment by Arithmetic Assignment} \
{OEBA - Plain assignment by Bitwise Assignment} \
{OESA - Plain assignment by Shift Assignment} \
{Oido - Increment/Decrement Mutation} \
{OIPM - Indirection Operator Precedence Mutation} \
{OLAN - Logical Operator by Arithmetic Operator} \
{OLBN - Logical Operator by Bitwise Operator} \
{OLLN - Logical Operator Mutation} \
{OLNG - Logical Negation} \
{OLRN - Logical Operator by Relational Operator} \
{OLSN - Logical Operator by Shift Operator} \
{ORAN - Relational Operator by Arithmetic Operator} \
{ORBN - Relational Operator by Bitwise Operator} \
{ORLN - Relational Operator by Logical Operator} \
{ORRN - Relational Operator Mutation} \
{ORSN - Relational Operator by Shift Operator} \
{OSAA - Shift Assignment by Arithmetic Assignment} \
{OSAN - Shift Operator by Arithmetic Operator} \
{OSBA - Shift Assignment by Bitwise Assignment} \
{OSBN - Shift Operator by Bitwise  Operator} \
{OSEA - Shift Assignment by Plain Assignment} \
{OSLN - Shift Operator by Logical Operator} \
{OSRN - Shift Operator by Relational Operator} \
{OSSA - Shift Assignment Mutation} \
{OSSN - Shift Operator Mutation}}

set stat_data {
{SBRC - break Replacement by continue} \
{SBRn - break Out to Nth Level} \
{SCRB - continue Replacement by break} \
{SCRn - continue Out to Nth Level} \
{SDWD - do-while Replacement by while} \
{SGLR - goto Label Replacement} \
{SMTC - n-trip continue} \
{SMTT - n-trip trap} \
{SMVB - Move Brace Up and Down} \
{SRSR - return Replacement} \
{SSDL - Statement Deletion} \
{SSWM - switch Statement Mutation} \
{STRI - Trap on if Condition} \
{STRP - Trap on Statement Execution} \
{SWDD - while Replacement by do-while} \
}

set var_data {
{VDTR - Domain Traps} \
{VGAR - Mutate Global Array References } \
{VGPR - Mutate Global Pointer References } \
{VGSR - Mutate Global Scalar References } \
{VGTR - Mutate Global Structure References } \
{VLAR - Mutate Local Array References } \
{VLPR - Mutate Local Pointer References } \
{VLSR - Mutate Local Scalar References } \
{VLTR - Mutate Local Structure References } \
{VSCR - Stucture Component Replacement} \
{VTWD - Twiddle Mutations} \
}

set const_data {
{Cccr - Constant for Constant Replacement } \
{Ccsr - Constant for Scalar Replacement} \
{CRCR - Required Constant Replacement} \
}
