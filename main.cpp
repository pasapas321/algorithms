#include <iostream>
#include <vector>
#include <algorithm>



class Graph
{
private:
    int vertexNumber;
    std::vector<std::vector<int>> vertices;
public:
    explicit Graph(int n = 0);
    bool IsValidVertex(int vertex) const;
    void AddVertex();
    void AddEdge(int from, int to);
    bool HasEdge(int from, int to) const;
    void DeleteEdge(int from, int to);
    int VertexCount() const;
    std::vector<int> MinCut();
};

Graph::Graph(int n) : vertexNumber(n) {
    vertices.resize(n);
    for( int i = 0; i < n; ++i ) {
        vertices[i].resize(n);
    }
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
    vertices[from][to] = 1;
}

bool Graph::HasEdge(int from, int to) const {
    if( !IsValidVertex(from) || !IsValidVertex(to) ) {
        return false;
    }
    if( vertices[from][to] == 1 ) {
        return true;
    }
    return false;
}

void Graph::DeleteEdge(int from, int to) {
    if( !IsValidVertex(from) || !IsValidVertex(to) ) {
        return;
    }
    vertices[from][to] = 0;
}

int Graph::VertexCount() const {
    return vertexNumber;
}

std::vector<int> Graph::MinCut() {
    int n = vertexNumber;
    int bestCost = 1e9;
    std::vector<int> bestCut;
    std::vector<std::vector<int>> squeezedVertices(n);
    for( int i = 0; i < n; ++i ) {
        squeezedVertices[i].push_back(i);
    }
    std::vector<int> weight(n);
    std::vector<bool> exist(n, true);
    std::vector<bool> inA(n);
    for( int j = 0; j < n - 1; ++j ) {
        inA.assign(n, false);
        weight.assign(n, 0);
        for( int it = 0, prev = 0; it < n - j; ++it ) {
            int last = -1;
            for( int i = 0; i < n; ++i ) {
                if( exist[i] && !inA[i] && (last == -1 || weight[i] > weight[last]) ) {
                    last = i;
                }
            }
            if( it == n - j - 1 ) {
                if( weight[last] < bestCost ) {
                    bestCost = weight[last];
                    bestCut = squeezedVertices[last];
                }
                squeezedVertices[prev].insert(squeezedVertices[prev].end(), squeezedVertices[last].begin(), squeezedVertices[last].end());
                for( int i = 0; i < n; ++i ) {
                    vertices[i][prev] += vertices[last][i];
                    vertices[prev][i] = vertices[i][prev];
                }
                exist[last] = false;
            } else {
                inA[last] = true;
                for( int i = 0; i < n; ++i ) {
                    weight[i] += vertices[i][last];
                }
                prev = last;
            }
        }
    }
    return bestCut;
}

int main() {
    int n;
    std::cin >> n;
    Graph graph(n);
    for( int i = 0; i < n; ++i ) {
        std::string line;
        std::cin >> line;
        for( int j = 0; j < n; ++j ) {
            if( line[j] == '1' ) {
                graph.AddEdge(i, j);
            }
        }
    }
    std::vector<int> vertices(n);
    for( int i = 0; i < n; ++i ) {
        vertices[i] = i;
    }
    auto left = graph.MinCut();
    for( size_t i = 0; i < left.size(); ++i ) {
        std::cout << left[i] + 1 << " ";
        vertices[left[i]] = -1;
    }
    std::cout << '\n';
    for( int i = 0; i < n; ++i ) {
        if( vertices[i] != -1 ) {
            std::cout << vertices[i] + 1 << " ";
        }
    }
    return 0;
}
