OS = LINUX
#OS = SUNOS5

PROTEU = .

LIB = $(PROTEU)/lib

PTESTE = $(PROTEU)/pteste/lib

TCASE = $(PROTEU)/tcase/lib

MUTA = $(PROTEU)/muta/lib

OPMUTA = $(PROTEU)/opmuta/lib

EXEMUTA = $(PROTEU)/exemuta

CHECKEQ = $(PROTEU)/checkequiv

REPORT = $(PROTEU)/report

INSTRUM = $(PROTEU)/instrum

POLONESA = $(PROTEU)/polonesa

UTIL = $(PROTEU)/util

MISC = $(PROTEU)/misc

LI = $(PROTEU)/li/lib

BTREE = $(PROTEU)/btree

BIN = $(PROTEU)/$(OS)/bin

OBJ = $(PROTEU)/$(OS)/obj


INCLUDEFILE = $(LIB)/gerais.h $(PTESTE)/pteste.h $(TCASE)/tcase.h \
	      $(MUTA)/muta.h $(BTREE)/btree.h $(LIB)/globais.h \
	      $(POLONESA)/polonesa.h $(LI)/li.h $(OPMUTA)/opmuta.h


CC = gcc

LD = ld 

CFLAGS = -c -g -I$(PROTEU) -D$(OS)

LFLAGS = -r 

CLFLAGS = -g -I$(PROTEU) -D$(OS)

CURSES = curses

ALL =  li pteste tcase muta exemuta report opmuta util misc btree \
	checkequiv instrum 

all: $(ALL)
	echo DONE

lib:	disco  gerais  globais  ttymodes
	$(LD) $(LFLAGS) $(OBJ)/disco.o $(OBJ)/gerais.o \
	$(OBJ)/globais.o  $(OBJ)/ttymodes.o -o $(OBJ)/libgerais.o

disco:  $(OBJ)/disco.o 


$(OBJ)/disco.o: $(LIB)/disco.c $(LIB)/gerais.h
	$(CC) $(CFLAGS) $(LIB)/disco.c -o $(OBJ)/disco.o
 
gerais:  $(OBJ)/gerais.o
	# Compiling gerais.c	

$(OBJ)/gerais.o: $(LIB)/gerais.c $(LIB)/gerais.h $(LIB)/globais.h
	$(CC) $(CFLAGS) $(LIB)/gerais.c -o $(OBJ)/gerais.o

globais: $(OBJ)/globais.o
	# Compiling globais.c

$(OBJ)/globais.o:  $(LIB)/globais.c $(LIB)/gerais.h $(LIB)/globais.h
	$(CC) $(CFLAGS) $(LIB)/globais.c -o $(OBJ)/globais.o

ttymodes: $(OBJ)/ttymodes.o
	# Compiling globais.c

$(OBJ)/ttymodes.o:  $(LIB)/ttymodes.c 
	$(CC) $(CFLAGS) $(LIB)/ttymodes.c -o $(OBJ)/ttymodes.o

li:	$(INCLUDEFILE) $(LI)/../mainli.c libli lib polonesa
	$(CC) $(CFLAGS) $(LI)/../mainli.c -o $(OBJ)/mainli.o ;\
	$(CC) $(CLFLAGS) $(OBJ)/mainli.o $(OBJ)/libli.o $(OBJ)/libgerais.o \
	$(OBJ)/libpolonesa.o -o $(BIN)/li

libli:	lli simbtab analex rotsem \
	gravali parser callgr gfc
	$(LD) $(LFLAGS) $(OBJ)/li.o $(OBJ)/simbtab.o \
	$(OBJ)/analex.o $(OBJ)/callgr.o \
	$(OBJ)/rotsem.o $(OBJ)/gravali.o $(OBJ)/parser.o  \
	$(OBJ)/gfc.o -o $(OBJ)/libli.o

lli: $(OBJ)/li.o
	# Compiling nanasin.c

$(OBJ)/li.o:	$(INCLUDEFILE) $(LI)/nanasin.c
	$(CC) $(CFLAGS) $(LI)/nanasin.c -o $(OBJ)/li.o

simbtab: $(OBJ)/simbtab.o
	# Compiling simbtab.c

$(OBJ)/simbtab.o: $(INCLUDEFILE) $(LI)/simbtab.c
	$(CC) $(CFLAGS) $(LI)/simbtab.c -o $(OBJ)/simbtab.o

analex: $(OBJ)/analex.o
	 # Compiling analex.c

$(OBJ)/analex.o: $(INCLUDEFILE) $(LI)/analex.c
	$(CC) $(CFLAGS) $(LI)/analex.c -o $(OBJ)/analex.o

rotsem:	$(OBJ)/rotsem.o
	 # Compiling rotsem.c

$(OBJ)/rotsem.o: $(INCLUDEFILE) $(LI)/rotsem.c
	$(CC) $(CFLAGS) $(LI)/rotsem.c -o $(OBJ)/rotsem.o

callgr: $(OBJ)/callgr.o
	# Compiling callgr.c

$(OBJ)/callgr.o: $(INCLUDEFILE) $(LI)/callgr.c
	$(CC) $(CFLAGS) $(LI)/callgr.c -o $(OBJ)/callgr.o

gfc: $(OBJ)/gfc.o
	# Compiling gfc.c

$(OBJ)/gfc.o: $(INCLUDEFILE) $(LI)/gfc.c
	$(CC) $(CFLAGS) $(LI)/gfc.c -o $(OBJ)/gfc.o

gravali: $(OBJ)/gravali.o
	# Compiling gravali.c 

$(OBJ)/gravali.o: $(INCLUDEFILE) $(LI)/gravali.c
	$(CC) $(CFLAGS) $(LI)/gravali.c -o $(OBJ)/gravali.o

parser: $(OBJ)/parser.o
	# Compliling parser.c

$(OBJ)/parser.o: $(INCLUDEFILE) $(LI)/parser.c
	$(CC) $(CFLAGS) $(LI)/parser.c -o $(OBJ)/parser.o

polonesa: $(INCLUDEFILE) polo pgerais extipo
	$(LD) $(LFLAGS) $(OBJ)/polonesa.o $(OBJ)/pgerais.o $(OBJ)/extipo.o \
	-o $(OBJ)/libpolonesa.o

polo: $(OBJ)/polonesa.o
	# Compiling polonesa.c

$(OBJ)/polonesa.o:	$(INCLUDEFILE) $(POLONESA)/polonesa.c
	$(CC) $(CFLAGS) $(POLONESA)/polonesa.c -o $(OBJ)/polonesa.o

pgerais: $(OBJ)/pgerais.o
	# Compiling pgerais.c

$(OBJ)/pgerais.o: $(INCLUDEFILE) $(POLONESA)/pgerais.c
	$(CC) $(CFLAGS) $(POLONESA)/pgerais.c -o $(OBJ)/pgerais.o

extipo: $(OBJ)/extipo.o
	# Compiling extipo.c

$(OBJ)/extipo.o: $(INCLUDEFILE) $(POLONESA)/extipo.c
	$(CC) $(CFLAGS) $(POLONESA)/extipo.c -o $(OBJ)/extipo.o

li2nli: $(BIN)/li2nli
	# Compiling li2nli


muta: $(INCLUDEFILE) $(MUTA)/../mainmuta.c lib $(OBJ)/libmuta.o \
	$(OBJ)/libpteste.o  
	$(CC) $(CFLAGS) $(MUTA)/../mainmuta.c -o $(OBJ)/mainmuta.o  ;\
	$(CC) $(CLFLAGS) $(OBJ)/mainmuta.o $(OBJ)/libgerais.o $(OBJ)/libmuta.o \
	$(OBJ)/libpteste.o  -o $(BIN)/muta

$(OBJ)/libmuta.o: btree $(OBJ)/arqmuta.o $(OBJ)/arqfunc.o
	$(LD) $(LFLAGS) $(OBJ)/arqmuta.o $(OBJ)/libbtree.o \
	$(OBJ)/arqfunc.o -o $(OBJ)/libmuta.o

$(OBJ)/arqmuta.o: $(INCLUDEFILE) $(MUTA)/arqmuta.c
	$(CC) $(CFLAGS) $(MUTA)/arqmuta.c -o $(OBJ)/arqmuta.o 

$(OBJ)/arqfunc.o: $(INCLUDEFILE) $(MUTA)/arqfunc.c
	$(CC) $(CFLAGS) $(MUTA)/arqfunc.c -o $(OBJ)/arqfunc.o 

tcase: $(INCLUDEFILE) $(TCASE)/../maintcas.c lib $(OBJ)/libtcase.o \
	$(OBJ)/libpteste.o lib
	$(CC) $(CFLAGS) $(TCASE)/../maintcas.c -o $(OBJ)/maintcas.o  ;\
	$(CC) $(CLFLAGS) $(OBJ)/maintcas.o $(OBJ)/libgerais.o \
	$(OBJ)/libtcase.o \
	$(OBJ)/libpteste.o -o $(BIN)/tcase

$(OBJ)/libtcase.o: $(INCLUDEFILE) $(TCASE)/arqtcase.c $(TCASE)/arqio.c \
	$(TCASE)/tcase_ex.c $(TCASE)/log.c $(TCASE)/playinput.c \
	$(TCASE)/recinput.c
	$(CC) $(CFLAGS) $(TCASE)/arqtcase.c -o $(OBJ)/arqtcase.o ;\
	$(CC) $(CFLAGS) $(TCASE)/arqio.c -o $(OBJ)/arqio.o ;\
	$(CC) $(CFLAGS) $(TCASE)/tcase_ex.c -o $(OBJ)/tcase_ex.o ;\
	$(CC) $(CFLAGS) $(TCASE)/log.c -o $(OBJ)/log.o ;\
	$(CC) $(CFLAGS) $(TCASE)/recinput.c -o $(OBJ)/recinput.o ;\
	$(CC) $(CFLAGS) $(TCASE)/playinput.c -o $(OBJ)/playinput.o ;\
	$(LD) $(LFLAGS) $(OBJ)/arqio.o $(OBJ)/arqtcase.o -o $(OBJ)/libtcase.o \
	$(OBJ)/log.o $(OBJ)/tcase_ex.o $(OBJ)/recinput.o $(OBJ)/playinput.o

pteste: $(INCLUDEFILE) $(PTESTE)/../mainptes.c lib $(OBJ)/libpteste.o lib
	$(CC) $(CFLAGS) $(PTESTE)/../mainptes.c -o $(OBJ)/mainptes.o ;\
	$(CC) $(CLFLAGS) $(OBJ)/mainptes.o $(OBJ)/libgerais.o \
	$(OBJ)/libpteste.o -o $(BIN)/pteste

$(OBJ)/libpteste.o: $(PTESTE)/pteste.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(PTESTE)/pteste.c -o $(OBJ)/libpteste.o

checkequiv: $(CHECKEQ)/maincheq.c $(INCLUDEFILES) $(OBJ)/libtcase.o \
		$(OBJ)/libmuta.o lib
	$(CC) $(CFLAGS) $(CHECKEQ)/maincheq.c -o $(OBJ)/maincheq.o; \
	$(CC) $(CLFLAGS) $(OBJ)/maincheq.o $(OBJ)/libgerais.o \
        $(OBJ)/libtcase.o $(OBJ)/libmuta.o -o $(BIN)/check-equiv

instrum: $(OBJ)/maininst.o $(OBJ)/instrum.o $(OBJ)/instrumbuild.o $(OBJ)/libpteste.o \
         $(INCLUDEFILES) lib
	$(CC) $(CFLAGS) $(INSTRUM)/maininst.c -o $(OBJ)/maininst.o; \
	$(CC) $(CLFLAGS) $(OBJ)/maininst.o \
	$(OBJ)/instrum.o $(OBJ)/instrumbuild.o $(OBJ)/libgerais.o -o $(BIN)/instrum \
        $(OBJ)/libpteste.o

$(OBJ)/maininst.o: $(INSTRUM)/maininst.c $(INCLUDEFILES)
	$(CC) $(CFLAGS) $(INSTRUM)/maininst.c -o $(OBJ)/maininst.o

$(OBJ)/instrum.o: $(INSTRUM)/instrum.c $(INCLUDEFILES)
	$(CC) $(CFLAGS) $(INSTRUM)/instrum.c -o $(OBJ)/instrum.o
 
$(OBJ)/instrumbuild.o: $(INSTRUM)/instrumbuild.c $(INCLUDEFILES)
	$(CC) $(CFLAGS) $(INSTRUM)/instrumbuild.c -o $(OBJ)/instrumbuild.o

exemuta: $(OBJ)/exemuta.o $(OBJ)/selemuta.o $(OBJ)/mainexmu.o \
	 $(OBJ)/montamuta.o lib $(OBJ)/libpteste.o $(OBJ)/exelog.o \
	 $(OBJ)/libtcase.o $(OBJ)/libmuta.o
	$(CC) $(CLFLAGS) $(OBJ)/exemuta.o $(OBJ)/selemuta.o $(OBJ)/mainexmu.o \
	$(OBJ)/montamuta.o $(OBJ)/exelog.o \
	$(OBJ)/libpteste.o $(OBJ)/libgerais.o $(OBJ)/libtcase.o \
	$(OBJ)/libmuta.o  -o $(BIN)/exemuta

$(OBJ)/mainexmu.o: $(INCLUDEFILE) $(EXEMUTA)/mainexmu.c
	$(CC) $(CFLAGS) $(EXEMUTA)/mainexmu.c -o $(OBJ)/mainexmu.o

$(OBJ)/exemuta.o: $(INCLUDEFILE) $(EXEMUTA)/exemuta.c
	$(CC) $(CFLAGS) $(EXEMUTA)/exemuta.c -o $(OBJ)/exemuta.o

$(OBJ)/selemuta.o: $(INCLUDEFILE) $(EXEMUTA)/selemuta.c
	$(CC) $(CFLAGS) $(EXEMUTA)/selemuta.c -o $(OBJ)/selemuta.o

$(OBJ)/montamuta.o: $(INCLUDEFILE) $(EXEMUTA)/montamuta.c
	$(CC) $(CFLAGS) $(EXEMUTA)/montamuta.c -o $(OBJ)/montamuta.o

$(OBJ)/exelog.o: $(INCLUDEFILE) $(EXEMUTA)/exelog.c
	$(CC) $(CFLAGS) $(EXEMUTA)/exelog.c -o $(OBJ)/exelog.o


report: $(OBJ)/libmuta.o lib $(OBJ)/libtcase.o \
	$(OBJ)/libpteste.o $(OBJ)/mainrep.o $(OBJ)/report.o $(OBJ)/reptrace.o
	$(CC) $(CLFLAGS) $(OBJ)/libmuta.o $(OBJ)/libgerais.o $(OBJ)/libtcase.o \
	$(OBJ)/libpteste.o $(OBJ)/mainrep.o $(OBJ)/report.o $(OBJ)/reptrace.o \
	-o $(BIN)/report 

$(OBJ)/mainrep.o: $(INCLUDEFILE) $(REPORT)/mainrep.c
	$(CC) $(CFLAGS) $(REPORT)/mainrep.c -o $(OBJ)/mainrep.o

$(OBJ)/report.o: $(INCLUDEFILE) $(REPORT)/report.c
	$(CC) $(CFLAGS) $(REPORT)/report.c -o $(OBJ)/report.o

$(OBJ)/reptrace.o: $(INCLUDEFILE) $(REPORT)/reptrace.c
	$(CC) $(CFLAGS) $(REPORT)/reptrace.c -o $(OBJ)/reptrace.o

opmuta: $(INCLUDEFILE) $(OPMUTA)/../mainopmu.c lib libli polonesa \
	$(OBJ)/libopmuta.o btree
	$(CC) $(CFLAGS) $(OPMUTA)/../mainopmu.c -o $(OBJ)/mainopmu.o
	$(CC) $(CLFLAGS) $(OBJ)/mainopmu.o $(OBJ)/libopmuta.o \
	$(OBJ)/libli.o $(OBJ)/libgerais.o $(OBJ)/libpolonesa.o \
	$(OBJ)/libbtree.o -o $(BIN)/opmuta

$(OBJ)/libopmuta.o: $(OBJ)/mutgera.o $(OBJ)/glob.o $(OBJ)/opmuta.o \
	$(OBJ)/fase1.o $(OBJ)/fase2.o $(OBJ)/loadfunc.o $(OBJ)/tmpcon.o \
	$(OBJ)/reqcons.o $(OBJ)/reparg.o $(OBJ)/argincr.o $(OBJ)/argneg.o \
	$(OBJ)/delarg.o $(OBJ)/delfunc.o $(OBJ)/swalike.o $(OBJ)/swdif.o \
	$(OBJ)/varset.o $(OBJ)/varrepp.o $(OBJ)/varrepc.o $(OBJ)/varrepg.o \
	$(OBJ)/varrepl.o $(OBJ)/varrepe.o $(OBJ)/varrepr.o $(OBJ)/varincr.o \
	$(OBJ)/varneg.o $(OBJ)/delret.o $(OBJ)/repret.o $(OBJ)/covnode.o \
	$(OBJ)/u-cccr.o $(OBJ)/u-ocng.o $(OBJ)/u-ccsr.o $(OBJ)/u-crcr.o \
	$(OBJ)/u-obom.o $(OBJ)/u-olbng.o $(OBJ)/u-oido.o $(OBJ)/u-oipm.o \
        $(OBJ)/u-ocor.o $(OBJ)/u-strp.o $(OBJ)/u-sbrc.o $(OBJ)/u-varr.o \
        $(OBJ)/u-vsrr.o $(OBJ)/u-vprr.o $(OBJ)/u-vscr.o $(OBJ)/u-vtrr.o \
        $(OBJ)/u-sglr.o $(OBJ)/u-vdtr.o $(OBJ)/u-smtc.o $(OBJ)/u-stri.o \
        $(OBJ)/u-sdwd.o $(OBJ)/u-sbrn.o $(OBJ)/u-sswm.o $(OBJ)/u-scrn.o \
        $(OBJ)/u-smvb.o
	$(LD) $(LFLAGS) $(OBJ)/mutgera.o $(OBJ)/glob.o $(OBJ)/opmuta.o \
	$(OBJ)/fase1.o $(OBJ)/fase2.o $(OBJ)/loadfunc.o $(OBJ)/tmpcon.o  \
	$(OBJ)/reqcons.o $(OBJ)/reparg.o $(OBJ)/argincr.o $(OBJ)/argneg.o \
	$(OBJ)/delarg.o $(OBJ)/delfunc.o $(OBJ)/swalike.o $(OBJ)/swdif.o \
	$(OBJ)/varset.o $(OBJ)/varrepp.o $(OBJ)/varrepc.o $(OBJ)/varrepg.o \
	$(OBJ)/varrepl.o $(OBJ)/varrepe.o $(OBJ)/varrepr.o $(OBJ)/varincr.o \
	$(OBJ)/varneg.o $(OBJ)/delret.o $(OBJ)/repret.o $(OBJ)/covnode.o \
	$(OBJ)/u-cccr.o $(OBJ)/u-ocng.o $(OBJ)/u-ccsr.o $(OBJ)/u-crcr.o \
	$(OBJ)/u-obom.o $(OBJ)/u-olbng.o $(OBJ)/u-oido.o $(OBJ)/u-oipm.o \
        $(OBJ)/u-ocor.o $(OBJ)/u-strp.o $(OBJ)/u-sbrc.o $(OBJ)/u-varr.o \
        $(OBJ)/u-vsrr.o $(OBJ)/u-vprr.o $(OBJ)/u-vscr.o $(OBJ)/u-vtrr.o \
        $(OBJ)/u-sglr.o $(OBJ)/u-vdtr.o $(OBJ)/u-smtc.o $(OBJ)/u-stri.o \
        $(OBJ)/u-sdwd.o $(OBJ)/u-sbrn.o $(OBJ)/u-sswm.o $(OBJ)/u-scrn.o \
        $(OBJ)/u-smvb.o -o $(OBJ)/libopmuta.o


$(OBJ)/mutgera.o: $(OPMUTA)/mutgera.c $(INCLUDEFILE)
	 $(CC) $(CFLAGS) $(OPMUTA)/mutgera.c -o $(OBJ)/mutgera.o
	
$(OBJ)/opmuta.o: $(OPMUTA)/opmuta.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/opmuta.c -o $(OBJ)/opmuta.o

$(OBJ)/glob.o: $(OPMUTA)/glob.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/glob.c -o $(OBJ)/glob.o


$(OBJ)/fase1.o: $(OPMUTA)/fase1.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/fase1.c -o $(OBJ)/fase1.o 

$(OBJ)/fase2.o: $(OPMUTA)/fase2.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/fase2.c -o $(OBJ)/fase2.o

$(OBJ)/loadfunc.o: $(OPMUTA)/loadfunc.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/loadfunc.c -o $(OBJ)/loadfunc.o 
	
$(OBJ)/tmpcon.o: $(OPMUTA)/tmpcon.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/tmpcon.c -o $(OBJ)/tmpcon.o

$(OBJ)/reqcons.o: $(OPMUTA)/reqcons.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/reqcons.c -o $(OBJ)/reqcons.o

$(OBJ)/varset.o: $(OPMUTA)/varset.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/varset.c -o $(OBJ)/varset.o

$(OBJ)/reparg.o: $(OPMUTA)/reparg.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/reparg.c -o $(OBJ)/reparg.o

$(OBJ)/argincr.o: $(OPMUTA)/argincr.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/argincr.c -o $(OBJ)/argincr.o

$(OBJ)/delarg.o: $(OPMUTA)/delarg.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/delarg.c -o $(OBJ)/delarg.o

$(OBJ)/delfunc.o: $(OPMUTA)/delfunc.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/delfunc.c -o $(OBJ)/delfunc.o

$(OBJ)/swalike.o: $(OPMUTA)/swalike.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/swalike.c -o $(OBJ)/swalike.o

$(OBJ)/swdif.o: $(OPMUTA)/swdif.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/swdif.c -o $(OBJ)/swdif.o

$(OBJ)/varrepp.o: $(OPMUTA)/varrepp.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/varrepp.c -o $(OBJ)/varrepp.o

$(OBJ)/varrepc.o: $(OPMUTA)/varrepc.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/varrepc.c -o $(OBJ)/varrepc.o

$(OBJ)/varrepg.o: $(OPMUTA)/varrepg.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/varrepg.c -o $(OBJ)/varrepg.o

$(OBJ)/varrepl.o: $(OPMUTA)/varrepl.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/varrepl.c -o $(OBJ)/varrepl.o

$(OBJ)/varrepe.o: $(OPMUTA)/varrepe.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/varrepe.c -o $(OBJ)/varrepe.o

$(OBJ)/varrepr.o: $(OPMUTA)/varrepr.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/varrepr.c -o $(OBJ)/varrepr.o

$(OBJ)/varincr.o: $(OPMUTA)/varincr.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/varincr.c -o $(OBJ)/varincr.o

$(OBJ)/varneg.o: $(OPMUTA)/varneg.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/varneg.c -o $(OBJ)/varneg.o

$(OBJ)/delret.o: $(OPMUTA)/delret.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/delret.c -o $(OBJ)/delret.o

$(OBJ)/repret.o: $(OPMUTA)/repret.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/repret.c -o $(OBJ)/repret.o

$(OBJ)/covnode.o: $(OPMUTA)/covnode.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/covnode.c -o $(OBJ)/covnode.o


$(OBJ)/argneg.o: $(OPMUTA)/argneg.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/argneg.c -o $(OBJ)/argneg.o

$(OBJ)/u-cccr.o: $(OPMUTA)/u-cccr.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-cccr.c -o $(OBJ)/u-cccr.o

$(OBJ)/u-ocng.o: $(OPMUTA)/u-ocng.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-ocng.c -o $(OBJ)/u-ocng.o

$(OBJ)/u-ccsr.o: $(OPMUTA)/u-ccsr.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-ccsr.c -o $(OBJ)/u-ccsr.o

$(OBJ)/u-crcr.o: $(OPMUTA)/u-crcr.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-crcr.c -o $(OBJ)/u-crcr.o

$(OBJ)/u-obom.o: $(OPMUTA)/u-obom.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-obom.c -o $(OBJ)/u-obom.o

$(OBJ)/u-olbng.o: $(OPMUTA)/u-olbng.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-olbng.c -o $(OBJ)/u-olbng.o

$(OBJ)/u-oido.o: $(OPMUTA)/u-oido.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-oido.c -o $(OBJ)/u-oido.o

$(OBJ)/u-oipm.o: $(OPMUTA)/u-oipm.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-oipm.c -o $(OBJ)/u-oipm.o

$(OBJ)/u-ocor.o: $(OPMUTA)/u-ocor.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-ocor.c -o $(OBJ)/u-ocor.o

$(OBJ)/u-strp.o: $(OPMUTA)/u-strp.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-strp.c -o $(OBJ)/u-strp.o

$(OBJ)/u-sbrc.o: $(OPMUTA)/u-sbrc.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-sbrc.c -o $(OBJ)/u-sbrc.o

$(OBJ)/u-varr.o: $(OPMUTA)/u-varr.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-varr.c -o $(OBJ)/u-varr.o

$(OBJ)/u-vsrr.o: $(OPMUTA)/u-vsrr.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-vsrr.c -o $(OBJ)/u-vsrr.o

$(OBJ)/u-vprr.o: $(OPMUTA)/u-vprr.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-vprr.c -o $(OBJ)/u-vprr.o

$(OBJ)/u-vscr.o: $(OPMUTA)/u-vscr.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-vscr.c -o $(OBJ)/u-vscr.o

$(OBJ)/u-vtrr.o: $(OPMUTA)/u-vtrr.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-vtrr.c -o $(OBJ)/u-vtrr.o

$(OBJ)/u-sdwd.o: $(OPMUTA)/u-sdwd.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-sdwd.c -o $(OBJ)/u-sdwd.o

$(OBJ)/u-vdtr.o: $(OPMUTA)/u-vdtr.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-vdtr.c -o $(OBJ)/u-vdtr.o

$(OBJ)/u-smtc.o: $(OPMUTA)/u-smtc.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-smtc.c -o $(OBJ)/u-smtc.o

$(OBJ)/u-stri.o: $(OPMUTA)/u-stri.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-stri.c -o $(OBJ)/u-stri.o

$(OBJ)/u-sglr.o: $(OPMUTA)/u-sglr.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-sglr.c -o $(OBJ)/u-sglr.o

$(OBJ)/u-sbrn.o: $(OPMUTA)/u-sbrn.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-sbrn.c -o $(OBJ)/u-sbrn.o

$(OBJ)/u-scrn.o: $(OPMUTA)/u-scrn.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-scrn.c -o $(OBJ)/u-scrn.o

$(OBJ)/u-sswm.o: $(OPMUTA)/u-sswm.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-sswm.c -o $(OBJ)/u-sswm.o

$(OBJ)/u-smvb.o: $(OPMUTA)/u-smvb.c $(INCLUDEFILE)
	$(CC) $(CFLAGS) $(OPMUTA)/u-smvb.c -o $(OBJ)/u-smvb.o

util: muta-gen test-new tcase-add muta-view list-good
	# Building util


muta-gen: $(UTIL)/muta-gen.c $(INCLUDEFILE) $(OBJ)/libmuta.o lib\
	  $(OBJ)/libpteste.o 
	  $(CC) $(CLFLAGS) $(UTIL)/muta-gen.c \
                $(OBJ)/libmuta.o $(OBJ)/libgerais.o\
	        $(OBJ)/libpteste.o -o $(BIN)/muta-gen


test-new: $(UTIL)/test-cre.c  $(INCILUDEFILE) $(OBJ)/libmuta.o lib\
	  $(OBJ)/libpteste.o 
	  $(CC) $(CLFLAGS) $(UTIL)/test-cre.c \
                $(OBJ)/libmuta.o $(OBJ)/libgerais.o\
	  	$(OBJ)/libpteste.o -o $(BIN)/test-new
   

tcase-add: $(UTIL)/tcase-ad.c  $(INCLUDEFILE) $(OBJ)/libmuta.o lib\
	  $(OBJ)/libpteste.o 
	  $(CC) $(CLFLAGS) $(UTIL)/tcase-ad.c \
		$(OBJ)/libmuta.o $(OBJ)/libgerais.o\
	  	$(OBJ)/libpteste.o  -o $(BIN)/tcase-add

muta-view: $(UTIL)/muta-viw.c $(INCLUDEFILE)  $(OBJ)/libmuta.o \
	  $(OBJ)/libpteste.o lib 
	  $(CC) $(CLFLAGS) $(UTIL)/muta-viw.c \
	  	$(OBJ)/libmuta.o $(OBJ)/libgerais.o \
	  	$(OBJ)/libpteste.o  -l$(CURSES) -ltermcap \
		-o $(BIN)/muta-view 

list-good: $(UTIL)/list-good.c $(INCLUDEFILE)  $(OBJ)/libmuta.o \
		$(OBJ)/libpteste.o $(OBJ)/libtcase.o lib
		$(CC) $(CLFLAGS) $(UTIL)/list-good.c \
		$(OBJ)/libmuta.o $(OBJ)/libgerais.o $(OBJ)/libtcase.o \
		$(OBJ)/libpteste.o  -o $(BIN)/list-good
  

misc: headtail extimout splitarg recinput
	# building auxiliar programs

splitarg: $(MISC)/splitarg.c $(INCLUDEFILE) $(OBJ)/libmuta.o
	$(CC) $(CLFLAGS) $(MISC)/splitarg.c -o $(BIN)/splitarg

headtail: $(MISC)/headtail.c $(INCLUDEFILE) $(OBJ)/libmuta.o
	$(CC) $(CLFLAGS) $(MISC)/headtail.c $(OBJ)/libgerais.o \
	-o $(BIN)/headtail

recinput: $(MISC)/recinput.c $(INCLUDEFILE) $(OBJ)/libmuta.o
	$(CC) $(CLFLAGS) $(MISC)/recinput.c $(OBJ)/libgerais.o \
	-o $(BIN)/recinput


extimout: $(MISC)/extimout.c  
	$(CC) $(CLFLAGS) $(MISC)/extimout.c  -o $(BIN)/extimout

btree: $(OBJ)/btree.o $(OBJ)/btlib.o $(OBJ)/bglob.o
	$(LD) $(LFLAGS) $(OBJ)/btree.o $(OBJ)/btlib.o  \
	$(OBJ)/bglob.o -o $(OBJ)/libbtree.o

$(OBJ)/btree.o: $(BTREE)/btree.c $(BTREE)/bglob.c $(BTREE)/btree.h
	$(CC) $(CFLAGS) $(BTREE)/btree.c -o $(OBJ)/btree.o

$(OBJ)/btlib.o: $(BTREE)/btlib.c $(BTREE)/btree.h
	$(CC) $(CFLAGS) $(BTREE)/btlib.c -o $(OBJ)/btlib.o

$(OBJ)/bglob.o: $(BTREE)/bglob.c 
	$(CC) $(CFLAGS) $(BTREE)/bglob.c -o $(OBJ)/bglob.o


