# Default target: Compiles the program
all:
	gcc ./src/core/annealing.c -fPIC -shared -o ./src/core/target/annealing.so

# Removes generated binary
clean:
	rm -f ./src/core/target/annealing.so