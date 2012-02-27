ROOTDIR=../libhw
THREADDIR=../gthread
MEMDIR=../gmem

LIBNAME=hw
LIBFILE=$(ROOTDIR)/lib/lib$(LIBNAME).a $(THREADDIR)/gthread.a $(MEMDIR)/gmem.a

CC = gcc
AR = ar
RANLIB = ranlib
CFLAGS = -Wall -m32 -g -m32
LIBDIR  = $(ROOTDIR)/lib
INCDIR  = -I$(ROOTDIR)/include -I$(THREADDIR) -I$(MEMDIR)
LIBS    = -L$(LIBDIR) -l$(LIBNAME) -L$(THREADDIR) -L$(MEMDIR)

###------------------------------
### Main targets 
###------------------------------------------------------------
BINARIES=spawner

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

spawner: spawner.o moduleTraduction.o moduleActionneur.o $(LIBFILE)


###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean
clean:
	$(RM) *.o $(BINARIES) Makefile.depends

