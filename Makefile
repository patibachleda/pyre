$(info $(SHELL))
CC = gcc
exec = pyre.exe
sources = $(wildcard *.c)
objects = $(sources:.c=.o)
flags = -g


$(exec): $(objects)
	$(CC) $(sources) $(flags) -o $(exec)

%.o: %.c include/%.h
	$(CC) -c $(flags) $< -o $@

install:
	make
	runas /user:Administrator "setx /M path \"%path%;.\pyre.exe\""

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o