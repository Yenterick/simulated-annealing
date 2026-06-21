# Default target: Compiles and runs the program
all:
	gcc ./src/annealing/annealing.c ./src/main.c -o annealing
	./annealing

# Removes generated binary
clean:
	rm -f ./annealing