//COMSC-210|Ethan Aylard|Lab-34
//IDE used: Visual Code Studio

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
#include <stack>
using namespace std;

const int SIZE = 7; bool debug = true;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair; // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;

    // a vector of edges to keep track of the edges:
    vector<Edge> originalEdges;
    // Graph Constructor
    Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);
        originalEdges = edges;

        // add edges to the directed graph
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, weight));
        }

    


    }

    //A helper:
    bool isOriginalEdge(int u, int v) {
    for (auto& e : originalEdges)
        if (e.src == u && e.dest == v)
            return true;
    return false;
}

    //using a Breadth First Search:
    vector<int> BFS(int start){
        //creating a vector to keep track of which nodes have already been visted:
        vector<bool> visited (SIZE, false); 
        queue<int> q;
        vector<int> order; // keeps track of visit order

        visited[start] = true;
        q.push(start);

        if(debug){
        cout << "BFS processing...\n";
        }

        //using a while loop to traverse through all the nodes:
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            order.push_back(v); //record visit

            // Sort neighbors by vertex ID to ensure consistent order:
            vector<int> neighbors;
            for(auto& p : adjList[v]){
                neighbors.push_back(p.first);
            }
            sort(neighbors.begin(), neighbors.end());

            //checking each neighbor node if they have been visted and adding them to q:
            for (int dest : neighbors){
                if (!visited[dest]){ //if the node at dest has not been visited:
                    visited[dest] = true;
                    q.push(dest);
                }
            }
        }
        return order;

    }

    //Using a Depth-First Search:
    vector<int> DFS(int start){
        //creating a vector to keep track of which nodes have already been visted:
        vector<bool> visited(SIZE, false);
        stack<int> st;
        vector<int> order;// keeps track of visit order

        st.push(start);

        if(debug){
        cout << "DFS processing...\n";
        }

        //using a while loop to traverse through all the nodes:
        while(!st.empty()){

            int v = st.top();
            st.pop();

            //Sort neighbors by vertex ID to ensure consistent order:
            if(!visited[v]){
                visited[v] = true;
                order.push_back(v); //record visit

                //Sort neighbors in reverse order so smallest comes out first:
                vector<int> neighbors;
                for(auto& p: adjList[v]){
                    neighbors.push_back(p.first);
                }
            }
                sort(neighbors.begin(), neighbors.end()); //ascending order

                //push in reverse order:
                for(int i = neighbors.size() - 1; i >= 0; i--){
                    if(!visited[neighbors[i]]){
                        st.push(neighbors[i]);
                    }
                    }
                }
            }
        
        return order;
    }

    // Print the graph's adjacency list
    void printGraph() {
        cout << "Graph's adjacency list:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
                cout << endl;
        }
    }

    
};

int main() {
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        {0,1,12},{0,2,8},{0,3,21},{2,3,6},{2,6,2},{5,6,6},{4,5,9},{2,4,4},{2,5,5}
    };

    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph
    graph.printGraph();

    //Print DFS from node 0:
    vector<int>dfsOrder = graph.DFS(0);
    cout << "DFS starting from vertex 0:\n";
    for(int v : dfsOrder) {
        cout << v << " ";
    }
    cout << endl;

    //Print BFS from node 0:
    vector<int>bfsOrder = graph.BFS(0);
    cout << "BFS starting from vertex 0:\n";
    for(int v : bfsOrder) {
        cout << v << " ";
    }
    cout << endl;
    

    

    return 0;
}