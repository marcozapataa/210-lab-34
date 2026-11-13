#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <tuple>
#include <functional>
#include <climits>
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
			cout << "  -> Facility " << p.first
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

// Shortest paths (Dijkstra's algorithm): finds the minimum total distance from
// the start facility to every other facility, always settling the closest
// facility that has not been finalized yet.
void shortestPaths(Graph &g, int start) {
	vector<int> dist(SIZE, INT_MAX);
	// each queue entry is (distance so far, facility)
	priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
	dist[start] = 0;
	pq.push(make_pair(0, start));
	while (!pq.empty()) {
		int d = pq.top().first;
		int v = pq.top().second;
		pq.pop();
		if (d > dist[v])
			continue; // stale entry: a shorter route was already found
		for (Pair p : g.adjList[v]) {
			int next = p.first;
			int newDist = d + p.second;
			if (newDist < dist[next]) {
				dist[next] = newDist;
				pq.push(make_pair(newDist, next));
			}
		}
	}

	cout << "Shortest path from node " << start << ":" << endl;
	for (int i = 0; i < SIZE; i++) {
		cout << start << " -> " << i << " : ";
		if (dist[i] == INT_MAX)
			cout << "unreachable" << endl;
		else
			cout << dist[i] << endl;
	}
}

// Minimum spanning tree (Prim's algorithm): starting at the given facility,
// repeatedly add the cheapest route that reaches a facility not yet connected.
void minimumSpanningTree(Graph &g, int start) {
	cout << "Minimum Spanning Tree edges:" << endl;

	// each queue entry is (route distance, facility reached, facility it came from)
	typedef tuple<int, int, int> Candidate;
	priority_queue<Candidate, vector<Candidate>, greater<Candidate>> pq;
	vector<bool> inTree(SIZE, false);

	pq.push(make_tuple(0, start, -1));
	while (!pq.empty()) {
		int miles = get<0>(pq.top());
		int v = get<1>(pq.top());
		int from = get<2>(pq.top());
		pq.pop();
		if (inTree[v])
			continue; // already connected by a cheaper route
		inTree[v] = true;
		if (from != -1)
			cout << "Edge from " << v << " to " << from
			     << " with distance: " << miles << " miles" << endl;
		for (Pair p : g.adjList[v])
			if (!inTree[p.first])
				pq.push(make_tuple(p.second, p.first, v));
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
	cout << endl;
	shortestPaths(graph, 0);
	cout << endl;
	minimumSpanningTree(graph, 0);
	return 0;
}