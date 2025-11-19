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
    // Graph Constructor
    Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

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
    //using a Breadth First Search:
    void BFS(int start){
        //creating a vector to keep track of which nodes have already been visted:
        vector<bool> visited (SIZE, false); 
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "BFS starting from vertex " << start << ":\n";

        //using a while loop to traverse through all the nodes:
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            cout << v << " ";

            // Sort neighbors by vertex ID to ensure consistent order:
            vector<int> neighbors;
            for(auto &neighbor : adjList[v]){
                neighbors.push_back(neighbor.first);
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
        cout << endl;

    }

    //Using a Depth-First Search:
    void DFS(int start){
        //creating a vector to keep track of which nodes have already been visted:
        vector<bool> visited(SIZE, false);
        stack<int> st;

        st.push(start);

        cout << "DFS starting from vertex " << start << ":\n";

        //using a while loop to traverse through all the nodes:
        while(!st.empty()){

            int v = st.top();
            st.pop();

            //Sort neighbors by vertex ID to ensure consistent order:
            if(!visited[v]){
                visited[v] = true;
                cout << v << " ";

                //Sort neighbors in reverse order so smallest comes out first:
                vector<int> neighbors;
                for(auto &neighbor: adjList[v]){
                    neighbors.push_back(neighbor.first);
                }
                sort(neighbors.begin(), neighbors.end(), greater<int>());

                for(int dest : neighbors){
                    if(!visited[dest]){
                        st.push(dest);
                    }
                }
            }
        }
        cout << endl;
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
    graph.DFS(0);

    //Print BFS from node 0:
    graph.BFS(0);

    

    return 0;
}