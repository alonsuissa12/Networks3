# Makefile for TCP project

all: Receiver Sender
clean:
	rm -f *.exe *.o Receiver Sender

.PHONEY: all clean

Receiver.o:Receiver.c Receiver.h
	gcc -c Receiver.c -o Receiver.o
Sender.o: Sender.c Sender.h
	gcc -c Sender.c -o Sender.o
Sender: Sender.o
	gcc Sender.o -o Sender
Receiver: Receiver.o
	gcc Receiver.o -o Receiver
