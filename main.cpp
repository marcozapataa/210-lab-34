#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
using namespace std;
const int SIZE = 9;
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

	// Iterative DFS: a vertex is marked visited when it is popped
	void DFS(int start) {
		vector<bool> visited(adjList.size(), false);
		stack<int> s;
		s.push(start);
		while (!s.empty()) {
			int v = s.top();
			s.pop();
			if (visited[v])
				continue; // already visited via another path
			visited[v] = true;
			cout << v << " ";
			// push all neighbors in adjacency-list order
			for (Pair p : adjList[v])
				s.push(p.first);
		}
		cout << endl;
	}

	// BFS: a vertex is marked visited when it is enqueued
	void BFS(int start) {
		vector<bool> visited(adjList.size(), false);
		queue<int> q;
		visited[start] = true;
		q.push(start);
		while (!q.empty()) {
			int v = q.front();
			q.pop();
			cout << v << " ";
			for (Pair p : adjList[v]) {
				if (!visited[p.first]) {
					visited[p.first] = true;
					q.push(p.first);
				}
			}
		}
		cout << endl;
	}
};

// ---------------------------------------------------------------------------
// APPLICATION LAYER: Regional Parcel Delivery Network
// Each vertex is a facility; each edge is a two-way truck route whose weight
// is the driving distance in miles.
// ---------------------------------------------------------------------------
const string ROLES[SIZE] = {
	"Central Warehouse",   // 0
	"Downtown Depot",      // 1
	"Airport Cargo Hub",   // 2
	"Rail Terminal",       // 3
	"Seaport Dock",        // 4
	"Northgate Locker",    // 5
	"Lakeside Store",      // 6
	"Hillcrest Center",    // 7
	"Riverside Hub"        // 8
};

// "Facility 2 (Airport Cargo Hub)"
string label(int v) {
	return "Facility " + to_string(v) + " (" + ROLES[v] + ")";
}

// Prints a heading followed by a line of '=' the same length
void heading(const string &text) {
	cout << text << endl;
	cout << string(text.size(), '=') << endl;
}

// Prints every facility and the routes that connect to it
void printNetwork(Graph &g) {
	heading("Regional Parcel Delivery Network Topology:");
	for (int i = 0; i < SIZE; i++) {
		cout << label(i) << " connects to:" << endl;
		for (Pair p : g.adjList[i])
			cout << " -> Facility " << p.first
			     << " (Distance: " << p.second << " miles)" << endl;
	}
}

// DFS trace: a vertex is marked as discovered when it is pushed, so only
// newly discovered neighbors are pushed (and reported) from each facility.
void networkTraceDFS(Graph &g, int start) {
	cout << "Network Trace (DFS) from " << label(start) << ":" << endl;
	cout << "Purpose: Tracing how a shipping delay could spread through the network" << endl;
	cout << string(66, '=') << endl;
	vector<bool> discovered(SIZE, false);
	stack<int> s;
	discovered[start] = true;
	s.push(start);
	while (!s.empty()) {
		int v = s.top();
		s.pop();
		cout << "Inspecting " << label(v) << endl;
		for (Pair p : g.adjList[v]) {
			if (!discovered[p.first]) {
				discovered[p.first] = true;
				s.push(p.first);
				cout << "  -> Possible delay spread to " << label(p.first)
				     << " - Distance: " << p.second << " miles" << endl;
			}
		}
	}
}

// BFS review: a vertex is marked as discovered when it is enqueued
void layerInspectionBFS(Graph &g, int start) {
	cout << "Layer-by-Layer Network Review (BFS) from " << label(start) << ":" << endl;
	cout << "Purpose: Grouping delivery zones by number of stops from the warehouse" << endl;
	cout << string(70, '=') << endl;
	vector<bool> discovered(SIZE, false);
	queue<int> q;
	discovered[start] = true;
	q.push(start);
	while (!q.empty()) {
		int v = q.front();
		q.pop();
		cout << "Checking " << label(v) << endl;
		for (Pair p : g.adjList[v]) {
			if (!discovered[p.first]) {
				discovered[p.first] = true;
				q.push(p.first);
				cout << "  -> Next delivery zone: " << label(p.first)
				     << " - Distance: " << p.second << " miles" << endl;
			}
		}
	}
}

int main() {
	// Creates a vector of graph edges/weights (weight = route length in miles)
	vector<Edge> edges = {
		// (x, y, w) —> edge from x to y having weight w
		{0,1,8},{0,2,21},{1,2,6},{1,3,5},{1,4,4},{3,4,9},
		{5,6,10},{6,7,3},{5,7,15},{5,8,5},{6,8,7},{2,7,11},{2,8,8}
	};
	// Creates graph
	Graph graph(edges);

	printNetwork(graph);
	cout << endl;
	networkTraceDFS(graph, 0);
	cout << endl;
	layerInspectionBFS(graph, 0);
	return 0;
}