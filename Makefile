CC  = /usr/local/Cellar/gcc/7.3.0_1/bin/g++-7
CXX = /usr/local/Cellar/gcc/7.3.0_1/bin/g++-7

INCLUDES = -I/usr/local/Cellar/boost/1.67.0_1/include/

CFLAGS   = -g -O2 -Wall $(INCLUDES)
CXXFLAGS = -g -O2 -Wall $(INCLUDES) -std=c++17 -fconcepts

executables = tests cite_graph
objects = tests.o cite_graph.o

.PHONY: default
default: $(executables)

$(executables):

$(objects):

.PHONY: clean
clean:
	rm -f *~ a.out core $(objects) $(executables)

.PHONY: all
all: clean default
