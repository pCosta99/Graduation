IDIR = ../include
LDIR = ../libs
CC = gcc
CFLAGS = -Wall -g -ansi -O2 -I$(IDIR)
CFLAGS_LIBS = -Wall -g
LIBS = `pkg-config --libs --cflags gmodule-2.0` -lm

ODIR = obj
SRC = src_files

_DEPS = pair.h pqueue.h filial.h catclientes.h catprods.h catalog.h config.h faturacao.h parser.h printableResults.h sale.h sgv.h stats.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o pair.o pqueue.o filial.o catclientes.o catprods.o stats.o printableResults.o sale.o sgv.o faturacao.o parser.o config.o catalog.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: mkdirs sgv

sgv: $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

$(ODIR)/%.o: $(SRC)/%.c
	$(CC) -c $(CFLAGS) $(LIBS) -o $@ $<

mkdirs:
	mkdir -p $(ODIR)
	mkdir -p Stats
	mkdir -p Stats/Results

.PHONY: clean

clean:
	rm -rf $(ODIR) sgv vgcore.*
