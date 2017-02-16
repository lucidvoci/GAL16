CPP=g++
CPPFLAGS=-std=c++11 -fopenmp
LDFLAGS=

main: main.o structures.o generate.o
	$(CPP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

generate.o: generate.cpp generate.hpp
	$(CPP) -c $(CPPFLAGS) $< -o $@

structures.o: structures.cpp structures.hpp
	$(CPP) -c $(CPPFLAGS) $< -o $@

main.o: main.cpp main.hpp
	$(CPP) -c $(CPPFLAGS) $< -o $@
