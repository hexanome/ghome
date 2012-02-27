# $Id: Makefile 118 2009-12-02 13:45:09Z simon_duquennoy $
##############################################################################
ROOTDIR=../libhardware
LIBNAME=hardware
LIBFILE=$(ROOTDIR)/lib/lib$(LIBNAME).a

CC	= gcc
CFLAGS	= -Wall -ansi -pedantic -g -m32
LIBDIR  = $(ROOTDIR)/lib
INCDIR  = -I$(ROOTDIR)/include
LIBS    = -L$(LIBDIR) -l$(LIBNAME)

###------------------------------
### Main targets 
###------------------------------------------------------------
BINARIES= mkhd

all: $(BINARIES)

###------------------------------
### Main rules 
###------------------------------------------------------------

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(INCDIR)

%: %.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

Makefile.depends:
	$(CC) -MM *.c $(INCDIR) > Makefile.depends

include Makefile.depends

mkhd: mkhd.o $(LIBFILE)

###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean:
	$(RM) *.o $(BINARIES) Makefile.depends
realclean: clean 
	$(RM) vdiskA.bin vdiskB.bin

