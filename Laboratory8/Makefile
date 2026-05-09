CCOMP = gcc
LOADER = gcc

#Optimization flags!

OPT = -O0

INC = -I./

#libraries
LIB = -lm -lrt

# zaleznosci i komendy
my_program: my_program.o mat_vec.o tmeas.o
	$(LOADER) $(OPT) my_program.o mat_vec.o tmeas.o -o my_program $(LIB)

my_program.o: my_program.c
	$(CCOMP) -c $(OPT) my_program.c $(INC)

mat_vec.o: mat_vec.c
	$(CCOMP) -c $(OPT) mat_vec.c

tmeas.o: tmeas.c
	$(CCOMP) -c $(OPT) tmeas.c $(INC)

clean:
	rm -f *.o
	rm -f my_program
