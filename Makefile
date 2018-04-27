CC  = gcc
CXX = g++

INCLUDES = -I/usr/local/Cellar/boost/1.67.0_1/include/

CFLAGS   = -g -O2 -Wall $(INCLUDES)
CXXFLAGS = -g -O2 -Wall $(INCLUDES) -std=c++17 -fconcepts

executables = tests cite_graph cite_graph_onlyeq cite_graph_unordered max_flow_test columbia_top_sort tester
objects = tests.o cite_graph.o cite_graph_onlyeq.o cite_graph_unordered.o max_flow_test.o columbia_top_sort.o tester.o

.PHONY: default
default: $(executables)

$(executables):

$(objects):

.PHONY: clean
clean:
	rm -f *~ a.out core $(objects) $(executables)

.PHONY: all
all: clean default
