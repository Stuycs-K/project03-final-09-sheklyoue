compile: client server 

functions.o: functions.c functions.h
	@gcc -c functions.c

client.o: client.c functions.h
	@gcc -c client.c

server.o: server.c functions.h
	@gcc -c server.c

client: client.o functions.o
	@gcc -o client client.o functions.o -Wall -lncurses

server: server.o functions.o
	@gcc -o server server.o functions.o -Wall -lncurses

testing.o: testing.c
	@gcc -c testing.c

ncurse: testing.o
	@gcc -o ncur testing.o functions.o -lncurses

ncur: ncurse

clean:
	rm -f *.o runme client server
	rm -f chat.txt
