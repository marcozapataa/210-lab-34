#include <iostream>
#include <vector>
#include <stack>
#include <queue>
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
int main() {
	// Creates a vector of graph edges/weights
	vector<Edge> edges = {
		// (x, y, w) —> edge from x to y having weight w
		{0,1,8},{0,2,21},{1,2,6},{1,3,5},{1,4,4},{3,4,9},
        {5,6,10},{6,7,3},{5,7,15},{5,8,5},{6,8,7},{2,7,11},{2,8,8}
	};
	// Creates graph
	Graph graph(edges);
	// Prints adjacency list representation of graph
	graph.printGraph();

	cout << "DFS starting from vertex 0:" << endl;
	graph.DFS(0);

	cout << "BFS starting from vertex 0:" << endl;
	graph.BFS(0);
	return 0;
}