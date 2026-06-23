#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* custom imports */
#include "annealing.h"

double calculate_distance(Node a, Node b);
double calculate_route_cost(Graph graph, Solution* solution);
Solution generate_neighbor(Graph graph, Solution current);

/** 
* ### Simulated Annealing
* 
* This function implements the Simulated Annealing algortihm.
* This algorithm allows hill-cimbing moves to let the program
* escape the local optima, in hopes of finding the global optimum.
* 
* Applied in the TSP. Seeking to be applied in the routing of 
* refrigerated vehicles.
* 
* #### Input:
* 
* #### Output:
* 
*/
double simulated_annealing() {

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

    free(neighbor.route);

    return neighbor;
}
