//COMSC-210|Ethan Aylard|Lab-34
//IDE used: Visual Code Studio

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <stack>
using namespace std;


const int SIZE = 9; bool debug = false;
//Struct to keep track of the edges:
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
    vector<int> BFS(const Graph& graph, int start){
        //creating a vector to keep track of which nodes have already been visted:
        vector<bool> visited (SIZE, false); 
        queue<int> q;
        vector<int> order; // keeps track of visit order

        //set the visited status of the starting value to true and start the queue at the starting value
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

            // for each entry(node) in the graph adjecent list at node v, check to see if it has been visited.
            for (auto &p : graph.adjList[v]){
                int v = p.first;
                if(!visited[v]){
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        return order;

    }

    //Using BFS to create a list of how many roads someone would have to travel to get to a city from a point:
    vector<vector<int>> BFS_levels(int start){
        //creating a vector to keep track of which nodes have already been visted:
        vector<bool> visited (SIZE, false); 
        queue<int> q;
        vector<vector<int>> levels; // keeps track of visit order

        //set the visited status of the starting value to true and start the queue at the starting value
        visited[start] = true;
        q.push(start);

        if(debug){
        cout << "BFS_levels processing...\n";
        }

        //using a while loop to traverse through all the nodes:
        while (!q.empty()) {
            int levelSize = q.size();
            vector<int> level;

            // for each entry(node) in the graph adjecent list at node v, check to see if it has been visited.
            for (int i = 0; i < levelSize; i++){
                int v = q.front();
                q.pop();
                level.push_back(v);

                //explore neighbors
                for (auto &p : adjList[v]){
                    int next = p.first;
                    if(!visited[next]){
                        visited[next] = true;
                        q.push(next);
                    }
                }

            }
            levels.push_back(level);
        }
        return levels;

    }

    //Using a Depth-First Search:
    vector<int> DFS(const Graph &g, int start){
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

            //if value v has not been visited 
            if(!visited[v]){
                //setting visited to true at value v and adding v to the order:
                visited[v] = true;
                order.push_back(v); //record visit

                
                // for each entry(node) in the graph adjecent list at node v, check to see if it has been visited.
                for(auto& p: g.adjList[v]){
                    int v = p.first;
                    if(!visited[v]){
                        st.push(v);
                    }
                }
            }
        }
        
        return order;
    }

    //A helper: Using DFS to find all paths that link two cities:
    void DFS_allPaths(int current, int dest, vector<bool>& visited, vector<int>& path, vector<vector<int>>& allPaths){
        //creating a vector to keep track of which nodes have already been visted:
        
        if(debug){
        cout << "DFS_AllPaths processing...\n";
        }

        visited[current] = true;
        path.push_back(current);

        if(current == dest){
            allPaths.push_back(path);
        }
        else {
            for(auto& p : adjList[current]){
                int next = p.first;
                if(!visited[next]){
                    DFS_allPaths(next, dest, visited, path, allPaths);
                }
            }
        }
    
        //Backtrack:
        path.pop_back();
        visited[current] = false;
       
    }
    //A base function to get a vector containing all the paths from city A to B:
    vector<vector<int>> getAllPaths(int start, int dest){
        vector<bool> visited(SIZE, false);
        vector<int> path;
        vector<vector<int>> allPaths;// keeps track of what paths are possible

        DFS_allPaths(start, dest, visited, path, allPaths);

        return allPaths;
    }

    //A helper for shortestPathsDetailed(): 
    vector<int> buildPath(int dest, const vector<int>& parent) {
        vector<int> path;
        //adding the cities that in the path to the path:
        for (int v = dest; v != -1; v = parent[v]) {
            path.push_back(v);
        }
        //showing the path as the way that you will come across:
        reverse(path.begin(), path.end());
        return path;
    }

    //shortestPathsDetailed(): It creates a pair of vectors that contains the
    pair<vector<int>, vector<int>> shortestPathsDetailed(int start) {
        vector<int> dist(SIZE, INT_MAX);
        vector<int> parent(SIZE, -1);

        dist[start] = 0;
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [currDist, node] = pq.top();
            pq.pop();

            if (currDist > dist[node]) continue;

            for (auto &edge : adjList[node]) {
                int next = edge.first;
                int weight = edge.second;

                if (dist[node] + weight < dist[next]) {
                    dist[next] = dist[node] + weight;
                    parent[next] = node;
                    pq.push({dist[next], next});
                }
            }
        }
        return {dist, parent};
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

    //Display the graph's adjacecny list as road map.
    void displayRoadMap(){
        cout << "\n=== Road Map ===\n";
        for(int src = 0; src < adjList.size(); src++){
            for(auto &p : adjList[src]){
                int dest = p.first;
                int weight = p.second;

                cout << "City " << src
                    << " <---- " << weight << " miles ----> "
                    << "City " << dest << endl;
                
            }
            cout << endl;
        }
        cout << "================\n";
    }

    
};



int main() {
    int startChoice = 0;
    int destChoice = 1;
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        {0,1,9},{0,4,6},{0,3,21},{0,8,16},{1,2,8},{1,5,6},{3,5,14},{2,4,10},{2,5,4},{4,5,9},{5,6,8},{6,7,4},{2,8,12},{4,7,8}
    };

    
    // Creates graph
    Graph graph(edges);

    if(debug){
        // Prints adjacency list representation of graph
        graph.printGraph();

        //Print DFS from node 0:
        vector<int>dfsOrder = graph.DFS(graph, startChoice);
        cout << "DFS starting from vertex 0:\n";
        for(int v : dfsOrder) {
            cout << v << " ";
        }
        cout << endl;

        //Print BFS from node 0:
        vector<int>bfsOrder = graph.BFS(graph, startChoice);
        cout << "BFS starting from vertex 0:\n";
        for(int v : bfsOrder) {
            cout << v << " ";
        }
    }

    graph.displayRoadMap();
    cout << endl;

    //Display all the routes from city A to city B:
    cout << "\n(DFS) Displaying all paths from " << startChoice << " to " << destChoice << ":\n";
    vector<vector<int>> paths = graph.getAllPaths(startChoice, destChoice);

    sort(paths.begin(), paths.end(), [](const vector<int>& a, const vector<int>& b){
        return a.size() < b.size(); // ascending order
    });

    for (auto& p : paths) {
        for(int i = 0; i < p.size(); i++){
            cout << p[i];
            if(i != (p.size() - 1)){
                cout << " -> ";
            }
       }
        cout << endl;
    }

    //Displaying the minimum number of roads need to travel from city of choice to any city:
    cout << "\n(BFS) Number of Roads Needed to Traverse Starting from City " << startChoice << ":\n";
    vector<vector<int>> levels = graph.BFS_levels(startChoice);

    //displaying each different level of different amount of roads:
    for(int i = 0; i < levels.size(); i++){
        cout << "# of Roads " << i << ": ";
        for(int v : levels[i]){
            cout << v << " ";
        }
        cout << endl;
    }

    //Displaying the shortest weighted route from the City of choice to every city:
    auto result = graph.shortestPathsDetailed(startChoice);
    vector<int> dist = result.first;
    vector<int> parent = result.second;

    cout << "\n=== Shortest Paths From City " << startChoice << " ===\n";

    for (int city = 0; city < SIZE; city++) {

        cout << startChoice << " -> " << city << " : ";

        if (dist[city] == INT_MAX) {
            cout << "No path\n";
            continue;
        }

        cout << dist[city] << " miles | Path: ";

        vector<int> path = graph.buildPath(city, parent);

        for (int i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << "\n";
    }


    return 0;
}