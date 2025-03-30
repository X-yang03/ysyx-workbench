CFLAGS = -Wall -g
INCLUDE_PATH = ./inc/ 
SOURCES = ./src/*.c 

stack: $(SOURCES) 
	cc $(CFLAGS) -I$(INCLUDE_PATH) -o stack $(SOURCES)


all: stack

clean:
	rm -f stack
