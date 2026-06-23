<p align="center">
  <img src="./docs/banner.png" alt="Simulated Annealing">
</p>

<p align="center">
    A C implementation of the Simulated Annealing metaheuristic algorithm designed for academic research, optimization problems, and experimentation with stochastic search methods.
</p>

<p align="center">
    <a href="#"><img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white" alt="Language"></a>
    <a href="#"><img src="https://img.shields.io/badge/Algorithm-Simulated%20Annealing-orange?style=for-the-badge" alt="Algorithm"></a>
    <a href="#"><img src="https://img.shields.io/badge/Field-Optimization-blue?style=for-the-badge" alt="Field"></a>
    <a href="#"><img src="https://img.shields.io/badge/Research-Academic-purple?style=for-the-badge" alt="Research"></a>
    <a href="#"><img src="https://img.shields.io/badge/Paradigm-Metaheuristic-red?style=for-the-badge" alt="Paradigm"></a>
    <a href="#"><img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge" alt="License"></a>
</p>

---

## Prerequisites

You need a C compiler and Make installed.

#### Linux (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install build-essential
```

#### Arch Linux / Manjaro

```bash
sudo pacman -S base-devel
```

#### macOS

```bash
xcode-select --install
```

#### Windows

Install either:

- MSYS2 (includes gcc and make)
- WSL (Ubuntu + build-essential)

```bash
gcc --version
make --version
```

---

## How to run

#### 1. Compile the Project

In order to run the algorithm you must compile the C files first:
```bash
make
```
or:
```bash
make all
```
It will leave a binary file, in case you want to delete it, you can do:
```bash
make clean
```

#### 2. Prepare your Files

Make sure your files have the next format:
```txt
graph_size
id x y
id x y
id x y
...
```
If you don't have any file prepared you can use the example in `./examples/input.txt`.

#### 3. Run the Project

You can run the command by using:
```bash
./annealing input_path output_path
```
Or by using:
```bash
./annealing input_path
```
and it will create the output file in the current directory.

---

## About the Project

### About Simulated Annealing

Simulated Annealing (SA) is a randomized optimization algorithm whose key feature is the ability to escape local optima by occasionally accepting worse solutions based on a probabilistic rule.
Its name comes from the physical process of annealing in metallurgy, where a material is heated and then slowly cooled to reduce defects and reach a more stable energy state.
In the same way, SA starts with a high “temperature” that allows exploration of the solution space and gradually cools down, reducing randomness.

### Algorithm Analysis

```
PROCEDURE Simulated-Annealing(graph, temperature, min_temperature, cooling_rate, max_time, max_on_same)

Inputs: 
• graph: A graph that contains an array of nodes an its size.
• temperature: The initial temperature of the algorithm.
• min_temperature: The lowest the temperature can go.
• cooling_rate: How fast the algorithm will cool down.
• max_time: The max running in seconds allowed.
• max_on_same: The max the algorithm can stay on the same solution.

Output: A solution that contains an array of nodes sorted in the route order and the cost of the route.

1. Select and calculate the cost of the initial solution generated randomly using Fisher-Yates.
2. Define and declare the best solution, putting the current solution on it.
3. Start the clock and the counter of the times staying on the same solution.
4. While the 
    temperature is GREATER THAN min_temperature AND
    elapsed time is LESS THAN max_time AND
    counter of same solution is LESS THAN max_on_same:
        A. Generates and calculates the cost of a new neighbor randomly rearranging two nodes.
        B. If the cost of the neighbor is LESS THAN the current cost, replace current with the neighbor and reset the same solution counter, if not:
            B.1. If not, calculate the probability of keeping the answer using Metropolitan acceptance probability.
            B.2. If the probability is correct replace current with the neighbor an reset the same solution counter, if not, add one to the same solution counter.
        C. If the current cost is LESS THAN the best cost, replace best with the current solution.
        D. Lower the temperature using the cooling_rate.
        E. Return the value of the best solution as the output.
```

### Complexity Analysis

Let n be the number of nodes in the graph.

- **Cost evaluation**: O(n)
- **Neighbor generation**: O(n)
- **One iteration of SA**: O(n)
- **Total complexity**: O(k · n), where k is number of iterations until stopping condition.

---

### License
This project is licensed under the [MIT License](./LICENSE).

### Authors
- [Sedkeee](https://github.com/Sedkeee)
- [Yenterick](https://github.com/Yenterick)

### References
- Kernighan, B. W., & Ritchie, D. M. (1988). *The C Programming Language*. Prentice Hall.
- James Madison University. (s.f.). *C documentation examples.*. Documentation examples for C programs
- Glover, F. W., & Kochenberger, G. A. (2003). *Handbook of Metaheuristics*. Springer.
- Cormen, T. H. (2013). *Algorithms Unlocked*. MIT Press.
- Proxihox, adamant-pwn & whyvineet. (2026, May 24). *Simulated annealing. In Algorithms for Competitive Programming*. https://cp-algorithms.com/num_methods/simulated_annealing.html
- Adhikari, B. (2017, February 2017). *The simulated annealing algorithm explained with an analogy to a toy* [Video]. YouTube. https://youtu.be/eBmU1ONJ-os?si=ZyIu43WscIgbVJRR


