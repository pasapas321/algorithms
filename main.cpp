#include <iostream>
#include <vector>
#include <set>



class Graph
{
private:
    long long vertexNumber;
    std::vector<std::vector<std::pair<long long, double>>> vertices;
public:
    explicit Graph(long long n = 0) : vertexNumber(n) {vertices.resize(n);}
    bool IsValidVertex(long long vertex) const;
    void AddVertex();
    void AddEdge(long long from, long long to, double weight);
    bool HasEdge(long long from, long long to) const;
    const std::vector<std::pair<long long, double>>& GetNextVertices(long long from) const;
    long long VertexCount() const;
};

bool Graph::IsValidVertex(long long vertex) const {
    return vertex >= 0 && vertex < vertexNumber;
}

void Graph::AddVertex() {
    vertexNumber++;
    vertices.push_back({});
}

void Graph::AddEdge(long long from, long long to, double weight) {
    if( !IsValidVertex(from) || !IsValidVertex(to) ) {
        return;
    }
    vertices[from].push_back({to, weight});
    vertices[to].push_back({from, weight});
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

const std::vector<std::pair<long long, double>>& Graph::GetNextVertices(long long from) const {
    return vertices[from];
}

long long Graph::VertexCount() const {
    return vertexNumber;
}

double MinProbability(const Graph& graph, long long start, long long finish) {
    const double infinity = 1.5;
    long long n = graph.VertexCount();
    std::vector<double> probability(n, infinity);
    probability[start] = 0;
    std::set<std::pair<double, long long>> vertices;
    vertices.insert({probability[start], start});
    while( !vertices.empty() ) {
        long long vertex = vertices.begin()->second;
        vertices.erase(vertices.begin());
        auto adjacentVertices = graph.GetNextVertices(vertex);
        for( size_t i = 0; i < adjacentVertices.size(); ++i ) {
            long long to = adjacentVertices[i].first;
            double currentProbability = adjacentVertices[i].second;
            if( probability[vertex] + currentProbability - probability[vertex] * currentProbability < probability[to] ) {
                vertices.erase({probability[to], to});
                probability[to] = probability[vertex] + currentProbability - probability[vertex] * currentProbability;
                vertices.insert({probability[to], to});
            }
        }
    }
    return probability[finish];
}

int main() {
    long long N;
    long long M;
    long long S;
    long long F;
    std::cin >> N >> M >> S >> F;
    Graph graph(N);
    for( int i = 0; i < M; ++i ) {
        long long s;
        long long f;
        double p;
        std::cin >> s >> f >> p;
        graph.AddEdge(s - 1, f - 1, p / 100);
    }
    std::cout << MinProbability(graph, S - 1, F - 1);
    return 0;
}

