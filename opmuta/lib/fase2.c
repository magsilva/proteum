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
#include	<polonesa/polonesa.h>
#include	<li/lib/li.h>
#include	"opmuta.h"


extern	int	pt;		/* apontador p/ prog LI */

extern	int	flg_erro,	/* flag de erro */
		flg_dcl;	/* indica declaracao */

extern	int	nret, nlabel, nbreak, ncont;

extern	int	lastnode;

extern	LI_SIMBOLO	sym;	/* simbolo 'lido' */

extern	char	bufmen[];	/* buffer auxiliar global */

extern	EXPRE_POS	expre_pos;

EXPRE_POS	ex_aux;

extern	int	nivel;

extern	int	g_calling,
		g_unit,
		g_called;

extern	TABSIMBOLO	tab_vars, tab_tipos;

static	char	func_name[TAMLEX+1];

static int nesting;

static int ptprim, ptult, ptfecha;

int	passo;



gera_fase2()
{
   nret = nlabel = nbreak = ncont = 0;
   lastnode = -1;
   nesting = 0;
   passo = 0;
   init_compute_set();
   compute_setE();
   if (func_inic() == ERRO)
   {
	erro:
	lib_sets();
	msg("Sintatic Error in LI Program");
	return ERRO;
   }

   fase2_comando();
   func_fim();
   if (flg_erro)
	     goto erro;

   passo = 1;
   if (func_inic() == ERRO)
	goto erro;
   compute_setP();

   fase2_comando();
   func_fim();
   if (flg_erro)
             goto erro;

/*   print_sets(); */
   lib_sets();
   return OK;
}




fase2_comando()
{

   if (e_abre(sym))
	     fase2_comp();
	else
   if (e_sequencial(sym))
	     fase2_seq(0, OP_3);
	else
   if (e_if(sym))
	     fase2_if();
	else
   if (e_case(sym))
	     fase2_case();	
	else
   if (e_for(sym))
	     fase2_for();	
	else
   if (e_while(sym))
	     fase2_while();	
	else
   if (e_repeat(sym))
	     fase2_repeat();	
	else
   if (e_goto(sym))
	     fase2_goto();	
	else
   if (e_break(sym))
	     fase2_break();	
	else
   if (e_continue(sym))
	     fase2_continue();	
	else
   if (e_return(sym))
	     fase2_return();
	else
   if (e_label(sym))	
	     fase2_label();
	else
   if (e_dcl(sym))
	     func_dcl();
	else func_erro();

}



fase2_label()
{
   if (g_unit && passo == 0)
       init_sglr();
   sym = *(pli_getsym(++pt));
   fase2_comando();
}



/************************************************************************
Fase2_comp:
	Trata cada um dos possiveis comandos da LI

***********************************************************************/

fase2_comp()	/* elimina comando composto */
{  
  int pt00, pt01;

   pt++;
   nivel++;
   sym = *(pli_getsym(pt));
   pt01 = pt00 = pt;
   while (! e_fecha(sym) && ! pli_fim_func(pt))
      {
        pt00=pt;
	fase2_comando();
      }
   if (! e_fecha(sym))
	     func_erro();
   ptfecha = pt;
   ptult = pt00;
   ptprim = pt01;
   esvazia_tab(&tab_vars, nivel);  /*libera todos simbolos definidos no nivel */
   esvazia_tab(&tab_tipos, nivel);
   flg_dcl = TRUE;
   nivel--;
   pt++;
   sym = *(pli_getsym(pt));
}







fase2_if()
{
  LI_SIMBOLO  scond;

   sym = *(pli_getsym(++pt));
   if ( ! e_cond(sym))
	return func_erro();
   if (passo == 1 && g_unit){
       scond = sym;
       u_stri(&scond);
     }
     fase2_seq1(1, 0);
  
   fase2_comando();

   if ( e_else(sym))
	     {
		sym = *(pli_getsym(++pt));
		fase2_comando();
	      }
  ptprim = ptult = ptfecha = -1;
}


fase2_case()
{
int	tembreak;
int 	oldnesting;
int     cont;
LI_SIMBOLO scase;

   sym = *(pli_getsym(++pt));
   if (! e_cond(sym))
	return func_erro();
   fase2_seq1(0, OP_3);

   if ( ! e_abre(sym))
	     return func_erro();

   sym = *(pli_getsym(++pt));
         tembreak = TRUE;
   oldnesting = nesting;
   nesting = cont = 0;
   while ( e_rotc(sym))
     {
        if (passo == 1 && g_unit)
        {
            scase = sym;
            u_sswm(&scase, ++cont);
        }
	CovAllEdgeSw(tembreak);
	tembreak = FALSE;
      	sym = *(pli_getsym(++pt));
        
	while (! e_rotc(sym) && ! e_fecha(sym) && ! pli_fim_func(pt))
	   {
              
		fase2_comando();
	    }
	if (pli_fim_func(pt))
	        return func_erro();
     }
   nesting = oldnesting;

   sym = *(pli_getsym(++pt));
   ptprim = ptult = ptfecha = -1;

}


fase2_for()
{
LI_SIMBOLO sfor, scom, sfim, sout, sfecha;
int flag, ok;

   ok=FALSE;
   sfor = sym;
   sym = *(pli_getsym(++pt));

   if ( ! e_sequencial(sym))
	return func_erro();
   fase2_seq(0, OP_3);

   if (! e_cond(sym) )
	return func_erro();
   fase2_seq(1, 0);

   if ( ! e_sequencial(sym))
	return func_erro();
   fase2_seq1(0, OP_3);

   scom = sym;

            /*scom aponta para o elemento seguinte a o comando for*/
   nesting++;
   fase2_comando();
   nesting--;

   /*encontra simbolos chaves para u-smvb*/

   if(!pli_fim_func(pt) && ptprim>=0)
   {
      sfim= *(pli_getsym(ptult));
      sfecha = *(pli_getsym(ptfecha));
      sout=*(pli_getsym(ptfecha+1));
      ok= TRUE;

      flag=0;
    }

   if (passo == 1 && g_unit)
   {
	u_smtc(&sfor, &scom, &sym, &sym);
     	u_sbrn(nesting, &scom);
     	u_scrn(nesting, &scom);
        if (ok)
          u_smvb(flag, &sfor, &sfim, &sout, &sfecha, &sfecha);
   }
   if (nesting == 0)
	ncont = nbreak = 0;
  ptprim = ptult = ptfecha = -1;
 }



fase2_while()
{
LI_SIMBOLO sw, scond, scoman, sfim, sout, sfecha;
int flag, ok;
    ok = FALSE;
   sw = sym;          /*sw eh o comando while*/
   sym = *(pli_getsym(++pt));

   if ( ! e_cond(sym))
	return func_erro();
   scond = sym;       /*scond eh a condicao*/
   fase2_seq1(1, 0);
   scoman = sym;      /*scoman eh o primeiro comando do laco*/
 
   nesting++;
   fase2_comando();
   nesting--;

    /*encontra simbolos chaves para u-smvb*/
   if(!pli_fim_func(pt) && ptprim>=0)
   {
      sfim= *(pli_getsym(ptult));
      sfecha = *(pli_getsym(ptfecha));
      sout=*(pli_getsym(ptfecha+1));
      ok=TRUE;
      flag=0;
    }

   if (passo == 1 && g_unit)
   {
   	/* /swhile = sym; */
        u_swdd(&sw, &scond, &sym);
        /* /sdo = sym; */
	u_smtc(&sw, &scoman, &sym, &sym);
     	u_sbrn(nesting, &scoman);
     	u_scrn(nesting, &scoman);
        if(ok)
         u_smvb(flag, &sw, &sfim, &sout, &sfecha, &sfecha);
   }

   if (nesting == 0)
	ncont = nbreak = 0;
  ptprim = ptult = ptfecha = -1;
}


fase2_repeat()
{
LI_SIMBOLO sdo, sc, sw, sfim, sout, scond, sfecha, scout, keep;
int flag, ptcond, ok, ptkeep, passo1;

   ok = FALSE;   
   sdo = sym;         
   sc = sym = *(pli_getsym(++pt));

   nesting++;
   fase2_comando();
   nesting--;
    
   if ( ! e_until(sym))
	return func_erro();
   

   if (passo == 1 && g_unit)
   {
      	u_sbrn(nesting, &sc);
      	u_scrn(nesting, &sc);
        
   }
   if (nesting == 0)
	ncont = nbreak = 0;

   sw = sym;
   sym = *(pli_getsym(++pt));

   if (! e_cond(sym))
	return func_erro();
   scond = sym;
   ptcond=pt; 
  
   
   /*encontra simbolos chaves para u-smvb*/
   if(!pli_fim_func(pt) && ptprim>=0)
   {
      sfim= *(pli_getsym(ptult));
      sfecha = *(pli_getsym(ptfecha));
      sout=*(pli_getsym(ptcond+1));
      ok=TRUE;
      flag=1;
    }


   if (passo == 1 && g_unit)
   {
      u_sdwd(&sdo, &sc, &sw, &sym);
      if(ok)
          u_smvb(flag, &sdo, &sfim, &sout, &sfecha, &scond);
    }
   fase2_seq(1, 0);
   if (passo == 1 && g_unit)
   	u_smtc(&sdo, &sc, &sw, &sym);
  ptprim = ptult = ptfecha = -1;

}


fase2_goto()
{
LI_SIMBOLO	s;
int k=0;
   s = sym;
   sym = *(pli_getsym(++pt));

   if (! e_label(sym))
	return func_erro();
        if (passo == 1 && g_unit)
        {
               u_sglr();
         }
   if (passo == 1 && g_called)
      CovAllNode_goto(&s);

   sym = *(pli_getsym(++pt));
  ptprim = ptult = ptfecha = -1;

}


fase2_break()
{
   if (passo == 1 && g_called)
        CovAllNode_stat();
   if (passo == 1 && g_unit && nesting > 0)
   {
	init_sbrn(nesting);
	u_sbrc();
   }
   sym = *(pli_getsym(++pt));
  ptprim = ptult = ptfecha = -1;

}

fase2_continue()
{
   if (passo == 1 && g_called)
        CovAllNode_stat();
   if (passo == 1 && g_unit)
   {
	init_scrn(nesting);
	u_scrb();
   }
   sym = *(pli_getsym(++pt));
  ptprim = ptult = ptfecha = -1;

}


fase2_seq1(int edgecov, int op_troca)
{
   if (carrega_fonte(bufmen, &sym) == ERRO)
             return func_erro();

   gera_expre(bufmen);      /* monta expressao posfixa */

   if (g_calling)
   {
	if (passo == 1)
   	{
	   fase2_apply_calling("(", ")");
	}
   }
   if (g_called || g_unit)
   {
        if (passo == 0)
        {
           compute_setL();
           compute_setGC();
        }
        else
        {
           compute_setL();
           if (g_called)
		fase2_apply_called("(", ")", FALSE);
	   if (g_unit)
		fase2_apply_unit("(", ")", op_troca);
	   if (edgecov)
		CovAllEdge("(", ")");
	   CovAllNode("(", ")");
        }
   }

   sym = *(pli_getsym(++pt));
   ptprim = ptult = ptfecha = -1;

}


fase2_seq(int edgecov, int op_troca)
{

   if (carrega_fonte(bufmen, &sym) == ERRO)
             return func_erro();

   gera_expre(bufmen);      /* monta expressao posfixa */
	
   if (g_calling)
   {
        if (passo == 1)
        {
           fase2_apply_calling("", ";");
        }
   }
   if (g_called || g_unit)
   {
        if (passo == 0)
        {
           compute_setL();
           compute_setGC();
        }
        else
        {
           compute_setL();
	   if (g_called)
		fase2_apply_called("", ";", FALSE);
	   if (g_unit)
		fase2_apply_unit("", ";", op_troca);
           if (edgecov)
                CovAllEdge("", ";");
           CovAllNode("", ";");
	}
   }

   sym = *(pli_getsym(++pt));
  ptprim = ptult = ptfecha = -1;

}



fase2_return()
{


   if (carrega_fonte(bufmen, &sym) == ERRO)
 	     return func_erro();

   gera_expre(&bufmen[6]);	/* monta expressao posfixa */

   if (g_calling)
   {
        if (passo == 1)
        {
           fase2_apply_calling("return", ";");
        }
   }
   if (g_called || g_unit)
   {
        if (passo == 0)
        {
           compute_setL();
           compute_setGC();
	   init_RepRetStat();
        }
        else
        {
           compute_setL();
           if (g_called)
	   {
		fase2_apply_called("return", ";", TRUE);
	   	DelRetStat();
	   	RepRetStat();
	   	CovAllNode_stat();
	   }
	   if (g_unit)
	   {
		fase2_apply_unit("return", ";", OP_3);
	   }
        }
   }

   sym = *(pli_getsym(++pt));
  ptprim = ptult = ptfecha = -1;

}




fase2_apply_calling(char *pre, char *pos)
{

      RepArgReq(pre, pos);
      ArgIncrDecr(pre, pos);
      ArgArithNeg(pre, pos);
      ArgLogNeg(pre, pos);
      ArgBitNeg(pre, pos);
      DelArg(pre, pos);
      DelFuncCall(pre, pos);
      SwitchAlikeArg(pre, pos);
      SwitchDifArg(pre, pos);
}


fase2_apply_called(char *pre, char *pos, int ret)
{
	DirVarRepPar(pre, pos);
	DirVarRepReq(pre, pos);
        DirVarRepLoc(pre, pos);
        DirVarRepExt(pre, pos);
        DirVarRepGlob(pre, pos);
	DirVarRepConst(pre, pos);
	DirVarIncrDecr(pre, pos);
        DirVarArithNeg(pre, pos);
        DirVarBitNeg(pre, pos);
        DirVarLogNeg(pre, pos);

        IndVarRepPar(pre, pos, ret);
        IndVarRepReq(pre, pos, ret);
        IndVarRepLoc(pre, pos, ret);
        IndVarRepExt(pre, pos, ret);
        IndVarRepGlob(pre, pos, ret);
        IndVarRepConst(pre, pos, ret);
	IndVarIncrDecr(pre, pos, ret);
	IndVarArithNeg(pre, pos, ret);
        IndVarBitNeg(pre, pos, ret);
        IndVarLogNeg(pre, pos, ret);

}

fase2_apply_unit(char *pre, char *pos, int op_troca)
{
	u_cccr(pre, pos);
	u_ccsr(pre, pos);
	u_crcr(pre, pos);
	u_oaaa(pre, pos);
	u_oaan(pre, pos);
	u_oaba(pre, pos);
        u_oabn(pre, pos); 
        u_oaea(pre, pos); 
        u_oaln(pre, pos);
        u_oarn(pre, pos);
        u_oasa(pre, pos);
        u_oasn(pre, pos);
        u_obaa(pre, pos);
        u_oban(pre, pos);
	u_obba(pre, pos);
   	u_obbn(pre, pos);
        u_obea(pre, pos);
        u_obln(pre, pos);
        u_obrn(pre, pos);
        u_obsa(pre, pos);
        u_obsn(pre, pos);
        u_oeaa(pre, pos);
	u_oeba(pre, pos);
	u_oesa(pre, pos);
        u_olan(pre, pos);
 	u_olbn(pre, pos);
    	u_olln(pre, pos);
  	u_olrn(pre, pos);
	u_olsn(pre, pos);
	u_oran(pre, pos);
	u_orbn(pre, pos);
	u_orln(pre, pos);
	u_orrn(pre, pos);
	u_orsn(pre, pos);
	u_osaa(pre, pos);
	u_osan(pre, pos);
	u_osba(pre, pos);
	u_osbn(pre, pos);
	u_osea(pre, pos);
	u_osln(pre, pos);
	u_osrn(pre, pos);
	u_ossa(pre, pos);
	u_ossn(pre, pos);
	u_ocng(pre, pos, op_troca);
    	u_olng(pre, pos);
	u_obng(pre, pos);    
	u_oido(pre, pos);
	u_oipm(pre, pos);
        u_ocor(pre, pos);
	u_vgar(pre, pos);
	u_vlar(pre, pos);
	u_vgsr(pre, pos);
	u_vlsr(pre, pos);
	u_vgpr(pre, pos);
	u_vlpr(pre, pos);
        u_vgtr(pre, pos);
	u_vltr(pre, pos);
	u_vscr(pre, pos);
	u_vdtr(pre, pos);
	u_vtwd(pre, pos);
}
