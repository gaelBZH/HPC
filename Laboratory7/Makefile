#C compiler
CCOMP = gcc

#C linker
LOADER = gcc

# optymalization options
OPT = -O3


all: libtmeas.a tmeas.o

tmeas.o: tmeas.c tmeas.h
	$(CCOMP) -c $(OPT) tmeas.c

libtmeas.a: tmeas.o
	ar rs libtmeas.a tmeas.o

clean:
	rm -f *.o
