CFLAGS= -c -Wall -Wextra -O2
CC=gcc
OBJECTS=hospital.o parse.o structure.o

hospital: $(OBJECTS)
	$(CC) -o hospital $(OBJECTS)
	
debug: CFLAGS += -g	
debug: $(OBJECTS)
	$(CC) -g -o hospital.dbg $(OBJECTS)
	
structure.o: structure.c structure.h
	$(CC) $(CFLAGS) $<

parse.o: parse.c parse.h
	$(CC) $(CFLAGS) $<
	
hospital.o: hospital.c
	$(CC) $(CFLAGS) $<	

.PHONY: clean debug

	
clean:
	@echo Usuwamy zbedne pliki
	rm -f *.o
	rm -f hospital
	rm -f hospital.dbg

