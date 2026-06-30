#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* custom imports */
#include "annealing/annealing.h"

/* definitions */
#define TEMPERATURE     1000.0
#define MIN_TEMPERATURE 0.01
#define COOLING_RATE    0.995
#define MAX_TIME        120
#define MAX_ON_SAME     100

int main(int argc, char *argv[]) {
    printf("\n");

    /* Checks if the parameters were correctly given */
    if (argc == 1) {
        printf("Not enough arguments were given...\n");
        printf("Example: annealing ./input.txt ./output.txt\n\n");
        return 1;
    }

    /* Tries to open the file and checks if it was opened successfully */
    FILE* input = fopen(argv[1], "r");

    if (input == NULL) {
        printf("The file couldn't be opened...\n\n");
        return 1;
    }

    /* Creates the output file */
    FILE* output;

    if (argv[2]) {
        output = fopen(argv[2], "w");
    } else {
        output = fopen("./output.txt", "w");
    }

    /* Reads the content of the file and creates the nodes */
    Graph graph;
    unsigned int graph_size = 0;
    fscanf(input, "%u", &graph_size);

    if (graph_size > 0) {
        graph = create_graph(graph_size);
    } else {
        printf("There was an error creating the graph...\n\n");
        return 1;
    }

    for (int i = 0; i < graph.size; i++) {
        char id[20];
        double x, y;
        fscanf(input, "%s %lf %lf", id, &x, &y);
        Node node = create_node(id, x, y);
        add_node(&graph, node, i);
    }

    /* Initializes the srand seed */
    srand(time(NULL));

    /* Runs the algorithm */
    Solution solution = simulated_annealing(
        graph, 
        TEMPERATURE, MIN_TEMPERATURE, 
        COOLING_RATE, 
        MAX_TIME, 
        MAX_ON_SAME
    );

    /* Writes the solution on the output file */
    fprintf(output, "%u\n", graph.size);

    for (int i = 0; i < graph_size; i ++) {
        Node node = graph.nodes[solution.route[i]];
        fprintf(output, "%s %lf %lf\n", node.id, node.x, node.y);
    }

    fprintf(output, "total_cost %.2lf\n", solution.cost);

    /* Rewinds the output pointer */
    rewind(output);

    /* Closes the file connections */
    fclose(input);
    fclose(output);

    return 0;
}