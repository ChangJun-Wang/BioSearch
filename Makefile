CC=g++
LDFLAGS=-std=c++11 -O3 -lm
SOURCES=src/searcher.cpp src/main.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bio
INCLUDES=src/edge.h src/node.h src/searcher.h

all: $(SOURCES) ./$(EXECUTABLE)

./$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o:  %.c  ${INCLUDES}
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o ./$(EXECUTABLE)
