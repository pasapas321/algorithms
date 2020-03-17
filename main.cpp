#include <iostream>
#include <vector>
#include <stack>


class Graph
{
private:
    int vertexNumber;
    std::vector<std::vector<int>> vertices;
public:
    Graph(int n = 0) : vertexNumber(n) {vertices.resize(n);}
    bool IsValidVertex(int vertex) const;
    void AddVertex(int vertex);
    void AddEdge(int from, int to);
    bool HasEdge(int from, int to);
    const std::vector<int> GetNextVertices(int from) const;
    int VertexCount() const;
};

bool Graph::IsValidVertex(int vertex) const {
    return vertex >= 0 && vertex < vertexNumber;
}

void Graph::AddVertex(int vertex) {
    vertexNumber++;
    vertices.push_back({});
}

void Graph::AddEdge(int from, int to) {
    if( !IsValidVertex(from) || !IsValidVertex(to) ) {
        return;
    }
    vertices[from].push_back(to);
}

bool Graph::HasEdge(int from, int to) {
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

const std::vector<int> Graph::GetNextVertices(int from) const {
    return vertices[from];
}

int Graph::VertexCount() const {
    return vertexNumber;
}

void DFS(const Graph& graph, int from, bool& cycle, std::vector<int>& answer, std::vector<int>& color) {
    std::stack<int> st;
    st.push(from);
    while( !st.empty() ) {
        int vertex = st.top();
        color[vertex] = 1;
        const std::vector<int> vert = graph.GetNextVertices(vertex);
        bool flag = 0;
        for( int i = 0; i < vert.size() && flag == 0; ++i ) {
            if( !color[vert[i]] ) {
                st.push(vert[i]);
                flag = 1;
            }
            if( color[vert[i]] == 1 ) {
                cycle = 1;
                return;
            }
        }
        if( flag == 0 ) {
            color[vertex] = 2;
            answer.push_back(vertex);
            st.pop();
        }
    }
}

void TopologicalSort(const Graph& graph, std::vector<int>& answer, std::vector<int>& color, bool cycle) {
    int n = graph.VertexCount();
    for( int i = 0; i < n; ++i ) {
        color[i] = 0;
    }
    answer.clear();
    for( int i = 0; i < n; ++i ) {
        if( !color[i] ) {
            DFS(graph, i, cycle, answer, color);
        }
    }
    for( int i = 0; 2 * i < n; ++i ) {
        std::swap(answer[i], answer[n - 1 - i]);
    }
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
    bool cycle = 0;
    std::vector<int> color(n, 0);
    std::vector<int> answer;
    for( int i = 0; i < n; ++i ) {
        DFS(graph, i, cycle, answer, color);
        if( cycle ) {
            std::cout << "NO";
            break;
        }
    }
    if( !cycle ) {
        TopologicalSort(graph, answer, color, cycle);
        std::cout << "YES" << '\n';
        for( int i = 0; i < n; ++i ) {
            std::cout << answer[i] << ' ';
        }
    }
    return 0;
}

