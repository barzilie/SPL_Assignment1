# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

#run
all: bin/main

#depend on main.o and Settlement.o
bin/main: clean bin/main.o bin/Settlement.o bin/Facility.o bin/SelectionPolicy.o bin/Plan.o bin/Simulation.o
	@echo 'invoking linker'
	g++ -o bin/main bin/main.o bin/Settlement.o bin/Facility.o bin/SelectionPolicy.o bin/Plan.o bin/Simulation.o
	@echo 'build completed'

#depend on source
bin/main.o:
	
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

bin/Settlement.o:
	
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Settlement.o src/Settlement.cpp

bin/Facility.o:
	
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Facility.o src/Facility.cpp

bin/SelectionPolicy.o:
	
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp

bin/Plan.o:
	
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Plan.o src/Plan.cpp

bin/Simulation.o:
	
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp

#clean
clean: 

	rm -f bin/*
