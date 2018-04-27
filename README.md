# graph_explorer
Use `-std=c++17` flag while compiling since optional datatype is only supported in CPP 17

Use -fconcepts flag while compiling since concepts are in use as well

to compile using gcc:

`g++ -std=c++17 -fconcepts <your_filename>.cpp`

To compile tests using makefile, replace the "INCLUDES = -I/usr/local/Cellar/boost/1.67.0_1/include/" line with whichever directory you have boost in to make sure include finds it there.
