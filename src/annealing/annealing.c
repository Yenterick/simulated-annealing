#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

/* custom imports */
#include "annealing.h"

double calculate_distance(Node a, Node b);
double calculate_route_cost(Graph graph, Solution* solution);
Solution generate_neighbor(Graph graph, Solution current);
Solution generate_initial_solution(Graph graph);
Solution copy_solution(Graph graph, Solution* source);
void free_solution(Solution* solution);

Node create_node(char* id, double x, double y);
Graph create_graph(int size);
void add_node(Graph* graph, Node node, int position);
void free_graph(Graph* graph);

/*
### Simulated Annealing

This function implements the Simulated Annealing algortihm.
This algorithm allows hill-cimbing moves to let the program
escape the local optima, in hopes of finding the global optimum.

Applied in the TSP. Seeking to be applied in the routing of 
refrigerated vehicles.

#### Input:
- Graph graph: The graph we're looking to solve.
- double temperature: The initial temperature of the algorithm to control the quantity of hill climbing moves.
- double min_temperature: The minimum temperature, if the temperature reaches it, the algorithm will end.
- double cooling_rate: How fast the algorithm will cool down.
- double max_time: How much time the algorithm can run in seconds, will end if it reaches it.
- int max_on_same: How many times the algorithm will stay on the same solution till it ends.

#### Output:
- The best solution found inside the graph.

### Example:
```c
simulated_annealing(graph, 1000.0, 0.01, 0.995, 120, 100);
```
*/
Solution simulated_annealing(
    Graph graph, 
    double temperature, 
    double min_temperature, 
    double cooling_rate, 
    double max_time, 
    unsigned int max_on_same) 
{
    Solution current = generate_initial_solution(graph);            /* It generates a first random solution using the graph */       
    current.cost = calculate_route_cost(graph, &current);           /* then, it calculates the total cost of the solution. */

    Solution best = copy_solution(graph, &current);                 /* It remembers the best solution possible yet. */

    clock_t start = clock();                                        /* Starts the clock */
    double time_taken = 0.0;                                        /* and initialize the time taken. */
    
    int i = 0;                                                      /* Initialize the variable that will remember how many */
                                                                    /* times we have stayed on the same solution */

    while (
        temperature > min_temperature &&                            /* It checks that the temperature is still inside the range */ 
        time_taken < max_time &&                                    /* the time hasn't exceeded yet */
        i < max_on_same)                                            /* and it hasn't stayed on the same solution for a lot of iterations. */
    {
        Solution neighbor = generate_neighbor(graph, current);      /* It generates a new neighbor (Similar solution with minimal changes) */
        neighbor.cost = calculate_route_cost(graph, &neighbor);     /* and calculates its route cost. */

        if (neighbor.cost < current.cost) {                         /* If checks if the neighbor is more optimal than the current solution */
            free_solution(&current);                                /* so it free the current solution allocated memory */
            current = copy_solution(graph, &neighbor);              /* deep copy the new solution */
            free_solution(&neighbor);                               /* free the neighbor allocated memory because we already copied it. */
            i = 0;                                                  /* and resets the "same solution" counter. */
        } else {
            double probability =                                    /* If is not more optimal, it calculates the probability of keeping it */
                exp((current.cost - neighbor.cost) / temperature);  /* using the Metropolis acceptance probability and the current temperature. */

            if ((rand() / (double)RAND_MAX) < probability) {        /* If the probability is correct */
                free_solution(&current);                            /* it free the current solution allocated memory */
                current = copy_solution(graph, &neighbor);          /* deep copy the new solution */
                free_solution(&neighbor);                           /* and free the neighbor allocated memory because we already copied it. */
                i = 0;                                              /* and resets the "same solution" counter */
            } else {                                                /* if not */
                free_solution(&neighbor);                           /* it free the neighbor allocated memory because we don't need it anymore */
                i++;                                                /* and adds 1 to the "same solution" counter. */
            }
        }

        if (current.cost < best.cost) {                             /* If the current cost is better than the previous one */
            free_solution(&best);                                   /* it wipes the previous one because we already have a better one */
            best = copy_solution(graph, &current);                  /* and copies the new one. */
        }

        temperature *= cooling_rate;                                /* It cools down using the cooling rate. */

        time_taken = ((double)(clock() - start)) / CLOCKS_PER_SEC;  /* It recalculates the elapsed time. */
    }

    free_solution(&current);                                        /* It free the memory allocated for the current solution. */

    return best;                                                    /* It returns the best solution found */
}

/*
This function calculates the distance between two nodes using Pythagoras Theorem 

#### Input:
- Node a: The origin node.
- Node b: The destiny node.

#### Output:
- The distance between these two nodes.
*/
double calculate_distance(Node a, Node b) {
    double x_component = b.x - a.x;
    double y_component = b.y - a.y;

    return sqrt(pow(x_component, 2) + pow(y_component, 2));
}

/* 
This function calculates the distance between every node inside a solution 

#### Input:
- Graph graph: The graph where all the nodes in the solution route are.
- Solution* solution: A pointer to the solution to check the route without making unnecessary copies.

#### Output:
- The distance between all the nodes in the solution route.
*/
double calculate_route_cost(Graph graph, Solution* solution) {
    double route_cost = 0.0;

    for (int i = 0; i < graph.size - 1; i++) {
        Node temp_a = graph.nodes[solution->route[i]];
        Node temp_b = graph.nodes[solution->route[i + 1]];

        route_cost += calculate_distance(temp_a, temp_b);
    }

    Node last = graph.nodes[solution->route[graph.size - 1]];
    Node first = graph.nodes[solution->route[0]];

    route_cost += calculate_distance(last, first);

    return route_cost;
}

/*
This functions generates a new neighbor making a swap between
two nodes inside an example solution.

#### Input:
- Graph graph: The graph where all the nodes in the solution route are.
- Solution current: The current solution.

#### Output:
- A new solution with a minor change to reevaluate.
*/
Solution generate_neighbor(Graph graph, Solution current) {
    Solution neighbor;

    neighbor.route = (int *)malloc(sizeof(int) * graph.size);

    for (int i = 0; i < graph.size; i++) {
        neighbor.route[i] = current.route[i];
    }

    int i = rand() % graph.size;
    int j = rand() % graph.size;

    while (j == i) {
        j = rand() % graph.size;
    }
        
    int temp = neighbor.route[i];
    neighbor.route[i] = neighbor.route[j];
    neighbor.route[j] = temp;

    return neighbor;
}

/*
This function generates a random initial solution using Fisher-Yates.

#### Input:
- Graph graph: The graph where all nodes are stored.

#### Output:
- A random valid solution.
*/
Solution generate_initial_solution(Graph graph) {
    Solution solution;

    solution.route = (int *)malloc(sizeof(int) * graph.size);

    for (int i = 0; i < graph.size; i++) {
        solution.route[i] = i;
    }

    for (int i = graph.size - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = solution.route[i];
        solution.route[i] = solution.route[j];
        solution.route[j] = temp;
    }

    solution.cost = 0.0;

    return solution;
}

/*
This function creates a deep copy of a solution.

#### Input:
- Graph graph: The graph where all the nodes in the solution route are.
- Solution* source: A pointer to the solution to copy.

#### Output:
- A new solution with its own allocated route.
*/
Solution copy_solution(Graph graph, Solution* source) {
    Solution copy;

    copy.cost = source->cost;
    copy.route = (int *)malloc(sizeof(int) * graph.size);

    for (int i = 0; i < graph.size; i++) {
        copy.route[i] = source->route[i];
    }

    return copy;
}

/*
This functions free the memory used for the route of a solution.

#### Input:
- Solution* solution: A pointer to the solution we want to free.

#### Output:
- Nothing.
*/
void free_solution(Solution* solution) {
    if (solution->route != NULL) {
        free(solution->route);
        solution->route = NULL;
    }
}

/* MAIN UTILS */

/*
This function creates a node with the given parameters.

#### Input:
- int id: Unique identifier of the node.
- double x: X coordinate of the node.
- double y: Y coordinate of the node.

#### Output:
- A Node struct initialized with the new values.
*/
Node create_node(char* id, double x, double y) {
    Node node;

    strcpy(node.id, id);
    node.x = x;
    node.y = y;

    return node;
}

/*
This function creates a graph with a given size.

#### Input:
- int size: Number of nodes the graph will contain.

#### Output:
- A Graph struct with allocated memory for nodes.
*/
Graph create_graph(int size) {
    Graph graph;

    graph.size = size;
    graph.nodes = (Node *)malloc(sizeof(Node) * size);

    return graph;
}

/*
This function assigns a node to a specific position in the graph.

#### Input:
- Graph* graph: Pointer to the graph.
- Node node: The node to insert.
- int position: Index where the node will be stored.

#### Output:
- None.
*/
void add_node(Graph* graph, Node node, int position) {
    graph->nodes[position] = node;
}

/*
This function frees the memory allocated for a graph.

#### Input:
- Graph* graph: Pointer to the graph to free.

#### Output:
- None.
*/
void free_graph(Graph* graph) {
    if (graph->nodes != NULL) {
        free(graph->nodes);
        graph->nodes = NULL;
    }
}

