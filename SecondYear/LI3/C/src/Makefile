IDIR = ../include
LDIR = ../libs
CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0` -Wall -g -ansi -O2 -I$(IDIR)
CFLAGS_NO_ANSI = `pkg-config --cflags glib-2.0` -Wall -g -O2 -I$(IDIR)
CFLAGS_LIBS = -Wall -g
LIBS = `pkg-config --libs glib-2.0` -lm

ODIR = obj
SRC = src_files

_DEPS = navegador.h controller.h views.h pair.h pqueue.h filial.h catclientes.h catprods.h catalog.h config.h faturacao.h parser.h sale.h interface.h stats.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o navegador.o controller.o views.o pair.o pqueue.o filial.o catclientes.o catprods.o stats.o sale.o interface.o faturacao.o parser.o config.o catalog.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: mkdirs sgv

sgv: $(OBJ)
	$(CC) $(CFLAGS_NO_ANSI) $(LIBS) -o $@ $^

mkdirs:
	mkdir -p $(ODIR)

$(ODIR)/main.o: $(SRC)/main.c $(IDIR)/interface.h $(IDIR)/controller.h $(IDIR)/config.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/navegador.o: $(SRC)/navegador.c $(IDIR)/navegador.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/controller.o: $(SRC)/controller.c $(IDIR)/controller.h $(IDIR)/pair.h $(IDIR)/pqueue.h $(IDIR)/navegador.h $(IDIR)/interface.h $(IDIR)/parser.h $(IDIR)/views.h $(IDIR)/config.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/views.o: $(SRC)/views.c $(IDIR)/views.h $(IDIR)/navegador.h $(IDIR)/pqueue.h $(IDIR)/pair.h $(IDIR)/config.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/pair.o: $(SRC)/pair.c $(IDIR)/pair.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/pqueue.o: $(SRC)/pqueue.c $(IDIR)/pqueue.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/filial.o: $(SRC)/filial.c $(IDIR)/filial.h $(IDIR)/pair.h $(IDIR)/sale.h $(IDIR)/config.h $(IDIR)/catalog.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/faturacao.o: $(SRC)/faturacao.c $(IDIR)/faturacao.h $(IDIR)/catalog.h $(IDIR)/sale.h $(IDIR)/pair.h $(IDIR)/config.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/catclientes.o: $(SRC)/catclientes.c $(IDIR)/catclientes.h $(IDIR)/catalog.h $(IDIR)/config.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/catprods.o: $(SRC)/catprods.c $(IDIR)/catprods.h $(IDIR)/catalog.h $(IDIR)/config.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/stats.o: $(SRC)/stats.c $(IDIR)/stats.h $(IDIR)/config.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/sale.o: $(SRC)/sale.c $(IDIR)/sale.h $(IDIR)/config.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/interface.o: $(SRC)/interface.c $(IDIR)/interface.h $(IDIR)/parser.h $(IDIR)/config.h $(IDIR)/interface.h $(IDIR)/pqueue.h $(IDIR)/pair.h $(IDIR)/stats.h $(IDIR)/faturacao.h $(IDIR)/filial.h $(IDIR)/catprods.h $(IDIR)/catclientes.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/parser.o: $(SRC)/parser.c $(IDIR)/parser.h $(IDIR)/sale.h $(IDIR)/config.h $(IDIR)/interface.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/config.o: $(SRC)/config.c $(IDIR)/config.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(ODIR)/catalog.o: $(SRC)/catalog.c $(IDIR)/catalog.h
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -rf $(ODIR) sgv vgcore.* ../docs/*.log ../docs/*.aux ../docs/*.toc
