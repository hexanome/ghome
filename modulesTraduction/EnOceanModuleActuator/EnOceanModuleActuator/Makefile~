# $Id: Makefile 95 2009-10-06 12:55:04Z simon_duquennoy $
#------------------------------------------------------------

CC = gcc
AR = ar
RANLIB = ranlib
CFLAGS = -Wall -pedantic -m32 -g -m32

OBJECTS = moduleActionneur.o

HARDWARELIB = moduleActionneur.a

all: $(HARDWARELIB)

###------------------------------
### Bibliotheque
###------------------------------------------------------------
%.o: %.c
	$(CC) $(CFLAGS) -c $<

%: %.o
	$(CC) $(CFLAGS) -o $@ $^

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
