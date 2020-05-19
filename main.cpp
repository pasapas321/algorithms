#include <iostream>
#include <vector>
#include <algorithm>



class Graph
{
private:
    int vertexNumber;
    int edgeNumber;
    std::vector<std::pair<long long, std::pair<int, int>>> vertices;
public:
    explicit Graph(int n = 0, int m = 0) : vertexNumber(n), edgeNumber(m) {}
    bool IsValidVertex(int vertex) const;
    void AddEdge(long long weight, int from, int to);
    bool HasEdge(int from, int to) const;
    const std::pair<long long, std::pair<int, int>>& GetEdge(int number) const;
    int VertexCount() const;
    int EdgesCount() const;
    void Sort();
};

bool Graph::IsValidVertex(int vertex) const {
    return vertex >= 0 && vertex < vertexNumber;
}

void Graph::AddEdge(long long weight, int from, int to) {
    if( !IsValidVertex(from) || !IsValidVertex(to) ) {
        return;
    }
    vertices.push_back({weight, {from, to}});
}

bool Graph::HasEdge(int from, int to) const {
    if( !IsValidVertex(from) || !IsValidVertex(to) ) {
        return false;
    }
    for( size_t i = 0; i < vertices.size(); ++i ) {
        if( vertices[i].second.first == from && vertices[i].second.second == to ) {
            return true;
        }
    }
    return false;
}

const std::pair<long long, std::pair<int, int>>& Graph::GetEdge(int number) const {
    return vertices[number];
}

int Graph::VertexCount() const {
    return vertexNumber;
}

int Graph::EdgesCount() const {
    return edgeNumber;
}

void Graph::Sort() {
    std::sort( vertices.begin(), vertices.end() );
}

class DSU {
private:
    std::vector<int> parent;
    std::vector<int> rank;
public:
    DSU(int n = 0);
    void MakeSet(int vertex);
    int FindSet(int vertex);
    void UnionSets(int a, int b);
};

DSU::DSU(int n) {
    parent.resize(n);
    rank.resize(n);
}

void DSU::MakeSet(int vertex) {
    parent[vertex] = vertex;
    rank[vertex] = 0;
}

int DSU::FindSet(int vertex) {
    return (vertex == parent[vertex]) ? vertex : (parent[vertex] = FindSet(parent[vertex]));
}

void DSU::UnionSets(int a, int b) {
    a = FindSet(a);
    b = FindSet(b);
    if( a != b ) {
        if( rank[a] < rank[b] ) {
            std::swap(a, b);
        }
        parent[b] = a;
        if( rank[a] == rank[b] ) {
            ++rank[a];
        }
    }
}

long long Kruskal(Graph& graph) {
    long long weightMST = 0;
    graph.Sort();
    int n = graph.VertexCount();
    DSU unions(n);
    for( int i = 0; i < n; ++i ) {
        unions.MakeSet(i);
    }
    int m = graph.EdgesCount();
    for( int i = 0; i < m; ++i ) {
        auto edge = graph.GetEdge(i);
        int from = edge.second.first;
        int to = edge.second.second;
        long long weight = edge.first;
        if( unions.FindSet(from) != unions.FindSet(to) ) {
            weightMST += weight;
            unions.UnionSets(from, to);
        }
    }
    return weightMST;
}

int main() {
    int n;
    int m;
    std::cin >> n >> m;
    Graph graph(n, m);
    for( int i = 0; i < m; ++i ) {
        int b;
        int e;
        long long w;
        std::cin >> b >> e >> w;
        graph.AddEdge(w, b - 1, e - 1);
    }
    std::cout << Kruskal(graph);
    return 0;
}

