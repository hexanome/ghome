# $Id: Makefile 95 2009-10-06 12:55:04Z simon_duquennoy $
#------------------------------------------------------------

CC = gcc
AR = ar
RANLIB = ranlib
CFLAGS = -Wall -pedantic -m32 -g

INSTALLDIR = ../libhw

OBJECTS = gmem.o

HARDWARELIB = gmem.a

all: $(HARDWARELIB)

###------------------------------
### Bibliotheque
###------------------------------------------------------------
$(HARDWARELIB): $(OBJECTS)
	$(AR) cr $(HARDWARELIB) $(OBJECTS)
	$(RANLIB) $(HARDWARELIB)

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

###------------------------------
### Install
###------------------------------------------------------------
.PHONY: install
install: 
	install -m 644 $(HARDWARELIB) 
