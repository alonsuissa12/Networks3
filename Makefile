# Makefile for TCP project

all: Receiver.o Sender.o
clean:
	rm -f *.exe *.o

.PHONEY: all clean

Receiver.o:Receiver.c Receiver.h
	gcc -c Receiver.c -o Receiver.o
Sender.o: Sender.c Sender.h
	gcc -c Sender.c -o Sender.o
Sender.exe: Sender.o
	gcc Sender.o -o Sender.exe
Receiver.exe: Receiver.o
	gcc Receiver.o -o Receiver.exe
runs: Sender.exe
	./Sender.exe
runr: Receiver.exe
	./Receiver.exe
