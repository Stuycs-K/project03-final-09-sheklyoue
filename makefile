default: run 

functions.o: functions.c functions.h
	@gcc -c functions.c

testing_main.o: testing_main.c functions.h
	@gcc -c testing_main.c

client.o: client.c functions.h
	@gcc -c client.c

server.o: server.c functions.h
	@gcc -c server.c

client: client.o functions.o
	@gcc -o client client.o functions.o

server: server.o functions.o
	@gcc -o server server.o functions.o

compile: functions.o testing_main.o
	@gcc -o runme functions.o testing_main.o -Wall

run: compile
	@./runme

compile_server: client server

clean:
	rm -f *.o runme client server
	rm -f chat.txt
