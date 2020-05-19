#include <iostream>
#include <vector>
#include <queue>


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

long long MinCost(const Graph& graph, long long start, long long finish, long long K) {
    std::queue<long long> vertices;
    vertices.push(start);
    long long n = graph.VertexCount();
    const long long infinity = 1e18;
    std::vector<long long> cost(n, infinity);
    cost[start] = 0;
    std::vector<long long> layer(n, 0);
    while( !vertices.empty() ) {
        long long vertex = vertices.front();
        vertices.pop();
        auto adjacentVertices = graph.GetNextVertices(vertex);
        for( size_t i = 0; i < adjacentVertices.size(); ++i ) {
            long long to = adjacentVertices[i].first;
            long long length = adjacentVertices[i].second;
            if( cost[vertex] + length < cost[to] ) {
                layer[to] = layer[vertex] + 1;
                if( layer[to] <= K ){
                    cost[to] = cost[vertex] + length;
                    vertices.push(to);
                }
            }
        }
    }
    return cost[finish] == infinity ? -1 : cost[finish];
}

int main() {
    long long N;
    long long M;
    long long K;
    long long S;
    long long F;
    std::cin >> N >> M >> K >> S >> F;
    Graph graph(N);
    for( int i = 0; i < M; ++i ) {
        long long s;
        long long f;
        long long p;
        std::cin >> s >> f >> p;
        graph.AddEdge(s - 1, f - 1, p);
    }
    std::cout << MinCost(graph, S - 1, F - 1, K);
    return 0;
}

