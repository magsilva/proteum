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
#include <string.h>
#include "li.h" 

int	nnodes;		/* nmber of nodes */
int	contreturn;	/* number of returns in a function */

struct node {
	struct {
	   unsigned char empty:2;
	   unsigned char chega:1;
	   unsigned char bbreak:1;
	   unsigned char bcase:1;
	   unsigned char bcontinue:1;
	   unsigned char breturn:1;
        }	flg;
	OSET	def;
	OSET	udef;
	OSET	use;
	OSET	next;
	};

#define		GOTONUMB	256

struct gotonode {
       	int	node;
        char	label[TAMLEX+1];
         } tabgoto[GOTONUMB];

struct gotonode tablabel[GOTONUMB];

int	ngoto, nlabel;

struct node	tabnodes[4096];

extern	int	flg_gera, flg_no_empty;	/* indica se deve gerar li */
extern  FILE	*arqgfc;

int gfc_begin_function(nome)
char	*nome;
{
int	i;

   nnodes = contreturn = ngoto = nlabel = 0;
   for (i = 0; i < GOTONUMB; i++)
     tablabel[i].node = tabgoto[i].node = -1;
   if (flg_gera)
   {
	fprintf(arqgfc, "@%s\n",nome); 
   }
   return gfc_new_node();
}


char *find_no_empty(char *s, OSET *x)
{
int i;

    while ( s != NULL && set_inn(x, s) >= 0)
    {
        i = atoi(s) - 1
        ;
        s = set_get(&(tabnodes[i].next), 0);
    }
    return s;

}

/**
 * Remove empty nodes from the graph.
 */
int remove_empty() {
    OSET list_empty;

    // Collect all empty nodes
    set_new(&list_empty);
    for (int i = 0; i < nnodes; i++) {
        if (gfc_is_empty(i) == EMPTY) {
            char[MAX_NUMBER_STRING_LENGTH] data;
            snprintf(data, MAX_NUMBER_STRING_LENGTH, "%d", i + 1);
            set_add(&list_empty, data);
        }
    }

    /* Find nodes that are linked to empty nodes and remove that edge */
    for (int i = 0; i < nnodes; i++) {
	int j = 0;
        char * p = set_get(&(tabnodes[i].next), j);
	while (p != NULL) {
	    if (set_inn(&list_empty, p) >= 0) {
                char * s = find_no_empty(p, &list_empty);
                gfc_del_aresta(i, atoi(p) - 1);
                if (s != NULL) {
                    gfc_add_aresta(i, atoi(s) - 1);
                }
                j--;
            }
	    j++;
	    p = set_get(&(tabnodes[i].next), j);
	}
    }
}


int gfc_fim_function()
{
int i, j;
char *p;

if (flg_gera)
{
    if (flg_no_empty)
        remove_empty();
   fprintf(arqgfc, "%d\n", nnodes);
   for (i = 0; i < nnodes; i++)
   {
	fprintf(arqgfc, "\n%d", i+1);
        fprintf(arqgfc, "\n\tDef: ");
	for (p = set_get(&(tabnodes[i].def), j = 0); p != NULL; 
	     p = set_get(&(tabnodes[i].def), ++j) )
	{
	   fprintf(arqgfc, "%s ", p);
	}
	set_free(&(tabnodes[i].def));
        fprintf(arqgfc, "\n\tUse: ");
	for (p = set_get(&(tabnodes[i].use), j = 0); p != NULL; 
	     p = set_get(&(tabnodes[i].use), ++j) )
	{
	   fprintf(arqgfc, "%s ", p);
	}
	set_free(&(tabnodes[i].use));
        fprintf(arqgfc, "\n\t");
	for (p = set_get(&(tabnodes[i].next), j = 0); p != NULL; 
	     p = set_get(&(tabnodes[i].next), ++j) )
	{
	   fprintf(arqgfc, "%s ", p);
	}
	fprintf(arqgfc, "0");
	set_free(&(tabnodes[i].next));
   }
   fprintf(arqgfc, "\n");
}
else
{
   for (i = 0; i < nnodes; i++)
   {
	set_free(&(tabnodes[i].def));
	set_free(&(tabnodes[i].use));
	set_free(&(tabnodes[i].next));
   }
}
return OK;
}



int gfc_new_node()
{
   set_new(&tabnodes[nnodes].def);
   set_new(&tabnodes[nnodes].use);
   set_new(&tabnodes[nnodes].next);
   tabnodes[nnodes].flg.empty = EMPTY;
   tabnodes[nnodes].flg.chega = FALSE;
   tabnodes[nnodes].flg.bbreak = FALSE;
   tabnodes[nnodes].flg.bcontinue = FALSE;
   tabnodes[nnodes].flg.bcase = FALSE;
   tabnodes[nnodes].flg.breturn = FALSE;
   return nnodes++;
}

int gfc_release(int node)
{
   if (node >= nnodes)
	return -1;
   set_free(&(tabnodes[node].def));
   set_free(&(tabnodes[node].use));
   set_free(&(tabnodes[node].next));
   nnodes = node;
   return nnodes;
}


int gfc_no_empty(int x, int k)
{
   if (x < 0 || x >= nnodes)
	return -1;
   tabnodes[x].flg.empty = k;
   return 0;
}

int gfc_is_empty(int x)
{
   if (x < 0 || x >= nnodes)
	return EMPTY;
   return tabnodes[x].flg.empty;
}

int gfc_chega(int x)
{
   if (x < 0 || x >= nnodes)
	return FALSE;
   return tabnodes[x].flg.chega;
}



int gfc_curnode()
{
   return nnodes-1;
}


/**
 * Add edge between two nodes.
 */
int gfc_add_aresta(int node1, int node2)
{
    int result;
    char[MAX_NUMBER_STRING_LENGTH] data;
    snprintf(data, MAX_NUMBER_STRING_LENGTH, "%d", node2 + 1);
 
    if (set_inn(&(tabnodes[node1].next), data) < 0) {	
    	set_add(&(tabnodes[node1].next), data);
	result = 1;
    } else {
	result = 0;
    }
    tabnodes[node2].flg.chega = TRUE;

    return result;
}

/**
 * Remove edge between two nodes.
 */
int gfc_del_aresta(int node1, int node2)
{
    int i;
    char[MAX_NUMBER_STRING_LENGTH] data;

    snprintf(data, MAX_NUMBER_STRING_LENGTH, "%d", node2 + 1);
    i = set_inn(&(tabnodes[node1].next), data);
    if (i < 0) {
	return -1;
    }
    set_del(&(tabnodes[node1].next), i);

    return 0;
}

/**
 * Remove edges from a node.
 */
void gfc_remove_arestas(int node)
{
    set_free(&(tabnodes[node].next));
    set_new(&(tabnodes[node].next));
}

/**
 * Check if there is a edge between two nodes.
 */
int gfc_has_aresta(int node1, int node2)
{
    int i;
    char[MAX_NUMBER_STRING_LENGTH] data;

    snprintf(data, MAX_NUMBER_STRING_LENGTH, "%d", node2 + 1);
    i = set_inn(&(tabnodes[node1].next), data);

    return (i >= 0);
}




int gfc_case(int node, int torf)
{
   if (node < nnodes & node >= 0)
	tabnodes[node].flg.bcase = torf;
}

int gfc_is_case(int node, int torf)
{
   if (node < nnodes & node >= 0)
	return tabnodes[node].flg.bcase;
   return FALSE;
}


int gfc_break(int node, int torf)
{
   if (node < nnodes & node >= 0)
	tabnodes[node].flg.bbreak = torf;
}

int gfc_is_break(int node, int torf)
{
   if (node < nnodes & node >= 0)
	return tabnodes[node].flg.bbreak;
   return FALSE;
}

int gfc_continue(int node, int torf)
{
   if (node < nnodes & node >= 0)
	tabnodes[node].flg.bcontinue = torf;
}

int gfc_is_continue(int node, int torf)
{
   if (node < nnodes & node >= 0)
	return tabnodes[node].flg.bcontinue;
   return FALSE;
}

int gfc_return(int node, int torf)
{
   if (node < nnodes & node >= 0)
   {
	tabnodes[node].flg.breturn = torf;
	contreturn++;
   }
}

int gfc_has_return()
{
   return contreturn > 0;
}


int gfc_is_return(int node, int torf)
{
   if (node < nnodes & node >= 0)
	return tabnodes[node].flg.breturn;
   return FALSE;
}


int gfc_label(int node, char *label)
{
int i; 

    for (i = 0; i < nlabel && tablabel[i].node >= 0; i++)
	if (strcmp(tablabel[i].label, label) == 0)
	    return -1;
    if ( tablabel[i].node >= 0)
	return -1;
     tablabel[i].node = node;
     strcpy(tablabel[i].label,label);
     nlabel++;
     for (i = 0; i < GOTONUMB; i++)
         if (tabgoto[i].node >= 0 && strcmp(tabgoto[i].label,label) == 0)
         {
	      	gfc_add_aresta(tabgoto[i].node,node);
		tabgoto[i].node = -1;
		ngoto--;
         }
     return 0;
}


int gfc_get_label(char *label)
{
int i; 

    for (i = 0; i < nlabel; i++)
	if (strcmp(tablabel[i].label, label) == 0)
	    return tablabel[i].node;
    return -1;
}


int gfc_goto(int node, char *label)
{
int i; 

    for (i = 0; i < GOTONUMB && tabgoto[i].node >= 0; i++);
    if ( tabgoto[i].node >= 0)
	return -1;
     tabgoto[i].node = node;
     strcpy(tabgoto[i].label,label);
     ngoto++;
     return 0;
}

