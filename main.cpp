#include <iostream>
#include <vector>
#include <algorithm>



class Graph
{
private:
    int vertexNumber;
    std::vector<std::vector<std::pair<int, std::pair<int, int>>>> vertices;
    std::vector<bool> visited;
public:
    explicit Graph(int n = 0);
    bool IsValidVertex(int vertex) const;
    void AddVertex();
    void AddEdge(int from, int to);
    bool HasEdge(int from, int to) const;
    void DeleteEdge(int from, int to);
    const std::vector<std::pair<int, std::pair<int, int>>>& GetNextVertices(int from) const;
    int VertexCount() const;
    int IncreasingPath(int vertex, int minCapacity, int t);
    int MaxFlow(int s, int t, int n);
    void FindPath(int s, int t);
    
};

Graph::Graph(int n) : vertexNumber(n) {
    vertices.resize(n);
    visited.assign(n, false);
}

bool Graph::IsValidVertex(int vertex) const {
    return vertex >= 0 && vertex < vertexNumber;
}

void Graph::AddVertex() {
    vertexNumber++;
    vertices.push_back({});
}

void Graph::AddEdge(int from, int to) {
    if( !IsValidVertex(from) || !IsValidVertex(to) ) {
        return;
    }
    vertices[from].push_back({to, {0, 1}}); // {vertex, {flow, capacity}}
    vertices[to].push_back({from, {0, 0}});
}

bool Graph::HasEdge(int from, int to) const {
    if( !IsValidVertex(from) || !IsValidVertex(to) ) {
        return false;
    }
    for( int i = 0; i < vertices[from].size(); ++i ) {
        if( vertices[from][i].first == to ) {
            return true;
        }
    }
    return false;
}

void Graph::DeleteEdge(int from, int to) {
    if( !IsValidVertex(from) || !IsValidVertex(to) ) {
        return;
    }
    int last = static_cast<int>(vertices[from].size()) - 1;
    for( int i = 0; i < last + 1; ++i ) {
        if( vertices[from][i].first == to ) {
            std::swap(vertices[from][i], vertices[from][last]);
            vertices[from].pop_back();
            return;
        }
    }
}

const std::vector<std::pair<int, std::pair<int, int>>>& Graph::GetNextVertices(int from) const {
    return vertices[from];
}

int Graph::VertexCount() const {
    return vertexNumber;
}

int Graph::IncreasingPath(int vertex, int minCapacity, int t) {
    if( vertex == t ) {
        return minCapacity;
    }
    visited[vertex] = true;
    for( size_t i = 0; i < vertices[vertex].size(); ++i ) {
        int to = vertices[vertex][i].first;
        int flow = vertices[vertex][i].second.first;
        int capacity = vertices[vertex][i].second.second;
        if( !visited[to] && flow < capacity ) {
            int delta = IncreasingPath(to, std::min(minCapacity, capacity - flow), t);
            if( delta > 0 ) {
                vertices[vertex][i].second.first += delta;
                for( size_t j = 0; j < vertices[to].size(); ++j ) {
                    if( capacity == 1 ) {
                        if( vertices[to][j].first == vertex && vertices[to][j].second.second == 0 ) {
                            vertices[to][j].second.first -= delta;
                            break;
                        }
                    } else {
                        if( vertices[to][j].first == vertex && vertices[to][j].second.second == 1 ) {
                            vertices[to][j].second.first -= delta;
                            break;
                        }
                    }
                }
                return delta;
            }
        }
    }
    return 0;
}

int Graph::MaxFlow(int s, int t, int n) {
    int maxFlow = 0;
    while( true ) {
        visited.assign(n, false);
        int delta = IncreasingPath(s - 1, 1, t - 1);
        if( delta != 0 ) {
            maxFlow += delta;
        } else {
            break;
        }
    }
    return maxFlow;
}

void Graph::FindPath(int s, int t) {
    int from = s;
    std::cout << s + 1 << " ";
    while( from != t ) {
        for( size_t i = 0; i < vertices[from].size(); ++i ) {
            int to = vertices[from][i].first;
            int flow = vertices[from][i].second.first;
            int capacity = vertices[from][i].second.second;
            if( capacity != 0 && capacity == flow ) {
                std::cout << to + 1 << " ";
                DeleteEdge(from, to);
                from = to;
                break;
            }
        }
    }
}

int main() {
    int n;
    int m;
    int s;
    int t;
    std::cin >> n >> m >> s >> t;
    Graph graph(n);
    for( int i = 0; i < m; ++i ) {
        int from;
        int to;
        std::cin >> from >> to;
        graph.AddEdge(from - 1, to - 1);
    }
    int maxFlow = graph.MaxFlow(s, t, n);
    if( maxFlow >= 2 ) {
        std::cout << "YES" << '\n';
        graph.FindPath(s - 1, t - 1);
        std::cout << '\n';
        graph.FindPath(s - 1, t - 1);
    } else {
        std::cout << "NO";
    }
    return 0;
}


