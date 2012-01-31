EnOceanModuleTraduction: Listener.o Message.o main.o
	gcc -o EnOceanModuleTraduction Listener.o Message.o main.o

Listener.o: Listener.c
	gcc -o Listener.o -c Listener.c -W -Wall

Message.o: Message.c
	gcc -o Message.o -c Message.c -W -Wall
	
main.o: main.c Listener.h
	gcc -o main.o -c main.c -W -Wall