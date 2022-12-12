# Makefile for TCP project

all: Receiver.exe Sender.exe
clean:
	rm -f *.exe *.o

.PHONEY: all clean runr runs

Receiver.o:Receiver.c Receiver.h
	gcc Receiver.c -o Receiver.o
Sender.o: Sender.c Sender.h
	gcc  Sender.c -o Sender.o
runs: Sender.o
	./Sender.o
runr: Receiver.o
	./Receiver.o
