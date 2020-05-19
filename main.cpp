#include <iostream>
#include <vector>
#include <stack>


class Graph
{
private:
    int vertexNumber;
    std::vector<std::vector<int>> vertices;
public:
    explicit Graph(int n = 0) : vertexNumber(n) {vertices.resize(n);}
    bool IsValidVertex(int vertex) const;
    void AddVertex();
    void AddEdge(int from, int to);
    bool HasEdge(int from, int to) const;
    const std::vector<int>& GetNextVertices(int from) const;
    int VertexCount() const;
};

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
    vertices[from].push_back(to);
}

bool Graph::HasEdge(int from, int to) const {
    if( !IsValidVertex(from) || !IsValidVertex(to) ) {
        return false;
    }
    for( int i = 0; i < vertices[from].size(); ++i ) {
        if( vertices[from][i] == to ) {
            return true;
        }
    }
    return false;
}

const std::vector<int>& Graph::GetNextVertices(int from) const {
    return vertices[from];
}

int Graph::VertexCount() const {
    return vertexNumber;
}

enum Colors
{
    white,
    grey,
    black,
};

void DFS(const Graph& graph, int from, bool& hasCycle, std::vector<int>& answer, std::vector<Colors>& color) {
    std::stack<int> vertices;
    vertices.push(from);
    while( !vertices.empty() ) {
        int vertex = vertices.top();
        color[vertex] = grey;
        auto adjacentList = graph.GetNextVertices(vertex);
        bool wayExist = false;
        for( int i = 0; i < adjacentList.size() && !wayExist; ++i ) {
            if( color[adjacentList[i]] == white ) {
                vertices.push(adjacentList[i]);
                wayExist = true;
            }
            if( color[adjacentList[i]] == grey ) {
                hasCycle = true;
                return;
            }
        }
        if( !wayExist ) {
            color[vertex] = black;
            answer.push_back(vertex);
            vertices.pop();
        }
    }
}

std::vector<int> TopologicalSort(const Graph& graph, bool hasCycle) {
    int n = graph.VertexCount();
    std::vector<Colors> color(n, white);
    std::vector<int> answer;
    for( int i = 0; i < n; ++i ) {
        if( color[i] == white ) {
            DFS(graph, i, hasCycle, answer, color);
        }
    }
    for( int i = 0; 2 * i < n; ++i ) {
        std::swap(answer[i], answer[n - 1 - i]);
    }
    return answer;
}

int main() {
    int n;
    int m;
    std::cin >> n >> m;
    Graph graph(n);
    for( int i = 0; i < m; ++i ) {
        int from;
        int to;
        std::cin >> from >> to;
        if( from == to ) {
            std::cout << "NO";
            return 0;
        } else {
            graph.AddEdge(from, to);
        }
    }
    bool hasCycle = false;
    std::vector<Colors> color(n, white);
    std::vector<int> answer;
    for( int i = 0; i < n; ++i ) {
        DFS(graph, i, hasCycle, answer, color);
        if( hasCycle ) {
            std::cout << "NO";
            break;
        }
    }
    if( !hasCycle ) {
        auto vertSequence = TopologicalSort(graph, hasCycle);
        std::cout << "YES" << '\n';
        for( int i = 0; i < n; ++i ) {
            std::cout << vertSequence[i] << ' ';
        }
    }
    return 0;
}
