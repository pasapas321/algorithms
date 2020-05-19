#include <iostream>
#include <vector>
#include <set>



class Graph
{
private:
    int vertexNumber;
    std::vector<std::vector<std::pair<int, long long>>> vertices;
public:
    explicit Graph(int n = 0) : vertexNumber(n) {vertices.resize(n);}
    bool IsValidVertex(int vertex) const;
    void AddVertex();
    void AddEdge(int from, int to, long long weight);
    bool HasEdge(int from, int to) const;
    const std::vector<std::pair<int, long long>>& GetNextVertices(int from) const;
    int VertexCount() const;
};

bool Graph::IsValidVertex(int vertex) const {
    return vertex >= 0 && vertex < vertexNumber;
}

void Graph::AddVertex() {
    vertexNumber++;
    vertices.push_back({});
}

void Graph::AddEdge(int from, int to, long long weight) {
    if( !IsValidVertex(from) || !IsValidVertex(to) ) {
        return;
    }
    vertices[from].push_back({to, weight});
    vertices[to].push_back({from, weight});
}

bool Graph::HasEdge(int from, int to) const {
    if( !IsValidVertex(from) || !IsValidVertex(to) ) {
        return false;
    }
    for( size_t i = 0; i < vertices[from].size(); ++i ) {
        if( vertices[from][i].first == to ) {
            return true;
        }
    }
    return false;
}

const std::vector<std::pair<int, long long>>& Graph::GetNextVertices(int from) const {
    return vertices[from];
}

int Graph::VertexCount() const {
    return vertexNumber;
}

long long Prim( const Graph& graph ) {
    int n  = graph.VertexCount();
    const long long infinity = 1e18;
    std::vector<long long> minEdge(n, infinity);
    minEdge[0] = 0;
    std::set<std::pair<long long, int>> vertices;
    vertices.insert({0, 0});
    for( int i = 0; i < n - 1; ++i ) {
        vertices.insert({minEdge[i + 1], i + 1});
    }
    while( !vertices.empty() ) {
        int vertex = vertices.begin()->second;
        vertices.erase(vertices.begin());
        auto adjacencyList = graph.GetNextVertices(vertex);
        for( size_t j = 0; j < adjacencyList.size(); ++j ) {
            int to = adjacencyList[j].first;
            long long weight = adjacencyList[j].second;
            if( vertices.count({minEdge[to], to}) && weight < minEdge[to] ) {
                vertices.erase({minEdge[to], to});
                minEdge[to] = weight;
                vertices.insert({minEdge[to], to});
            }
        }
    }
    long long WeightMST = 0;
    for( int i = 0; i < n; ++i ) {
        WeightMST += minEdge[i];
    }
    return WeightMST;
}

void ChangeSets(std::set<std::pair<long long, int>>& markedVertices,
                std::set<std::pair<long long, int>>& notMarkedVertices) {
    std::pair<long long, int> vertex;
    vertex.first = notMarkedVertices.begin()->first;
    vertex.second = notMarkedVertices.begin()->second;
    markedVertices.insert(vertex);
    notMarkedVertices.erase(notMarkedVertices.begin());
}

int main() {
    int n;
    int m;
    std::cin >> n >> m;
    std::set<std::pair<long long, int>> markedVertices;
    std::set<std::pair<long long, int>> notMarkedVertices;
    for( int i = 0; i < n; ++i ) {
        long long weight;
        std::cin >> weight;
        notMarkedVertices.insert({weight, i});
    }
    Graph graph(n);
    ChangeSets(markedVertices, notMarkedVertices);
    while( !notMarkedVertices.empty() ) {
        int from = markedVertices.begin()->second;
        int to = notMarkedVertices.begin()->second;
        long long weight = markedVertices.begin()->first + notMarkedVertices.begin()->first;
        graph.AddEdge(from, to, weight);
        ChangeSets(markedVertices, notMarkedVertices);
    }
    for( int i = 0; i < m; ++i ) {
        int x;
        int y;
        long long w;
        std::cin >> x >> y >> w;
        graph.AddEdge(x - 1, y - 1, w);
    }
    std::cout << Prim(graph);
    return 0;
}

