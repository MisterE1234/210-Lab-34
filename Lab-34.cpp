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

    //shortestPathsDetailed(): It uses the Dijkstra algorithm to create a pair of vectors that contains a vector path to each city:
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

    vector<Edge> MST_Prim(int start = 0) {
        vector<int> key(SIZE, INT_MAX);     // best edge weight to each node
        vector<int> parent(SIZE, -1);       // store MST parent node
        vector<bool> inMST(SIZE, false);    // included in MST?

        key[start] = 0; // start node

        // Build MST with SIZE - 1 edges
        for (int i = 0; i < SIZE - 1; i++) {
        
            // Pick the vertex u NOT in MST with the smallest key value
            int u = -1;
            int minValue = INT_MAX;
            for (int v = 0; v < SIZE; v++) {
                if (!inMST[v] && key[v] < minValue) {
                    minValue = key[v];
                    u = v;
                }
            }

            inMST[u] = true; // include u in MST

            // Update key[] and parent[] for neighbors of u
            for (auto &p : adjList[u]) {
                int v = p.first;
                int weight = p.second;

                // If v is not in MST and weight < current key
                if (!inMST[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                }
            }
        }

        // Convert parent[] into list of MST edges
        vector<Edge> mstEdges;
        for (int v = 0; v < SIZE; v++) {
            if (parent[v] != -1) {
                mstEdges.push_back({parent[v], v, key[v]});
            }
        }

        return mstEdges;
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

int menu(){
    bool valid = false;
    int choice;

    while(!valid){
        cout << "Road Map Network Menu:\n";
        cout << "[1] Display Road Map\n";
        cout << "[2] Display the Minimum number of Roads From City A (BFS)\n";
        cout << "[3] Display All Paths From City A to City B\n";
        cout << "[4] Calculate Shortest Paths From City A\n";
        cout << "[5] Find the Minimum Spanning Tree\n";
        cout << "[0] Exit\n";

        cout << "Enter Your Choice (0-5): ";

        cin >> choice;

        if(cin.fail()){
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Not an integer. Try again...\n";
        }
        else if(choice < 0 || choice > 5){
            cout << "Not in range. Try again...\n";
        }
        else{
            valid = true;
        }
        
    }

    return choice;
}

int citySelect(){
    int choice;
    bool valid = false;

    while(!valid){
        cout << "Choose a City: \n";
        cin >> choice;

        if(cin.fail()){
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Not an integer. Try again...\n";
        }
        else if(choice < 0 || choice >= SIZE){
            cout << "Not in range. Try again...\n";
        }
        else{
            valid = true;
        }

    }
}



int main() {

    bool exit = false;
    int startChoice = 0;
    int destChoice = 1;
    int count;

    vector<vector<int>> roads;
    vector<int> dist;
    vector<int> parent;
    vector<int> path;

    

    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        {0,1,9},{0,4,6},{0,3,21},{0,8,16},{1,2,8},{1,5,6},{3,5,14},{2,4,10},{2,5,4},{4,5,9},{5,6,8},{6,7,4},{2,8,12},{4,7,8}
    };

    
    // Creates graph
    Graph graph(edges);
    
    auto result = graph.shortestPathsDetailed(0);

    while(!exit){
        switch(menu()){

            case 0:{
                cout << "Exiting Program ...\n";
                exit = true;
                break;
            }

            case 1:{
                graph.displayRoadMap();
                cout << endl;
                break;
            }

            case 2:{

                cout << "Select Starting City: \n";
                startChoice = citySelect();

                //Displaying the minimum number of roads need to travel from city of choice to any city:
                cout << "\n(BFS) Number of Roads Needed to Traverse Starting from City " << startChoice << ":\n";
                roads = graph.BFS_levels(startChoice);

                //displaying each different level of different amount of roads:
                for(int i = 0; i < roads.size(); i++){
                    cout << "# of Roads " << i << ": ";
                    for(int v : roads[i]){
                        cout << v << " ";
                    }
                    cout << endl;
                }
                break;
            }

            case 3:{
                cout << "Select Starting City: \n";
                startChoice = citySelect();

                cout << "Select Destination City: \n";
                destChoice = citySelect();

                //Display all the routes from city A to city B:
                cout << "\n(DFS) Displaying all paths from " << startChoice << " to " << destChoice << ":\n";
                roads = graph.getAllPaths(startChoice, destChoice);

                sort(roads.begin(), roads.end(), [](const vector<int>& a, const vector<int>& b){
                    return a.size() < b.size(); // ascending order
                });

                for (auto& p : roads) {
                    for(int i = 0; i < p.size(); i++){
                        cout << p[i];
                        if(i != (p.size() - 1)){
                            cout << " -> ";
                        }
                    }
                    cout << endl;
                }
                break;
            }

            case 4:{
                cout << "Select Starting City: \n";
                startChoice = citySelect();
                
                //Displaying the shortest weighted route from the City of choice to every city:
                result = graph.shortestPathsDetailed(startChoice);
                dist = result.first;
                parent = result.second;

                cout << "\n=== Shortest Paths From City " << startChoice << " ===\n";

                //using a for loop to go through each city:
                for (int city = 0; city < SIZE; city++) {

                    cout << startChoice << " -> " << city << " : ";

                    //if the distance is infinite aka there is no path to other city:
                    if (dist[city] == INT_MAX) {
                        cout << "No path\n";
                        continue;
                    }

                    cout << dist[city] << " miles | Path: ";

                    //using the vector containing the number of cities and connections between each city to create a path to each.
                    path = graph.buildPath(city, parent);

                    //going through each city in a path to complete the display of the path
                    for (int i = 0; i < path.size(); i++) {
                        cout << path[i];
                        if (i < path.size() - 1) cout << " -> ";
                    }
                    cout << "\n";
                }
                break;
            }

            case 5:{
                // Displaying the MST using Prim's Algorithm
                cout << "\n=== Minimum Spanning Tree (Prim's Algorithm) ===\n";

                vector<Edge> mst = graph.MST_Prim(0);

                count = 0;

                //Using a for iterator loop to display each distinct edge:
                for (auto &e : mst) {
                    cout << e.src << " --(" << e.weight << " miles)-- " << e.dest << endl;
                    count += e.weight;
                }

                cout << "Total miles of MST: " << count << "\n";
                break;
            }

            default:{
                cout << "Error. Please try again...\n";
            }

        }
    }

            
    return 0;
}