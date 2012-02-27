

CC = gcc
AR = ar
RANLIB = ranlib
CFLAGS = -Wall -m32 -g -m32

###------------------------------
### Main targets 
###------------------------------------------------------------
BINARIES=moduleTraduction.o

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

moduleTraduction.o: Listener.o Message.o 


###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean
clean:
	$(RM) *.o $(BINARIES) Makefile.depends

