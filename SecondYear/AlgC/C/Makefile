CC = gcc
CFLAGS = -g -Wall -pedantic -lm
PROGRAMS = dataStructures grafos npComplete
DEPS = avlTree.h minHeaps.h hashProbeCounting.h gui.h grafos.h findAndUnion.h fringeOrd.h fringeVetor.h fringeMin.h
OBJ = main.o avlTree.o minHeaps.o hashProbeCounting.o gui.o
OBJG = grafos.o findAndUnion.o fringeOrd.o fringeVetor.o fringeMin.o 
OBJN = npComplete.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

all: dataStructures grafos npComplete

dataStructures: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

grafos: $(OBJG)
	$(CC) -o $@ $^ $(CFLAGS) 

npComplete: $(OBJN)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm *.o $(PROGRAMS)
