#include <fstream>

// Define the maximum number of nodes in the graph
const unsigned int MAX_NODES = 20001;

// Define the structure for an edge in the graph
struct Edge {
    unsigned int dest; // Assigned node of the edge
    Edge* next; // Pointer to the next edge

    // Constructor to initialize the edge with its destination
    Edge(unsigned int d) : dest(d), next(nullptr) {}
};

// Define the structure for a node in the graph
struct Node {
    Edge* edges; // Linked list of edges connected to this node
    unsigned int shortestPath; // Shortest path from this node to any other node

    // Constructor to initialize the node
    Node() : edges(nullptr), shortestPath(MAX_NODES) {}

    // Function to add an edge to the node
    void addEdge(unsigned int dest) {
        // Create a new edge and add it to the node's edges
        Edge* newEdge = new Edge(dest);
        newEdge->next = edges;
        edges = newEdge;
    }
};

// Define the structure for the graph
struct Graph {
    Node nodes[MAX_NODES]; // Array of nodes in the graph
    unsigned int numNodes; // Number of nodes in the graph

    // Constructor to initialize the graph
    Graph() : numNodes(0) {}

    // Function to add an edge between two nodes. Link is store in both nodes.
    void addEdge(unsigned int u, unsigned int v) {
        nodes[u].addEdge(v);
        nodes[v].addEdge(u);
    }

    // Function to perform Breadth-First Search (BFS) from a given source node
    unsigned int bfs(unsigned int source) {
        unsigned int levels[MAX_NODES]; // Array to store levels of nodes during BFS
        for (int i = 1; i <= numNodes; ++i) {
            levels[i] = MAX_NODES; // Initialize levels to maximum value
        }

        levels[source] = 0; // Set level of source node to 0

        unsigned int queue[MAX_NODES]; // Queue for BFS traversal
        unsigned int front = 0, rear = 0; // Pointers for queue
        queue[rear++] = source; // Enqueue the source node

        unsigned int maxLevels = 0; // Initialize maximum level encountered during BFS

        // Perform BFS traversal
        while (front != rear) {
            unsigned int u = queue[front++]; // Dequeue a node

            for (Edge* edge = nodes[u].edges; edge != nullptr; edge = edge->next) {
                unsigned int v = edge->dest; // Get adjacent node
                if (levels[v] == MAX_NODES) {
                    levels[v] = levels[u] + 1; // Update level of adjacent node
                    queue[rear++] = v; // Enqueue the adjacent node
                    if(levels[v] > maxLevels) {
                        maxLevels = levels[v]; // Update maximum level encountered
                    }
                }
            }
        }

        // Update shortest path of the source node
        nodes[source].shortestPath = maxLevels;
        return maxLevels; // Return maximum level encountered
    }

    // Function to find and write nodes with shortest path and shortest "time" to reach end of the graph to an output file
    void findShortestNodes(unsigned int minLevels, std::ofstream& outputFile) {
        outputFile << minLevels << std::endl; // Write minimum levels to the output file
        for (unsigned int i = 1; i <= numNodes; ++i) {
            if (nodes[i].shortestPath == minLevels) {
                outputFile << i << " "; // Write nodes with shortest path to the output file
            }
        }
    }
};

int main() {
    // Initialize graph and edge variables
    Graph graph;
    unsigned int u, v;

    // Open input file
    std::ifstream inputFile("server.in");

    // Read number of nodes, set number of nodes
    inputFile >> graph.numNodes;

    // Read edges and add them to the graph
    while (inputFile >> u >> v) {
        if (u == 0 && v == 0) { // Read input until reaching "0 0" input
            break;
        }
        graph.addEdge(u, v); // Add edges to the graph
    }

    unsigned int minLevels = MAX_NODES; // Initialize minimum levels

    // Perform BFS for each node to find the shortest path
    for (unsigned int i = 1; i <= graph.numNodes; ++i) {
        minLevels = std::min(minLevels, graph.bfs(i)); // Find the minimum levels encountered
    }

    // Open output file
    std::ofstream outputFile("server.out");

    // Find and write nodes with shortest path and shortest "time" to reach end of the graph to an output file
    graph.findShortestNodes(minLevels, outputFile);

    // Close input and output files
    inputFile.close();
    outputFile.close();
    return 0;
}
