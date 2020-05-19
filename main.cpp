#include <iostream>
#include <vector>
#include <set>


class Graph
{
private:
    long long vertexNumber;
    std::vector<std::vector<std::pair<long long, long long>>> vertices;
public:
    explicit Graph(long long n = 0) : vertexNumber(n) {vertices.resize(n);}
    bool IsValidVertex(long long vertex) const;
    void AddVertex();
    void AddEdge(long long from, long long to, long long weight);
    bool HasEdge(long long from, long long to) const;
    const std::vector<std::pair<long long, long long>>& GetNextVertices(long long from) const;
    long long VertexCount() const;
};

bool Graph::IsValidVertex(long long vertex) const {
    return vertex >= 0 && vertex < vertexNumber;
}

void Graph::AddVertex() {
    vertexNumber++;
    vertices.push_back({});
}

void Graph::AddEdge(long long from, long long to, long long weight) {
    if( !IsValidVertex(from) || !IsValidVertex(to) ) {
        return;
    }
    vertices[from].push_back({to, weight});
}

bool Graph::HasEdge(long long from, long long to) const {
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

const std::vector<std::pair<long long, long long>>& Graph::GetNextVertices(long long from) const {
    return vertices[from];
}

long long Graph::VertexCount() const {
    return vertexNumber;
}

long long Dijkstra(const Graph& graph, long long start, long long end) {
    const long long infinity = 1e18;
    long long n = graph.VertexCount();
    std::vector<long long> distance(n, infinity);
    distance[start] = 0;
    std::set<std::pair<long long, long long>> vertices;
    vertices.insert({distance[start], start});
    while( !vertices.empty() ) {
        long long vertex = vertices.begin()->second;
        vertices.erase(vertices.begin());
        auto adjacentVertices = graph.GetNextVertices(vertex);
        for( size_t j = 0; j < adjacentVertices.size(); ++j ) {
            long long to = adjacentVertices[j].first;
            long long length = adjacentVertices[j].second;
            if( distance[vertex] + length < distance[to] ) {
                vertices.erase({distance[to], to});
                distance[to] = distance[vertex] + length;
                vertices.insert({distance[to], to});
            }
        }
    }
    return distance[end];
}

int main() {
    long long a;
    long long b;
    long long M;
    long long x;
    long long y;
    std::cin >> a >> b >> M >> x >> y;
    Graph graph(M);
    for( long long i = 0; i < M; ++i ) {
        if( (i + 1) % M == (i * i + 1) % M ) {
            if( a < b ) {
                graph.AddEdge(i, (i + 1) % M, a);
            } else {
                graph.AddEdge(i, (i * i + 1) % M, b);
            }
        } else {
            graph.AddEdge(i, (i + 1) % M, a);
            graph.AddEdge(i, (i * i + 1) % M, b);
        }
    }
    std::cout << Dijkstra(graph, x, y);
    return 0;
}

