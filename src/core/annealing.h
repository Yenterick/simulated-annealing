#ifndef annealing
#define annealing

/* On the intended application, represents a city */
typedef struct Node {
    char id[20];
    double x, y;
} Node;

/* On the intended application, represents a set of cities */
typedef struct Graph {
    Node* nodes;
    unsigned int size;
} Graph;

/* It represents a possible solution */
typedef struct Solution {
    int* route;
    double cost;
} Solution;

Solution simulated_annealing(Graph graph, double temperature, double min_temperature, double cooling_rate, double max_time, unsigned int max_on_same);

Node create_node(char* id, double x, double y);
Graph create_graph(int size);
void add_node(Graph* graph, Node node, int position);
void free_graph(Graph* graph);

#endif