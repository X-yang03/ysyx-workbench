CFLAGS = -Wall -g
INCLUDE_PATH = ./inc/ 
SOURCES = ./src/*.c 

queue: $(SOURCES) 
	cc $(CFLAGS) -I$(INCLUDE_PATH) -o queue $(SOURCES)


all: queue

clean:
	rm -f queue
