$(info $(SHELL))
CC = gcc
sources = $(wildcard *.c)
objects = $(sources:.c=.o)
flags = -g

ifeq ($(OS),Windows_NT)
    exec = pyre.exe
    RM = del /Q
    INSTALL = runas /user:Administrator "setx /M path \"%path%;.\pyre.exe\""
else
    exec = pyre
    RM = rm -f
    INSTALL = sudo cp $(exec) /usr/local/bin/
endif

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

