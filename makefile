default: run 

functions.o: functions.c
	@gcc -c functions.c

testing_main.o: testing_main.c
	@gcc -c testing_main.c

compile: functions.o testing_main.o
	@gcc -o runme functions.o testing_main.o -Wall

run: compile
	@./runme

clean:
	rm -f *.o runme
