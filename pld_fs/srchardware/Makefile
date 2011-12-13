# $Id: Makefile 95 2009-10-06 12:55:04Z simon_duquennoy $
#------------------------------------------------------------

CC = gcc
AR = ar
RANLIB = ranlib
CFLAGS = -Wall -pedantic -m32 -g

INSTALLDIR = ../libhardware

OBJECTS = config_lex.o config_yacc.o hardware.o \
	hw_clock.o hw_disk.o hw_ethernet.o hw_serialline.o hw_tools.o hw_mmu.o hw_mmu_tlb.o
HARDWARELIB = libhardware.a

all: $(HARDWARELIB)

###------------------------------
### Bibliotheque
###------------------------------------------------------------
$(HARDWARELIB): $(OBJECTS)
	$(AR) cr $(HARDWARELIB) $(OBJECTS)
	$(RANLIB) $(HARDWARELIB)

###------------------------------
### Lecture de configuration
###------------------------------------------------------------
config_lex.c: config.l config_yacc.h
	flex  -oconfig_lex.c config.l
config_yacc.h: config_yacc.c
config_yacc.c: config.y
	bison -d -oconfig_yacc.c config.y

###------------------------------
### Inclusions
###------------------------------------------------------------
Makefile.depends:
	$(CC) -MM *.c $(INCDIR) > Makefile.depends

include Makefile.depends

###------------------------------
### Tools
###------------------------------------------------------------
.PHONY: clean realclean 
clean:
	rm -f $(HARDWARELIB)
	rm -f *.o
	rm -f Makefile.depends
realclean : clean
	rm -f $(OBJECTS) $(addsuffix .o,$(BINARIES)) 
	rm -f $(BINARIES) 
	rm -f config_lex.c config_yacc.h config_yacc.c 

###------------------------------
### Install
###------------------------------------------------------------
.PHONY: install
install: $(HARDWARELIB)
	mkdir -p $(INSTALLDIR)/lib $(INSTALLDIR)/include $(INSTALLDIR)/etc
	install -m 644 $(HARDWARELIB) $(INSTALLDIR)/lib 
	install -m 644 hardware.h $(INSTALLDIR)/include
	install -m 644 hardware.ini $(INSTALLDIR)/etc
