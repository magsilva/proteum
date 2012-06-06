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


/*-----------------------------------------------------------------------------*/
/*   INSTRUM.C
/*   Functions to instrument source file.
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*---------------------------------------------------------------------------*/

#include	<string.h> 

#include	"lib/gerais.h"
#include	"tcase/lib/tcase.h"
#include	"li/lib/li.h"

     

#define	e_func(x)	(x[0] == '@')

#define	e_dcl(x)	(strcmp(x, "$DCL") == 0)

#define	e_label(x)	(strpbrk(x, "$@{}") == NULL)

#define	e_sequencial(x)	(strncmp(x, "$S", 2) == 0)

#define	e_if(x)		(strcmp(x, "$IF") == 0)

#define	e_else(x)	(strcmp(x, "$ELSE") == 0)

#define	e_cond(x)	(strncmp(x, "$C(", 3) == 0 || strcmp(x, "$CC") == 0 || strncmp(x, "$NC(", 4) == 0)

#define	e_case(x)	(strcmp(x, "$CASE") == 0)

#define	e_rotc(x)	(strcmp(x, "$ROTC") == 0 ||  strcmp(x, "$ROTD") == 0)

#define	e_while(x)	(strcmp(x, "$WHILE") == 0)

#define	e_for(x)	(strcmp(x, "$FOR") == 0)

#define	e_repeat(x)	(strcmp(x, "$REPEAT") == 0)

#define	e_until(x)	(strcmp(x, "$UNTIL") == 0)

#define	e_goto(x)	(strcmp(x, "$GOTO") == 0)

#define	e_break(x)	(strcmp(x, "$BREAK") == 0)

#define	e_continue(x)	(strcmp(x, "$CONTINUE") == 0)

#define	e_return(x)	(strcmp(x, "$RETURN") == 0)

#define	e_abre(x)	(strcmp(x, "{") == 0)

#define	e_fecha(x)	(strcmp(x, "}") == 0)

#define	e_EOF(x)	(x[0] == '\0')



char	symbol[TAMLEX+1];
long	offset;
int	node, size, line;

long	curfunction;
int	curnode;

char	bufaux[1024];

extern FILE *fp_source,     /* pointer to source file __<file>.c */
            *fp_li,         /* pointer to LI file __<file>.nli */
            *fp_instrum;    /* pointer to instrumented file __<file>_inst.c */

long last_offset = 1;

int 	id = 0;


/*-----------------------------------------------------------------------------*/
/*   GETSYMBOL ()
/*   Gets a symbol from LI file.
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/


void getsymbol()
{
int i;
int last_size;

   last_size = size;
   i = fscanf(fp_li, "%s %d %ld %d %d", symbol, &node, &offset, &size, &line);
   if ( i != 5)
   {
	symbol[0] = '\0';
	node = size = line = offset = -1;
/* 	copy_file(fp_source, last_offset-1, -1, fp_instrum); */
	fflush(fp_instrum);
        return;
   }
/*   if (! e_func(symbol) && offset != 0 )
   {
	copy_file(fp_source, last_offset-1, offset-last_offset, fp_instrum);
	fflush(fp_instrum);
	last_offset = offset;
   }
   if (offset == 0)
   {
	copy_file(fp_source, last_offset-1, last_size, fp_instrum);
	fflush(fp_instrum);
	last_offset += last_size;
   } */
}



/*-----------------------------------------------------------------------------*/
/*   WRITE_PP ()
/*   Writes a "ponta_de_prova ()" in a specified point of the file.
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/


void write_pp (long func, int node, char *str)
{
   fprintf (fp_instrum, "ponta_de_prova (%ld, %d)%s", func, node, str);
   fflush (fp_instrum);
}



/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

void put_str(char *str)
{
   fprintf(fp_instrum, "%s", str);
   fflush(fp_instrum);
}


/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

void put_sym()
{
char *p;

   if (size > 0)
   {
      	p = (char *) malloc(size+1);
   	posiciona(fp_source, offset-1);
   	learq(fp_source, p,  size);
   	p[size] = '\0';
	put_str(p);
	free(p);
   }
   else
   {
	p = symbol;
   	put_str(p);
   }
   put_str(" ");
   last_offset += size;
   size = 0;
}

/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

void sint_error()
{
   sprintf(bufaux, "Sintatic error in LI program\nLine: %d Statment: %s",
			line, symbol);
   msg(bufaux);
   exit(1);
}

/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

void indent()
{
int i;

   fprintf(fp_instrum, "\n");
   for (i = 0; i < id; i++) 
	fprintf(fp_instrum, "    ");
}

/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

msg(char *s)
{
   fprintf(stderr, "\ninstrum: %s\n", s);
}


/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

program()
{
   do
   {
      while (e_dcl(symbol))
	 declaration();

      if ( ! e_func(symbol) && ! e_EOF(symbol))
	  sint_error();
      if (e_func(symbol))
      	function();
   } while ( ! e_EOF(symbol) );
}


/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

function()
{
   curfunction = offset;
   getsymbol();
   while (e_dcl(symbol) )
	declaration();
   if ( ! e_abre(symbol) )
	sint_error();
   curnode = 0;
   comp_stat();
}

/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

comp_stat()
{
   indent();
   put_sym();
   getsymbol();
   while ( e_dcl(symbol) )
	declaration();
   if (node != curnode)
   {
	write_pp(curfunction, node, ";");
	curnode = node;
   }


   while ( ! e_fecha(symbol))
	statement();
   if (node != curnode)
   {
	write_pp(curfunction, node, ";");
	curnode = node;
   }
   indent();
   put_sym();
   getsymbol();
}


/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

statement()
{
   id++;
   if (e_EOF(symbol) )
	sint_error();
   else
   if (e_dcl(symbol) )
	declaration();
   else
   if (e_sequencial (symbol) || e_break (symbol) ||
       e_continue (symbol) || e_return (symbol))
	seq_stat ();
   else
   if (e_if (symbol))
	if_stat ();
   else
   if (e_abre (symbol))
	comp_stat ();
   else
   if ( e_case (symbol))
	switch_stat ();
   else
   if (e_while (symbol))
	while_stat ();
   else
   if (e_repeat (symbol))
	repeat_stat ();
   else
   if (e_rotc (symbol))
	rotc_stat ();
   else
   if (e_for (symbol))
	for_stat ();
   else
   if (e_goto (symbol))
	goto_stat ();
   else 
   if (e_label (symbol))
	label ();
   else
	sint_error();
   id--;
}


/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

declaration()
{
   indent();
   put_sym();
   getsymbol();
}


/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

seq_stat()
{
   if (node != curnode)
   {
	indent();
	write_pp(curfunction, node, ";");
	curnode = node;
   }
   indent();
   put_sym();
   getsymbol();
}


/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

if_stat()
{
int k;
   k = FALSE;
   indent();
   put_sym();
   if (node != curnode)
   {
	put_str(" ( ");
	write_pp(curfunction, node, ", " );
	curnode = node;
	k = TRUE;
   }
   getsymbol();
   if ( ! e_cond(symbol) )
	sint_error();

   condition();
   if (k) 
      put_str(")");

   k = e_abre(symbol);
   if (! k)
       put_str("{");
   statement();
   if (! k)
       put_str("}");

   if (e_else(symbol) )
   {
	indent();
	put_sym();
	getsymbol();
   	k = e_abre(symbol);
   	if (! k)
           put_str("{");
	statement();
   	if (! k)
            put_str("}");
   }
}


/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

switch_stat()
{
int k;
   k = FALSE;
   indent();
   put_sym();
   if (node != curnode)
   {
	put_str(" ( ");
	write_pp(curfunction, node, ", " );
	curnode = node;
	k = TRUE;
   }
   getsymbol();
   if ( ! e_cond(symbol) )
	sint_error();

   condition();
   if (k) 
      put_str(")");

   k = e_abre(symbol);
   if (! k)
       put_str("{");
   curnode = node;
   statement();
   if (! k)
       put_str("}");

   if (e_else(symbol) )
   {
	indent();
	put_sym();
	getsymbol();
   	k = e_abre(symbol);
   	if (! k)
           put_str("{");
	statement();
   	if (! k)
            put_str("}");
   }
}

/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

while_stat()
{
int k;
   k = FALSE;
   indent();
   put_sym();
   if (node != curnode)
   {
	put_str(" ( ");
	write_pp(curfunction, node, ", " );
	curnode = node;
	k = TRUE;
   }
   getsymbol();
   if ( ! e_cond(symbol) )
	sint_error();

   condition();
   if (k) 
      put_str(")");

   k = e_abre(symbol);
   if (! k)
       put_str("{");
   statement();
   if (! k)
       put_str("}");
}

/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

repeat_stat()
{
int k;

   indent();
   put_sym();
   getsymbol();


   k = e_abre(symbol);
   if (! k)
       put_str("{");
   statement();
   if (! k)
       put_str("}");

   if ( ! e_until(symbol) )
	sint_error();
   indent();
   put_sym();
   getsymbol();
   if ( ! e_cond(symbol) )
	sint_error();
   if ( node != curnode )
   {
      	put_str(" ( ");
      	write_pp(curfunction, node, ", ");
   	curnode = node;
	size--;
	put_sym();
	put_str(" );");
   }
   else
	put_sym();
   getsymbol();
}

/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

for_stat()
{
int k;

   indent();
   put_sym();
   getsymbol();
   if ( ! e_sequencial(symbol) )
	sint_error();
   put_str("( ");
   if ( curnode != node )
   {
	if ( size > 1)
           write_pp(curfunction, node, ", ");
	else
	   write_pp(curfunction, node, "");
	curnode = node;
   }
   put_sym();
   getsymbol();
   if ( ! e_cond(symbol) )
	sint_error();
   if ( size == 0)
   {
	write_pp(curfunction, node, ", 1");
	put_str("; ");
   }
   else
   {
	write_pp (curfunction, node, ", ");
	put_sym ();
   }
   curnode = node;
   getsymbol ();
   if ( size == 0)
	write_pp (curfunction, node, "");
   else
   {
	write_pp (curfunction, node, ", ");
	put_sym ();
   }
   curnode = node;
   put_str (" )");
   getsymbol ();

   k = e_abre(symbol);
   if (! k)
       put_str("{");
   statement();
   if (! k)
       put_str("}");
}
   

/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/27/96
/*   last atualization: 08/27/96
/*-----------------------------------------------------------------------------*/

goto_stat ()
{
   indent ();
   if (curnode != node) {
      write_pp (curfunction, node, ";");
      curnode = node;
   }
   put_sym ();
   getsymbol();
   if ( ! e_label(symbol) )
	sint_error();
   put_sym();
   getsymbol ();
}

/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

label ()
{
   indent ();
   put_sym ();
   if (curnode != node) {
      write_pp (curfunction, node, ";");
      curnode = node;
   }
   getsymbol ();
   statement ();
}

/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

condition()
{
   put_sym ();
   getsymbol ();
}


/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

rotc_stat ()
{
   indent ();
   put_sym ();
   write_pp (curfunction, node, ";");
   curnode = node;
   getsymbol ();
   statement();
}


/*-----------------------------------------------------------------------------*/
/*
/*
/*   date: 08/26/96
/*   last atualization: 08/26/96
/*-----------------------------------------------------------------------------*/

create_func_pp (char *outname, char dir[], char file[])
{
   FILE *fp_pp;

   if ((fp_pp = criarq (dir, file, SUFIXO_PP)) == NULL) {
      return FALSE;
   } 
   

   fprintf(fp_pp, "#include	<stdio.h>\n#include	<instrument.h>\n\nint ponta_de_prova (long func, int node)\n{\nstatic FILE *fp=NULL;\nstatic long lf;\nstatic TRACE_LOG	*pr;\n\n   if (fp == NULL) {\n	fp = fopen (\"\%s\", \"w+\");\n	if (fp == NULL) {\n	   perror (\"Error creating log file\");\n	   exit (1);\n 	}\n	pr = new_log();\n   }\n   if ( ! test_node_log(pr, func, node) )\n   {\n	ins_node_log(pr, func, node);\n   	if (lf != func)\n	   fprintf (fp, \"\\n%%ld \", -(lf = func) );\n   	fprintf (fp, \" %%d\", node);\n	fflush(fp);\n   }   return 1;\n}\n\n\n", outname);

   fclose (fp_pp);
}


 
