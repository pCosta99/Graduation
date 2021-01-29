CC = gcc
CFLAGS = -Wall -g

all: files ma sv cv ag

ma: ma.o utilities.o

sv: sv.o utilities.o

concurrentSV: concurrentSV.o utilities.o

cv: cv.o utilities.o

concurrentCV: concurrentCV.o utilities.o

ag: ag.o utilities.o

printer: printer.o utilities.o

ma.o: ma.c

sv.o: sv.c

cv.o: cv.c

ag.o: ag.c

printer.o: printer.c

utilities.o: utilities.c

concurrentSV.o: concurrentSV.c

concurrentCV.o: concurrentCV.c

files:
	mkdir -p Files
	mkdir -p Sales
	mkdir -p SpeedTesting
	touch Files/artigos
	touch Files/strings
	touch Files/stocks
	touch Files/vendas
	touch Files/errorLogging
	chmod 0700 Files/*

clean: 
	rm -f *.o pip ma sv cv ag printer concurrentCV concurrentSV

cleanFiles:
	rm -f /tmp/fifo*
	rm -f /tmp/holder*

delete: clean
	rm -rf Files Sales
	rm -f /tmp/fifo*
	rm -f /tmp/holder*

reboot: delete files ma sv cv ag printer concurrentSV concurrentCV