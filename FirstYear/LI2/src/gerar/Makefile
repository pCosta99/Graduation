CFLAGS=-std=c11 -Wall -Wextra -pedantic -g
FICHEIROS=cgi.h gerar.c estado.c estado.h main.c file.c file.h ler_estado.c ler_estado.h check.c check.h ajuda.c ajuda.h list.c list.h resolveEstado.c resolveEstado.h Makefile
EXECUTAVEL=gerar
OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))

install: $(EXECUTAVEL) 
	sudo cp $(EXECUTAVEL) /usr/lib/cgi-bin

$(EXECUTAVEL): $(OBJECTS)
	gcc $(CFLAGS) -o $(EXECUTAVEL) $(OBJECTS)

GandaGalo.zip: $(FICHEIROS)
	zip -9 GandaGalo.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen

clean:
	rm -rf *.o $(EXECUTAVEL) Doxyfile latex html relatorio.log relatorio.aux relatorio.pdf missfont.log