#ifndef annealing
#define annealing

/* On the intended application, represents a city */
typedef struct Node {
    int id;
    double x, y;
} Node;

/* On the intended application, represents a set of cities */
typedef struct Graph {
    Node* nodes;
    int size;
} Graph;

/* It represents a possible solution */
typedef struct Solution {
    int* route;
    double cost;
} Solution;

#endif