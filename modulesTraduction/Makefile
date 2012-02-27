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

all: 
	cd EnOceanModuleTraduction/EnOceanModuleTraduction/; make
	cd EnOceanModuleActuator/EnOceanModuleActuator/; make
	make $(BINARIES)
	

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

spawner: spawner.o EnOceanModuleTraduction/EnOceanModuleTraduction/moduleTraduction.o EnOceanModuleActuator/EnOceanModuleActuator/moduleActionneur.o EnOceanModuleActuator/EnOceanModuleActuator/Listener.o EnOceanModuleActuator/EnOceanModuleActuator/Sender.o EnOceanModuleTraduction/EnOceanModuleTraduction/Message.o EnOceanModuleTraduction/EnOceanModuleTraduction/Listener.o $(LIBFILE)


###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean
clean:
	$(RM) *.o $(BINARIES) Makefile.depends

