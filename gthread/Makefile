ROOTDIR=../libhw
MEMDIR=../gmem

LIBNAME=hw
LIBFILE=$(ROOTDIR)/lib/lib$(LIBNAME).a $(MEMDIR)/gmem.a

CC = gcc
AR = ar
RANLIB = ranlib
CFLAGS = -Wall -pedantic -m32 -g -m32
LIBDIR  = $(ROOTDIR)/lib $(MEMDIR)
INCDIR  = -I$(ROOTDIR)/include -I$(MEMDIR)
LIBS    = -L$(LIBDIR) -l$(LIBNAME)

OBJECTS = gthread.o

HARDWARELIB = gthread.a

all: $(HARDWARELIB)

###------------------------------
### Bibliotheque
###------------------------------------------------------------
%.o: %.c
	$(CC) $(CFLAGS) -c $< $(INCDIR)

%: %.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

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
