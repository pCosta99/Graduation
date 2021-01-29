CFLAGS=-std=c11 -Wall -Wextra -pedantic -g
FICHEIROS=cgi.h estado.c estado.h main.c file.c file.h ler_estado.c ler_estado.h check.c check.h ajuda.c ajuda.h list.c list.h resolveEstado.c resolveEstado.h Makefile
EXECUTAVEL=GandaGalo
OBJECTS:=$(patsubst %.c,%.o,$(wildcard *.c))

install: $(EXECUTAVEL) imagens ficheiros users css gera
	sudo cp $(EXECUTAVEL) /usr/lib/cgi-bin
	touch install

$(EXECUTAVEL): $(OBJECTS)
	gcc $(CFLAGS) -o $(EXECUTAVEL) $(OBJECTS)

gera:
	cd gerar && make

css:
	sudo mkdir -p /var/www/html/css
	sudo cp CSS/*.css /var/www/html/css

imagens:
	sudo mkdir -p /var/www/html/images
	sudo cp Imagens/*.png /var/www/html/images

ficheiros:
	sudo mkdir -p /var/www/html/ficheiros
	sudo cp Estados/*.e /var/www/html/ficheiros  
	sudo chmod -R a+rwx /var/www/html/*

users: 
	sudo mkdir -p /var/www/html/users
	sudo touch /var/www/html/user.e
	sudo chmod -R a+rwx /var/www/html/*

GandaGalo.zip: $(FICHEIROS)
	zip -9 GandaGalo.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen

clean:
	rm -rf *.o $(EXECUTAVEL) Doxyfile latex html install