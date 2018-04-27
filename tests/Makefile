CC  = gcc
CXX = g++

INCLUDES = -I/usr/local/Cellar/boost/1.67.0_1/include/

CFLAGS   = -g -O2 $(INCLUDES)
CXXFLAGS = -g -O2 $(INCLUDES) -std=c++17 -fconcepts

executables = tests cite_graph cite_graph_onlyeq max_flow_test columbia_top_sort
objects = tests.o cite_graph.o cite_graph_onlyeq.o max_flow_test.o columbia_top_sort.o

.PHONY: default
default: $(executables)

$(executables):

$(objects):

.PHONY: clean
clean:
	rm -f *~ a.out core $(objects) $(executables)

.PHONY: all
all: clean default
