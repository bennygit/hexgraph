CC = g++
CPPFLAGS = -std=c++0x -Wall `root-config --cflags`
LDFLAGS = `root-config --libs`
EXE1 = hexgraph
INCLUDES = ./tclap

all: $(EXE1).cc $(INCLUDES)
	$(CC) $(CPPFLAGS) -I $(INCLUDES) $(EXE1).cc -o $(EXE1) $(LDFLAGS)


.PHONY: clean
clean:
	rm -f $(EXE1)
