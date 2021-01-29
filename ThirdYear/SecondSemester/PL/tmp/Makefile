gedcom : y.tab.o lex.yy.o
	gcc -o gedcom y.tab.o lex.yy.o -ll

y.tab.o : y.tab.c 
	gcc -c y.tab.c 

lex.yy.o : lex.yy.c 
	gcc -c lex.yy.c 

y.tab.c y.tab.h : gedcom.y 
	yacc -d gedcom.y 

lex.yy.c : gedcom.l y.tab.h 
	flex gedcom.l

clean : 
	rm -f lex.yy.c y.tab.c y.tab.o lex.yy.o y.tab.h gedcom
