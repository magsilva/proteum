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
#include <lib/gerais.h>
#include <polonesa/polonesa.h>
#include "li.h"




#define	TOKEN(x)	strcmp((x),saida.classe)==0
#define sintatic_error	s_error(); return FALSE


int	nivel;		/* current block nesting level */
int	nestlevel;	/* current statments nesting level */
int	switchlevel;	/* current switch nesting level */

extern  int pred, seq, cond; /* predicate, statement and condition conters */

extern	int	debug;

extern 	int	loc;

extern TABSIMBOLO	tab_vars,	/* variable table */
			tab_tipos;	/* type table */

extern	EXPRE_POS	expre_pos;	/* expressao na forma posfixa */

extern struct tableout saida;
extern int	error_sintatic;

extern	long	ini_token,
		comp_token,
		comp_ant,
		lin_token;

extern	struct tableout ident;

extern SIMBOLO *aloca_no();
int		libera_no();

analex()
{
   yylex();
   if (debug & 1)
      printf("\nlinha: %d simbolo: %s  Token: %s", saida.linha, 
					saida.label, saida.classe);
}

s_error()
{
   error_sintatic = TRUE;
   force_eof();
}





translation_unit()
{
   seq = cond = 1;
   nivel = 0;

   while (! TOKEN("EOF"))
   {
	if (! declaration())
	{
	   sintatic_error;
	}
   }
}


/*--------------------------------------------------------------------------
The functions global_declaration and global_function are only 
called by the mutation operators to analise a global declaration
or the begining of a function.
-------------------------------------------------------------------------*/
global_declaration()
{
SIMBOLO *ds, aux_ds;
int spec, decl;

   memset(&aux_ds, 0, sizeof aux_ds);
   spec = declaration_specifiers(&aux_ds);

   ds = aloca_no();
   *ds = aux_ds;

   decl = declarator(ds);

   l1:
   if (TOKEN("="))
   {
	analex();
	if (! initializer())
	{
	   sintatic_error;
	}
	ds->initialized = TRUE;
   }


  if (TOKEN(","))
   {
        if ( ! decl)
        {
           sintatic_error;
        }
        insert_simbol_variable(ds); /*insert variable in the simbol table */

        ds = aloca_no();
        *ds = aux_ds;           /* restore specifiers */
        analex();
        if ( ! declarator(ds))
        {
           sintatic_error;
        }
        goto l1;
   }

   if (TOKEN(";"))
   {
        if ( decl)
          insert_simbol_variable(ds); /* insert variable in the simbol table */
	analex();
   }
   else
   {
	sintatic_error;
   }

   if (! (decl || spec))
   {
	libera_no(ds);
	return FALSE;
   }
   return TRUE;
}


global_function()
{
SIMBOLO *ds, aux_ds;

   memset(&aux_ds, 0, sizeof(aux_ds));  /* initialize simbol to be defined */
   declaration_specifiers(&aux_ds);

   ds = aloca_no();
   *ds = aux_ds;
   if ( ! declarator(ds))
   {
	libera_no(ds);
	return FALSE;
   }

   li_begin_function(ds);
   cg_begin_function(ds->nome);
   insert_simbol_function(ds);
}





declaration()
{
int	has_init, decl, spec;
SIMBOLO	*ds, aux_ds;

   memset(&aux_ds, 0, sizeof(aux_ds));	/* initialize simbol to be defined */
   has_init = FALSE;
   spec = declaration_specifiers(&aux_ds);

   ds = aloca_no();
   *ds = aux_ds;
   decl = declarator(ds);

   l1:
   if (TOKEN("="))
   {
	has_init= TRUE;
	analex();
	if (! initializer())
	{
	   sintatic_error;
	}
	ds->initialized = TRUE;
   }

   if (TOKEN(","))
   {
	if ( ! decl)
	{
	   sintatic_error;
	}
	insert_simbol_variable(ds); /*insert variable in the simbol table */
	has_init = TRUE;

	ds = aloca_no();
	*ds = aux_ds;		/* restore specifiers */
	analex();
	if ( ! declarator(ds))
	{
	   sintatic_error;
	}
	goto l1;
   }

   if (TOKEN(";"))
   {
	if ( decl)
	  insert_simbol_variable(ds); /* insert variable in the simbol table */
	grava_li(1, "$DCL");
      	analex();
	return TRUE;
   }

   if ( ! (decl || spec))
   {
	libera_no(ds);
	return FALSE;
   }

   if (has_init || ! decl)
   {
	sintatic_error;
   }

   gfc_begin_function(ds->nome); /* initializes Control Flow Graph 
					for this function */
   li_begin_function(ds);	/* generate LI line indicating the begining
				   of o function ( @function ) */

   cg_begin_function(ds->nome);	/* initialize generation of call-graph 
				   for this function */


   insert_simbol_function(ds);  /* insert function name in the simbol table */



   if ( ! function_definition())
   {
	sintatic_error;
   }
   
   return TRUE;
}



block_declaration_list()
{
   while (block_declaration());
   return TRUE;
}


block_declaration()
{
int     has_init, decl;
SIMBOLO *ds, aux_ds;

   memset(&aux_ds, 0, sizeof(aux_ds));  /* initialize simbol to be defined */
   has_init = FALSE;
   if (! declaration_specifiers(&aux_ds))
	return FALSE;

   ds = aloca_no();
   *ds = aux_ds;
   decl = declarator(ds);

   l1:
   if (TOKEN("="))
   {
        has_init= TRUE;
        analex();
        if (! initializer())
        {
           sintatic_error;
        }
	ds->initialized = TRUE;
   }

   if (TOKEN(","))
   {
        if ( ! decl)
        {
           sintatic_error;
        }
        insert_simbol_variable(ds); /*insert variable in the simbol table */
        has_init = TRUE;

	ds = aloca_no();
        *ds = aux_ds;            /* restore specifiers */
        analex();
        if ( ! declarator(ds))
        {
           sintatic_error;
        }
        goto l1;
   }

   if (TOKEN(";"))
   {
        if ( decl)
          insert_simbol_variable(ds); /* insert variable in the simbol table */
        grava_li(1, "$DCL");
        analex();
        return TRUE;
   }

   return TRUE;
}



  
declaration_specifiers(ds)
SIMBOLO	*ds;
{
int k, j;

   j = FALSE;

   do {
	k = FALSE;
	k |= storage_class(ds);
	k |= type_specifier(ds);
	k |= type_qualifier(ds);
	j |= k;
      } while (k);

    if ((ds->specifier).f_char == 0 && ds->specifier.f_void == 0 &&
	ds->specifier.f_float == 0 && ds->specifier.f_double == 0 &&
	ds->specifier.f_other == 0  && ds->specifier.f_enum == 0 &&
        ds->specifier.f_struct == 0)
	  ds->specifier.f_int = TRUE;

    if (ds->tipo[0] == '\0')
	tipo_to_str(ds, ds->tipo);
    return j;
}


storage_class(ds)
SIMBOLO	*ds;
{

   if (TOKEN("auto"))
   {
	ds->storage.f_auto = TRUE;
	goto l1;
   }
   if (TOKEN("register"))
   {
	ds->storage.f_register= TRUE;
	goto l1;
   }
   if (TOKEN("static"))
   {
	ds->storage.f_static = TRUE;
	goto l1;
   }
   if (TOKEN("extern"))
   {
	ds->storage.f_extern = TRUE;
	goto l1;
   }
   if (TOKEN("typedef"))
   {
	ds->storage.f_typedef = TRUE;
	goto l1;
   }
   return FALSE;
   l1:
   analex();
   return TRUE;
}


type_specifier(ds)
SIMBOLO	*ds;
{

   if (TOKEN("void"))
   {
	ds->specifier.f_void = TRUE;
	l1:
	analex();
	return TRUE;
   }
   if (TOKEN("char"))
   {
	ds->specifier.f_char = TRUE;
	goto l1;
   }
   if (TOKEN("short"))
    {
	ds->specifier.f_short = TRUE;
	goto l1;
   }
   if (TOKEN("int"))
    {
	ds->specifier.f_int = TRUE;
	goto l1;
   }
   if (TOKEN("long"))
    {
	ds->specifier.f_long = TRUE;
	goto l1;
   }
   if (TOKEN("float"))
    {
	ds->specifier.f_float = TRUE;
	goto l1;
   }
   if (TOKEN("double"))
    {
	ds->specifier.f_double = TRUE;
	goto l1;
   }
   if (TOKEN("signed"))
    {
	ds->specifier.f_signed = TRUE;
	goto l1;
   }
   if (TOKEN("unsigned"))
    {
	ds->specifier.f_unsigned = TRUE;
	goto l1;
   }


   if (TOKEN("TIPDEF"))
   {
	ds->specifier.f_other = TRUE; /* is a user-defined type */
	strcpy(ds->tipo, saida.label);
	goto l1;
   }
   if ( struct_or_union_specifier(ds))
   {
	ds->specifier.f_struct = TRUE;
	return TRUE;
   }

   if (  enum_specifier(ds))
   {
        ds->specifier.f_enum = TRUE;
	return TRUE;
   }
   return FALSE;

}


type_qualifier(ds)
SIMBOLO	*ds;
{

   if (TOKEN("const")) 
   {
	ds->qualifier.f_const = TRUE;
	analex();
	return TRUE;
   }
   if (TOKEN("volatile")) 
   {
	ds->qualifier.f_volatile = TRUE;
	analex();
	return TRUE;
   }


   return FALSE;
}


struct_or_union_specifier(ds)
SIMBOLO	*ds;
{
int	 tem_ident;
static int	cont = 0;
static char	buf[10];
SIMBOLO	*aux_ds;

   if ( (! TOKEN("struct")) && (! TOKEN("union")))
   	return FALSE;

   aux_ds = aloca_no();
   strcpy(aux_ds->nome, saida.label);
   aux_ds->specifier.f_struct = TRUE;

   analex();

   if ( tem_ident = (TOKEN("IDENT") || TOKEN("TIPDEF")))
   {
	strcat(aux_ds->nome, " ");
	strcat(aux_ds->nome, saida.label);
	analex();
   }
   else
   {
	sprintf(buf, " $%d", cont++);
	strcat(aux_ds->nome, buf);
   }

   if (TOKEN("{") )
   {
	analex();
	if (! struct_declaration_list(aux_ds))
	{
		sintatic_error;
	}
	if (TOKEN("}"))
	{
	   strcpy(ds->tipo, aux_ds->nome);
	   insert_simbol_struct(aux_ds);
	   analex();
	   return TRUE;
	}
	else
	{
	   sintatic_error;
	}
   }
   else
   {
	if (tem_ident)
	{
	   strcpy(ds->tipo, aux_ds->nome);
	   return TRUE;
	}
	else
	{
	   sintatic_error;
	}
   }
}


struct_declaration_list(ds)
SIMBOLO	*ds;
{

   while (struct_declaration(ds));
   return TRUE;
}


struct_declaration(ds)
SIMBOLO	*ds;
{
SIMBOLO	aux_ds, aux2_ds;
int	esp, decl;

   memset(&aux_ds, 0, sizeof(aux_ds));

   esp = specifier_qualifier_list(&aux_ds);

   aux2_ds = aux_ds;
   decl = struct_declarator(&aux_ds); 

   if (! (decl || esp))
	return FALSE;

   l1:

   if (TOKEN(","))
   {
     insert_field(ds, &aux_ds);	/* insert field in a structure */

     analex();

     aux_ds = aux2_ds;
     if ( ! struct_declarator(&aux_ds))
     {
	sintatic_error;
     }

     goto l1;
    }

   if ( ! TOKEN(";"))
   {
	sintatic_error;
   }

   insert_field(ds, &aux_ds); 
   analex();
   return TRUE;
}


specifier_qualifier_list(ds)
SIMBOLO *ds;
{

int k, j;

   j = FALSE;

   do {
	k = FALSE;
	k |= type_specifier(ds);
	k |= type_qualifier(ds);
	j |= k;
      } while (k);

    if (ds->specifier.f_char == 0 && ds->specifier.f_void == 0 &&
	ds->specifier.f_float == 0 && ds->specifier.f_double == 0 &&
	ds->specifier.f_other == 0 && ds->specifier.f_enum == 0 &&
	ds->specifier.f_struct == 0)
	  ds->specifier.f_int = TRUE;

    if (ds->tipo[0] == '\0')
        tipo_to_str(ds, ds->tipo);

   return j;
}


register_specifier_qualifier_list(ds)
SIMBOLO *ds;
{

int k, j;

   j = FALSE;

   do {
        k = FALSE;
	if (TOKEN("register"))
	{
	   analex();
	   ds->storage.f_register = TRUE;
	   k = TRUE;
	}
        k |= type_specifier(ds);
        k |= type_qualifier(ds);
        j |= k;
      } while (k);

    if (ds->specifier.f_char == 0 && ds->specifier.f_void == 0 &&
        ds->specifier.f_float == 0 && ds->specifier.f_double == 0 &&
        ds->specifier.f_other == 0 && ds->specifier.f_enum == 0 &&
        ds->specifier.f_struct == 0)
          ds->specifier.f_int = TRUE;

    if (ds->tipo[0] == '\0')
        tipo_to_str(ds, ds->tipo);

   return j;
}


struct_declarator(ds)
SIMBOLO	*ds;
{

   if (! declarator(ds)  && ! TOKEN(":"))
	return FALSE;

   if (TOKEN(":"))
   {
	analex();
	if ( ! constant_expression())
	{
	   sintatic_error;
	}
	else
	   return TRUE;
   }
   else
	return TRUE;
}



declarator(ds)
SIMBOLO	*ds;
{

   pointer(ds);

   return direct_declarator(ds);
}


pointer(ds)
SIMBOLO	*ds;
{
SIMBOLO	aux_ds;

   if ( ! TOKEN("*"))
	return FALSE;
   strcat(ds->ptar, "*");

   analex();
   type_qualifier_list(&aux_ds);

   while (TOKEN("*"))
   {
	strcat(ds->ptar, "*");
	analex();
	type_qualifier_list(&aux_ds);
   }
   return TRUE;
}


type_qualifier_list(ds)
SIMBOLO	*ds;
{

   while(type_qualifier(ds));
   return TRUE;
}


direct_declarator(ds)
SIMBOLO	*ds;
{
SIMBOLO	*pds;

   if (TOKEN("("))
   {
	strcat(ds->ptar, "{");
	analex();
	if ( ! declarator(ds))
	{
	   sintatic_error;
	}
	if (TOKEN(")"))
	{
	   strcat(ds->ptar, "}");
	   analex();
	}
	else
	{
	   sintatic_error;
	}
    }
    else
    if (TOKEN("IDENT"))
    {
	strcpy(ds->nome, saida.label);
	ident = saida;
	analex();
    }
    else
	return FALSE;

   while (TOKEN("["))
   {
	strcat(ds->ptar, "[");
	analex();
	if (TOKEN("]"))
	{
	   analex();
	   continue;
	}

	if ( ! constant_expression())
	{
	   sintatic_error;
	}
   	if (TOKEN("]"))
	{
	   analex();
	 }
	 else
	 {
	   sintatic_error;
	 }
    }

    if (TOKEN("("))
    {
        strcat(ds->ptar, "(");
	analex();
	if (TOKEN(")"))
	{
	   analex();
	   return TRUE;
	}

	parameter_type_list(ds);
	if ( TOKEN(")"))
	{
	   analex();
	   return TRUE;
	}
	else
	{
	   sintatic_error;
	}
    }

    return TRUE;
}


initializer()
{

   if (TOKEN("{"))
   {
	analex();
	l1:
	if ( ! initializer() )
	{
	   sintatic_error;
	}

	if ( TOKEN("}"))
	{
	   analex();
	   return TRUE;
	}

	if (TOKEN(","))
	{
	   analex();
	}
	else
	{
	   sintatic_error;
	}

	if ( TOKEN("}"))
	{
	   analex();
	   return TRUE;
	}
	else
	   goto l1;
    }
    else
	return aux_assign_expression();
}


enum_specifier(ds)
SIMBOLO	*ds;
{
static int	cont = 0;
static char 	buf[10];
int	tem_ident;
SIMBOLO *aux_ds;

   if (! TOKEN("enum"))
   {
	return FALSE;
   }
   aux_ds = aloca_no();
   strcpy(aux_ds->nome, saida.label);
   aux_ds->specifier.f_enum = TRUE;

   analex();

  if ( tem_ident = (TOKEN("IDENT") || TOKEN("TIPDEF") ))
  {
	strcat(aux_ds->nome, " ");
	strcat(aux_ds->nome, saida.label);
     	analex();
  }
  else
  {
	sprintf(buf, " $%d", cont++);
	strcat(aux_ds->nome, buf);
   }
   if ( TOKEN("{"))
   {
	analex();
	if ( ! enumerator_list(aux_ds))
	{
	   sintatic_error;
	}

	if ( TOKEN("}"))
	{
	   insert_simbol_enum(aux_ds);
	   strcpy(ds->tipo, aux_ds->nome);
	   analex();
	   return TRUE;
	}
	else
	{
	   sintatic_error;
	}
    }
    else
    {
	if (tem_ident)
	{
           strcpy(ds->tipo, aux_ds->nome);
	   return TRUE;
	}
	else
	   sintatic_error;
    }
}



enumerator_list(ds)
SIMBOLO	*ds;
{
SIMBOLO aux_ds;

   ins_fake_enum(&tab_tipos, ds);
   memset(&aux_ds, 0, sizeof(aux_ds)); 
   if ( ! enumerator(&aux_ds))
   {
	delete_fake(&tab_tipos);
	return FALSE;
   }
   insert_field(ds, &aux_ds); /* insert constant enum */


   while ( TOKEN(","))
   {
	analex();
	memset(&aux_ds, 0, sizeof(aux_ds));
	if ( ! enumerator(&aux_ds))
	{
	   sintatic_error;
	}
	insert_field(ds, &aux_ds); /* insert constant enum */

   }
   delete_fake(&tab_tipos);
   return TRUE;
}



enumerator(ds)
SIMBOLO *ds;
{

   if ( ! TOKEN("IDENT"))
	return FALSE;
   strcpy(ds->nome, saida.label);

   analex();

   if ( TOKEN("="))
   {
	analex();
	if ( ! constant_expression())
	{
	   sintatic_error;
	}

	return TRUE;
    }

    return TRUE;
}

parameter_type_list(ds)
SIMBOLO	*ds;
{
SIMBOLO	aux_ds;

   memset(&aux_ds, 0, sizeof(aux_ds));
   if ( ! parameter_declaration(&aux_ds))
	return FALSE;
   insert_parameter(ds, &aux_ds);

   while (TOKEN(","))
   {
	analex();
	if (TOKEN("..."))
	{
	   analex();
	   return TRUE;
	}

   	memset(&aux_ds, 0, sizeof(aux_ds));
	if ( ! parameter_declaration(&aux_ds))
	{
	   sintatic_error;
	}
	else
	{
	   insert_parameter(ds, &aux_ds);
	}
   }
   return TRUE;
}


parameter_declaration(ds)
SIMBOLO	*ds;
{
int dcl, abs;

   dcl = declaration_specifiers(ds); 
   abs = abstract_declarator(ds); 
   return dcl || abs;
}


abstract_declarator(ds)
SIMBOLO	*ds;
{

   if (pointer(ds))
   {
	direct_abstract_declarator(ds);
	return TRUE;
   }
   else
   {
	return direct_abstract_declarator(ds);
   }

}



direct_abstract_declarator(ds)
SIMBOLO	*ds;	
{

   if (TOKEN("("))
   {
        strcat(ds->ptar, "{");
	analex();
	if ( ! abstract_declarator(ds))
	{
	   sintatic_error;
	}
	if (TOKEN(")"))
	{
           strcat(ds->ptar, "}");
	   analex();
	}
	else
	{
	   sintatic_error;
	}
    }
    else
    if (TOKEN("IDENT"))
    {
        strcpy(ds->nome, saida.label);
	analex();
    }

   while (TOKEN("["))
   {
        strcat(ds->ptar, "[");
	analex();
	if (TOKEN("]"))
	{
	   analex();
	   continue;
	}
	if ( ! constant_expression())
	{
	   sintatic_error;
	}
   	if (TOKEN("]"))
	{
	   analex();
	 }
	 else
	{
	   sintatic_error;
	}
    }

    if (TOKEN("("))
    {
        strcat(ds->ptar, "(");
	analex();
	if (TOKEN(")"))
	{
	   analex();
	   return TRUE;
	}

	parameter_type_list(ds);
	if ( TOKEN(")"))
	{
	   analex();
	   return TRUE;
	}
	else
	{
	   sintatic_error;
	}
    }

    return TRUE;
}


type_name(ds)
SIMBOLO	*ds;
{

   if (! (type_specifier(ds) || type_qualifier(ds)))
	return FALSE;

   while ( type_specifier(ds) || type_qualifier(ds));


   abstract_declarator(ds);
   return TRUE;
}



function_definition()
{
int i, endnode;

   nivel = 1;
   nestlevel = switchlevel = 0;

   if ( ! function_declaration_list())
	return FALSE;

   if ( !TOKEN("{"))
	return FALSE;

   /* ??? definicao de todas as globais */
   /* ??? definicao de todos os parametros */

   grava_li(1, "{");	/* write inic of body functio to li file */
   nivel = 2;		/* all body declarations starts at level 2 */

   analex();


   block_declaration_list();
   /* ??? definicao ou indefinicao de todas as locais de nivel 2 */

   statement_list();

   if ( ! TOKEN("}"))
   {
	sintatic_error;
   }
   endnode = gfc_curnode();
   if ( gfc_has_return() &&  gfc_is_empty(endnode) == NOEMPTY)
   {
	gfc_add_aresta(endnode, gfc_new_node());
    	endnode = gfc_curnode();
   }

   for (i = 0; i < endnode; i++)
	if ( gfc_is_return(i) )
	   gfc_add_aresta(i,endnode);

   grava_li(1, "}");


   li_fim_function();
   cg_fim_function();
   gfc_fim_function();	/* ends Control Flow Graph */
   nivel = 0;		/* all out-body declarations are level 0 */

/*   print_tab(&tab_vars); */
   esvazia_tab(&tab_vars, 1);  /* release all simbols defined at level 1 */
   esvazia_tab(&tab_tipos, 1);  /* or deeper				 */

   analex();

   return TRUE;
}




function_declaration_list()
{
   while (function_declaration());
   return TRUE;
}


function_declaration()
{
SIMBOLO *aux_ds, aux2_ds;
int	esp, decl;

   memset(&aux2_ds, 0, sizeof(aux2_ds));
   esp = register_specifier_qualifier_list(&aux2_ds);
   aux_ds = aloca_no();
   *aux_ds = aux2_ds;

   decl = struct_declarator(aux_ds);

   if (! (decl || esp))
   {
	libera_no(aux_ds);
        return FALSE;
   }

   insert_simbol_parameter(aux_ds); /* insert field in a structure */
 
   l1:

   if (TOKEN(","))
   {

     analex();

     aux_ds = aloca_no();
     *aux_ds = aux2_ds;
     if ( ! struct_declarator(aux_ds))
     {
        sintatic_error;
     }
     insert_simbol_parameter(aux_ds); /* insert field in a structure */

     goto l1;
    }

   if ( ! TOKEN(";"))
   {
        sintatic_error;
   }

   grava_li(1, "$DCL");
   analex();
   return TRUE;
}

   


statement_list()
{
   while (statement());
   return TRUE;
}

statement()
{

   if ( TOKEN(";"))
   {
	grava_li(2, "$S");
   	gfc_no_empty(gfc_curnode(), NOEMPTY);
	analex();
	return TRUE;
   }
   else
   if ( labeled_statement())
	return TRUE;
   else
   if ( compound_statement())
	return TRUE;
   else
   if ( selection_statement())
   	return TRUE;
   else
   if ( iteraction_statement())
	return TRUE;
   else
   if ( jump_statement())
	return TRUE;
   else
   	return expression_statement();
}


expression_statement()
{

   if ( ! expression())
	return FALSE;

   grava_li(2, "$S");
   gfc_no_empty(gfc_curnode(), NOEMPTY);

   if ( TOKEN(";"))
   {
	analex();
	return TRUE;
   }
   else
	return FALSE;
}


compound_statement()
{
int k;

    if (! TOKEN("{"))
	return FALSE;
   k = gfc_curnode();

   grava_li(1, "{");	/* write endof body functio to li file */
   nivel++;		/* increment nesting level */

   analex();

   block_declaration_list();

   /* ??? definicao ou indefinicao de todas as locais no k, nivel "nivel" */

   statement_list();

   if ( TOKEN("}"))
   {
	gfc_no_empty(gfc_curnode(), NOEMPTY);
   	grava_li(1, "}");	/* write endof body functio to li file */

   	esvazia_tab(&tab_vars, nivel);  /* release all simbols defined at */
   	esvazia_tab(&tab_tipos, nivel);  /* that level or deeper	 */

   	nivel-- ;		/* decrement nesting level */

	analex();
	return TRUE;
   }
   else
   {
	sintatic_error;
   }
}



selection_statement()
{
int i, k;

   if ( TOKEN("if"))
   {
    int ifnode, thennode, endnode, elsenode = -1;

	nestlevel++;
	grava_li(1, "$IF");

	analex();
	if ( TOKEN("("))
	{
	   analex();
	   if (! expression())
	   {
		sintatic_error;
	   }
	   ifnode = gfc_curnode(); /* get the current node */
	   /* ??? definicao e uso das variaveis da condicao no' ifnode */
	    if ( TOKEN(")"))
	    {
		   grava_li(3, "$C");

		   analex();
		   thennode = gfc_new_node();

			/* insere aresta entre ifnode e thennode */
		   gfc_add_aresta(ifnode, thennode);
		   if (! statement())
		   {
			sintatic_error;
		   }
		   thennode = gfc_curnode();
		   if ( gfc_is_empty(thennode) == EMPTY &&
			! gfc_chega(thennode) )
		   {
			gfc_release(thennode);
			thennode = -1;
		   }
		   

		   if (TOKEN("else"))
		   {
			elsenode = gfc_new_node();
			grava_li(1, "$ELSE");
		   	gfc_add_aresta(ifnode, elsenode);

			analex();
			if ( ! statement())
			{
				sintatic_error;
			}
			elsenode = gfc_curnode(); /* get current node */
		   	if ( gfc_is_empty(elsenode) == EMPTY &&
			     ! gfc_chega(elsenode) )
		   	{
			    gfc_release(elsenode);
			    elsenode = -2;
		   	}
		    }
		   endnode = gfc_new_node();

			/* insere aresta entre thennode e endnode */
		   if (thennode >= 0)
			gfc_add_aresta(thennode, endnode);

		   if (elsenode >= 0)
			/* insere aresta entre elsenode e endnode */
		        gfc_add_aresta(elsenode, endnode);
		   else
		   if (elsenode == -1)
			gfc_add_aresta(ifnode,endnode);
	     }
	     else
	     {
		    sintatic_error;
	     }
	}
	else
	{
	      sintatic_error;
	}
	nestlevel--;
   }
   else
   if ( TOKEN("switch"))
   {
    int switchnode, nextnode, endnode;

	switchlevel++;
	switchnode = gfc_curnode();
	grava_li(1, "$CASE");
	gfc_no_empty(switchnode, NOEMPTY);

	analex();
	if ( ! TOKEN("("))
	{
	   sintatic_error;
	}

	analex();
	if ( ! expression())
	{
	   sintatic_error;
	}

	if (! TOKEN(")"))
	{
   	   sintatic_error;
	}

	grava_li(1, "$CC");

	gfc_new_node();
	analex();	   

	if ( ! statement())
	{
	   sintatic_error;
	}
	endnode = gfc_curnode();
	if ( gfc_is_empty(endnode) == EMPTY &&
		! gfc_chega(endnode) )
	{
		gfc_release(endnode);
		endnode = -1;
	}
	nextnode = gfc_new_node();
	if (endnode >= 0)
	   gfc_add_aresta(endnode,nextnode);

	for (i = switchnode; i < nextnode; i++)
	{
	   if ( gfc_is_case(i) )
	   {
		gfc_add_aresta(switchnode,i);
		gfc_case(i,FALSE);
	   }
	}
	trata_break(switchnode, nextnode);
	gfc_add_aresta(switchnode,nextnode);
	switchlevel--;
    }
    else
	return FALSE;	    

    return TRUE;
}





labeled_statement()
{
struct tableout	aux_saida;

   aux_saida = saida;
   if ( TOKEN("IDENT"))
   {
    int labelnode;
	analex();
	if ( TOKEN(":"))
	{
	   labelnode = gfc_curnode();
	   if (  gfc_is_empty(labelnode) == NOEMPTY)
	   {
		gfc_add_aresta(labelnode, gfc_new_node());
		labelnode = gfc_curnode();
		gfc_no_empty(labelnode, WLABEL);
	   }
	   grava_li(1, aux_saida.label);
           if ( gfc_label(labelnode, aux_saida.label) < 0)
           {
		sintatic_error;
	   }
	   analex();
	}
	else
	{
	     back_track(&aux_saida);
	     return FALSE;
	}
	if (! statement())
	{
	   sintatic_error;
	}
   }
   else
   if ( TOKEN("case"))
   {
   int labelnode;

	if (switchlevel <= 0)
	{
	   sintatic_error;
	}

      	analex();
      	if ( ! constant_expression())
	{
	   sintatic_error;
	}

	if (! TOKEN(":"))
	{
	   sintatic_error;
	}

	labelnode = gfc_curnode();
	
	if ( gfc_is_empty(labelnode) == NOEMPTY )
	{
	   gfc_add_aresta(labelnode, gfc_new_node());
	   labelnode = gfc_curnode();
	   gfc_no_empty(labelnode, WLABEL);
	}
	gfc_case(labelnode, TRUE);

	grava_li(1, "$ROTC");

	analex();
	if (! statement())
	{
	   sintatic_error;
	}
   }
   else
   if ( TOKEN("default"))
   {
   int labelnode;

	if (switchlevel <= 0)
	{
	   sintatic_error;
	}

	analex();
	if (! TOKEN(":"))
	{
	   sintatic_error;
	}

	labelnode = gfc_curnode();
	
	if ( ! gfc_is_empty(labelnode) == NOEMPTY)
	{
	   gfc_add_aresta(labelnode, gfc_new_node());
	   labelnode = gfc_curnode();
	   gfc_no_empty(labelnode, WLABEL);
	}
	gfc_case(labelnode, TRUE);

	grava_li(1, "$ROTD");

	analex();
	if (! statement())
	{
	   sintatic_error;
	}
   }
   else
	return FALSE;

   return TRUE;
}



iteraction_statement()
{

   if ( TOKEN("while"))
   {
    int whilenode, bodynode, nextnode;
	nestlevel++;
	whilenode = gfc_curnode();
	if (  gfc_is_empty(whilenode) == NOEMPTY)
	{
	   gfc_add_aresta(whilenode, gfc_new_node());
	   whilenode = gfc_curnode();
	}

	grava_li(1, "$WHILE");

	analex();
	if ( ! TOKEN("("))
	{
	   sintatic_error;
	}
	analex();
	if ( ! expression())
	{
	   sintatic_error;
	}
	if ( ! TOKEN(")"))
	{
	   sintatic_error;
	}
	grava_li(3, "$C");

	bodynode = gfc_new_node();
	gfc_add_aresta(whilenode, bodynode);
	analex();
	if ( ! statement())
	{
	   sintatic_error;
	}
	bodynode = gfc_curnode();
	if ( gfc_is_empty(bodynode) == EMPTY &&
		! gfc_chega(bodynode) )
	{
		gfc_release(bodynode);
		bodynode = -1;
	}
	nextnode = gfc_new_node();
	if (bodynode >= 0)
	   gfc_add_aresta(bodynode, whilenode);
	gfc_add_aresta(whilenode, nextnode);
	trata_break(whilenode, nextnode);
	trata_continue(whilenode,nextnode);
	nestlevel--;
   }
   else
   if ( TOKEN("do"))
   {
   int donode, whilenode, nextnode;

	nestlevel++;
	donode = gfc_curnode();
	if (  gfc_is_empty(donode) == NOEMPTY)
	{
	   gfc_add_aresta(donode, gfc_new_node());
	   donode = gfc_curnode();
	}

	grava_li(1, "$REPEAT");

	analex();
	if ( ! statement())
	{
	   sintatic_error;
	}
	if (! TOKEN("while"))
	{
	   sintatic_error;
	}
	grava_li(1, "$UNTIL");

	analex();
	if ( ! TOKEN("("))
	{
	   sintatic_error;
	}
	analex();
	if ( ! expression())
	{
	   sintatic_error;
	}
	if ( ! TOKEN(")"))
	{
	   sintatic_error;
	}
	analex();
	if ( ! TOKEN(";"))
	{
	   sintatic_error;
	}
	grava_li(3, "$NC");

	whilenode = gfc_curnode();
	
	gfc_add_aresta(whilenode, donode);
	nextnode = gfc_new_node();
	gfc_add_aresta(donode, nextnode);
	gfc_add_aresta(whilenode, nextnode);
	trata_break(donode, nextnode);
	trata_continue(donode,nextnode);
	nestlevel--;
	
	analex();
	 
   }
   else
   if ( TOKEN("for"))
   {
    int fornode, condnode, bodynode, incrnode, nextnode;

	nestlevel++;
	fornode = gfc_curnode();
/*	if (  gfc_is_empty(fornode) == NOEMPTY )
	{
	   gfc_add_aresta(fornode, gfc_new_node());
	   fornode = gfc_curnode();
	}
*/
	grava_li(1, "$FOR");

	analex();
	if ( ! TOKEN("("))
	{
	   sintatic_error;
	}
	ini_token = 0;		/* ( nao deve aparecer na li */

	analex();
	expression();

	if ( ! TOKEN(";"))
	{
	   	sintatic_error;
	}

	grava_li(2, "$S");

	condnode = gfc_new_node(); /* new node to the condition */
	analex();
	if ( ! expression())
	{			/* condition is empty */
	   ini_token = comp_token = lin_token = 0;
	}


	if ( ! TOKEN(";"))
	{
	   sintatic_error;
	}

	grava_li(3, "$C");

	analex();

	if (expression())
	{
	   comp_token = comp_ant;
	}
	else
	{
	   ini_token = comp_token = lin_token = 0;
	}

	incrnode = gfc_new_node(); /* new node to the increment */
	grava_li(2, "$S");
	calcula_posicao();

	if ( ! TOKEN(")"))
	{
	   sintatic_error;
	}
	ini_token = 0;		/* ) nao deve aparecer na li */

	bodynode = gfc_new_node();
	gfc_add_aresta(condnode, bodynode);
	analex();
	if (! statement())
	{
	   sintatic_error;
	}
	bodynode = gfc_curnode();
	if ( gfc_is_empty(bodynode) == EMPTY &&
		! gfc_chega(bodynode) )
	{
		gfc_release(bodynode);
		bodynode = -1;
	}

	nextnode = gfc_new_node();
	gfc_add_aresta(fornode, condnode);
	gfc_add_aresta(condnode, nextnode);
	if (bodynode >= 0)
	   gfc_add_aresta(bodynode, incrnode);
	gfc_add_aresta(incrnode, condnode);
	trata_break(incrnode, nextnode);
	trata_continue(incrnode, nextnode);
	nestlevel--;
   }
   else 
	return FALSE;

   return TRUE;

}

	    
jump_statement()
{
char	*p;
char	buf[sizeof(saida.label)];
int	gonode, k;

   gonode = gfc_curnode();
   if (TOKEN("goto"))
   {
	grava_li(1, "$GOTO");

	analex();
	if ( !TOKEN("IDENT"))
	{
	   sintatic_error;
	}
	strcpy(buf, saida.label);
	p = buf;
        if ( (k = gfc_get_label(p))  >= 0)
           gfc_add_aresta(gonode, k);
        else
           if ( gfc_goto(gonode, p) < 0)
	   {
		sintatic_error;
	   }
	analex();
    }
    else
    if (TOKEN("continue"))
    {
	if (nestlevel <= 0)
	{
	   sintatic_error;
	}
	gfc_continue(gonode,TRUE);
	p = "$CONTINUE";
	analex();
     }
    else
    if (TOKEN("break"))
    {
	if (nestlevel <= 0 && switchlevel <= 0)
	{
	   sintatic_error;
	}
	gfc_break(gonode,TRUE);
	p = "$BREAK";
	analex();
     }
    else
    if (TOKEN("return"))
    {
	gfc_return(gonode,TRUE);
	p = "$RETURN";
	analex();
	expression();
     }
     else
	return FALSE;

    if ( ! TOKEN(";"))
    {
	sintatic_error;
    }
    gfc_no_empty(gonode, NOEMPTY);
    grava_li(1, p);
    gfc_new_node();
    analex();
    return TRUE;
}



expression()
{
int	i;

   pred = 1;
   zera_expre(&expre_pos);
   i = expression_aux();
   if (debug & 2)
	print_expre(&expre_pos);
   return i;
}

aux_assign_expression()
{
int	i;

   pred = 1;
   zera_expre(&expre_pos);
   i = assigment_expression();
   if (debug & 2)
	print_expre(&expre_pos);
   return i;
}

expression_aux()
{
   if ( ! assigment_expression())
	return FALSE;

   while (TOKEN(","))
   {
	analex();
	if (! assigment_expression())
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, ",", OP_2_SEQ);
   }
   return TRUE;

}


assigment_expression()
{
char	b[4];
int	op;

   if ( ! conditional_expression())
      return FALSE;

   strcpy(b, saida.label);
   if ( assigment_operator())
   {
	if ( ! assigment_expression())
	{
	   sintatic_error;
	}
	op = type_of_assigment(b);
	ins_expre(&expre_pos, b, op);
   }
   
   return TRUE;
}

assigment_operator()
{

   if (TOKEN("=") || TOKEN("*=") || TOKEN("/=") || TOKEN("%=") ||
      TOKEN("+=") || TOKEN("-=") || TOKEN("<<=") || TOKEN(">>=") ||
      TOKEN("^=") || TOKEN("|=") || TOKEN("&=") )
   {
	analex();
	return TRUE;
   }
   else
	return FALSE;
}


conditional_expression()
{
   if ( ! logical_OR_expression())
	return FALSE;

   if (TOKEN("?"))
   {
	analex();
	if ( ! expression_aux())
	{
	   sintatic_error;
	}
	if (! TOKEN(":"))
	{
	   sintatic_error;
	}

	analex();
	if ( ! conditional_expression())
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, "?", OP_3);
    }
    
    return TRUE;
}



constant_expression()
{
   zera_expre(&expre_pos);
   return conditional_expression();
}



logical_OR_expression()
{
   if ( ! logical_AND_expression())
	return FALSE;

   while ( TOKEN("||"))
   {
	pred++;
	analex();
	if ( ! logical_AND_expression())
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, "||", OP_2_LOG);
   }
   return TRUE;
}

  
logical_AND_expression()
{
   if ( ! inclusive_OR_expression())
	return FALSE;

   while ( TOKEN("&&"))
   {
	pred++;
	analex();
	if ( ! inclusive_OR_expression())
	{
	   sintatic_error;
	}
  	ins_expre(&expre_pos, "&&", OP_2_LOG); 
  }
   return TRUE;
}

inclusive_OR_expression()
{
   if ( ! exclusive_OR_expression())
	return FALSE;

   while ( TOKEN("|"))
   {
	analex();
	if ( ! exclusive_OR_expression())
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, "|", OP_2_BIT);
   }
   return TRUE;
}


exclusive_OR_expression()
{
   if ( ! AND_expression())
	return FALSE;

   while ( TOKEN("^"))
   {
	analex();
	if ( ! AND_expression())
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, "^", OP_2_BIT);
   }
   return TRUE;
}



AND_expression()
{
   if ( ! equality_expression())
	return FALSE;

   while ( TOKEN("&"))
   {
	analex();
	if ( ! equality_expression())
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, "&", OP_2_BIT);
   }
   return TRUE;
}


equality_expression()
{
char	b[3];

   if ( ! relational_expression())
	return FALSE;

   while ( TOKEN("==") || TOKEN("!="))
   {
	strcpy(b, saida.label);
	analex();
	if ( ! relational_expression())
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, b, OP_2_RELA);
   }
   return TRUE;
}


relational_expression()
{
char	b[3];

   if ( ! shift_expression())
	return FALSE;

   while ( TOKEN("<") || TOKEN(">") || TOKEN("<=") || TOKEN(">="))
   {
	strcpy(b, saida.label);
	analex();
	if ( ! shift_expression())
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, b, OP_2_RELA);
   }
   return TRUE;
}


shift_expression()
{
char	b[3];

   if ( ! additive_expression())
	return FALSE;

   while ( TOKEN("<<") || TOKEN(">>"))
   {
	strcpy(b, saida.label);
	analex();
	if ( ! additive_expression())
	{
	   sintatic_error;
	}
   	ins_expre(&expre_pos, b, OP_2_SHIFT);
   }
   return TRUE;
}


additive_expression()
{
char	b[2]; 

   if ( ! multiplicative_expression())
	return FALSE;

   while ( TOKEN("+") || TOKEN("-"))
   {
	strcpy(b, saida.label);
	analex();
	if ( ! multiplicative_expression())
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, b, OP_2_ARIT);
   }
   return TRUE;
}


multiplicative_expression()
{
char	b[2];
	
   if ( ! cast_expression())
	return FALSE;

   while ( TOKEN("*") || TOKEN("/") || TOKEN("%"))
   {
	strcpy(b, saida.label);
	analex();
	if ( ! cast_expression())
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, b, OP_2_ARIT);
   }
   return TRUE;
}

cast_expression()
{
SIMBOLO	ds;
struct tableout aux_saida;
char	b[TAMLEX+1];

   if ( ! TOKEN("("))
	return unary_expression();

   memset(&ds, 0, sizeof(ds));
   aux_saida = saida;
   analex();
   if ( ! type_name(&ds))
   {
	back_track(&aux_saida);
	return unary_expression();
   }

   cast_operator(&ds, b);
   lib_arvore(ds.campos);

   if ( ! TOKEN(")"))
   {
	sintatic_error;
   }

   analex();
   if ( ! cast_expression())
   {
	sintatic_error;
   }
   ins_expre(&expre_pos, b, OP_1_CAST);
   return TRUE;
}

unary_expression()
{
char	b[TAMLEX+1];
int	op;
SIMBOLO ds;

   strcpy(b, saida.label);
   memset(&ds, 0, sizeof(ds));

   if (TOKEN("++") || TOKEN("--"))
   {
	analex();
	if ( ! unary_expression())
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, b, OP_1_ARIT);
   }
   else
   if ( unary_operator())
   {
	if ( ! cast_expression())
	{
	   sintatic_error;
	}
	op = type_of_unary(b);
	ins_expre(&expre_pos, b, op);
   }
   else
   if (TOKEN("sizeof"))
   {
	analex();
	if ( TOKEN("("))
	{
	   analex();
	   if ( type_name(&ds))
	   {
		cast_operator(&ds, b);
		ins_expre(&expre_pos, b, NOME_TIPO);
		lib_arvore(ds.campos);
	   }
	   else
	   if (! expression_aux())
	   {
		sintatic_error;
	   }

	   if ( ! TOKEN(")"))
	   {
		sintatic_error;
	   }
	   analex();
	 }
	 else
	   if ( ! unary_expression())
	   {
		sintatic_error;
	   }
	ins_expre(&expre_pos, "sizeof", OP_1_SIZEOF);
   }
   else
	return postfix_expression();

   return TRUE;
}


unary_operator()
{

   if ( TOKEN("&") || TOKEN("*") || TOKEN("+") ||
	TOKEN("-") || TOKEN("~") || TOKEN("!") )
   {
	analex();
	return TRUE;
    }

   return FALSE;
}

postfix_expression()
{

   if ( ! primary_expression())
	return FALSE;

   do  
   {
     if ( TOKEN("["))
     {
	analex();
	if ( ! expression_aux())
	{
	   sintatic_error;
	}
	if (! TOKEN("]"))
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, "]", OP_POS2);
	analex();
	
     }
     else

     if (TOKEN("("))
     {
	analex();
	if (! argument_expression_list())
	   ins_expre(&expre_pos, "", OP_0_ARGUMENTO);
	if ( ! TOKEN(")"))
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, ")", OP_POS2);
	analex();
     }
     else

     if (TOKEN("."))
     {
	analex();
	if ( ! TOKEN("IDENT"))
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, saida.label, CAMPO);
	ins_expre(&expre_pos, ".", OP_POS2);
	analex();
     }
     else

     if (TOKEN("->"))
     {
	analex();
	if ( ! TOKEN("IDENT"))
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, saida.label, CAMPO);
 	ins_expre(&expre_pos, "->", OP_POS2);
	analex();
      }
     else

     if (TOKEN("++") || TOKEN("--"))
     {
	ins_expre(&expre_pos, saida.label, OP_POS1_ARIT);
	analex();
     }
     else
	break;
   } while (TRUE);

   return TRUE;
}

     
primary_expression()
{
struct tableout saida_aux;

   if (TOKEN("IDENT"))
   {
	reanalex();
	if (TOKEN("VARIABLE"))
	{
	   saida_aux = saida;
	   ins_expre(&expre_pos, saida.label, VARIAVEL);
	   analex();
	   if (TOKEN("("))
              cg_add_call(saida_aux.label);

	}
	else
	if (TOKEN("ENUM_CONST"))
	{
	   ins_expre(&expre_pos, saida.label, CONST_ENUM);
	   analex();
	}
	else
	{		/* function call */
           ins_expre(&expre_pos, saida.label, VARIAVEL);
	   cg_add_call(saida.label);
	   analex();	/* function */
	   if (! TOKEN("("))
	   {
		sintatic_error;
	   }
	}
   }
   else
   if (TOKEN("STRING"))
   {
	ins_expre(&expre_pos, saida.label, STRING);
	analex();
   }
   else
   if (TOKEN("INT_CONST"))
   {
	ins_expre(&expre_pos, saida.label, CONST_INT);
	analex();
   }
   else
   if (TOKEN("CHAR_CONST"))
   {
	ins_expre(&expre_pos, saida.label, CONST_CHAR);
	analex();
   }
   else
   if (TOKEN("FLOAT_CONST"))
   {
	ins_expre(&expre_pos, saida.label, CONST_FLOAT);
	analex();
   }
   else
   if (TOKEN("("))
   {
	analex();
	if ( ! expression_aux())
	{
	   sintatic_error;
	}
	if (! TOKEN(")"))
	{
	   sintatic_error;
	}
	analex();
   }
   else
	return FALSE;

   return TRUE;
}


argument_expression_list()
{

   if ( ! assigment_expression())
	return FALSE;

   while ( TOKEN(","))
   {
	analex();
	if ( ! assigment_expression())
	{
	   sintatic_error;
	}
	ins_expre(&expre_pos, ",", OP_2_ARGUMENTO);
   }

   return TRUE;
}



trata_break(int initnode, int breaknode)
{
int i;

    for (i = initnode; i < breaknode; i++)
	if (gfc_is_break(i) )
	{
	   gfc_add_aresta(i,breaknode);
	   gfc_break(i,FALSE);
	}
}

trata_continue(int contnode, int endnode)
{
int i;

   for (i = endnode; i > contnode; i--)
	if (gfc_is_continue(i) )
	{
	   gfc_add_aresta(i, contnode);
	   gfc_continue(i, FALSE);
	}
}


trata_empty(int ininode, int endnode, int qual, int por)
{
int i;

    for (i = endnode; i > ininode; i--)
       	if (gfc_has_aresta(i, qual) )
	{
	    gfc_del_aresta(i,qual);
	    gfc_add_aresta(i,por);
	}
}
