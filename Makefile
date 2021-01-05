all: server.o client.o
	gcc -o server server.o
	gcc -o client client.o
	
console.o: server.c
	gcc -c server.c
	
processor.o: client.c
	gcc -c client.c
	
clean:
	rm *.o
