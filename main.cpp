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

void FirstDFS(const Graph& graph, int from, std::vector<int>& color, std::vector<int>& order) {
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
        }
        if( flag == 0 ) {
            color[vertex] = 2;
            order.push_back(vertex);
            st.pop();
        }
    }
}

void SecondDFS(const Graph& transposedGraph, int from, std::vector<int>& color, std::vector<int>& component) {
    std::stack<int> st;
    st.push(from);
    while( !st.empty() ) {
        int vertex = st.top();
        color[vertex] = 1;
        component.push_back(vertex);
        const std::vector<int> vert = transposedGraph.GetNextVertices(vertex);
        bool flag = 0;
        for( int i = 0; i < vert.size() && flag == 0; ++i ) {
            if( !color[vert[i]] ) {
                st.push(vert[i]);
                flag = 1;
            }
        }
        if( flag == 0 ) {
            color[vertex] = 2;
            st.pop();
        }
    }
}

Graph BuildCondensation(const Graph& graph, const Graph& transposedGraph, int& amount) {
    int n = graph.VertexCount();
    std::vector<int> color(n, 0);
    std::vector<int> component;
    std::vector<int> order;
    std::vector<int> newVertices(n);
    
    for( int i = 0; i < n; ++i ) {
        if( !color[i] ) {
            FirstDFS(graph, i, color, order);
        }
    }
    color.assign(n, 0);
    for( int i = 0; i < n; ++i ) {
        int vertex = order[n - 1 - i];
        if( !color[vertex] ) {
            SecondDFS(transposedGraph, vertex, color, component);
            for( int j = 0; j < component.size(); ++j ) {
                newVertices[component[j]] = amount;
            }
            amount++;
            component.clear();
        }
    }
    
    Graph newGraph(amount);
    for( int i = 0; i < n; ++i ) {
        const std::vector<int> vert = graph.GetNextVertices(i);
        for( int j = 0; j < vert.size(); ++j ) {
            if( newVertices[i] != newVertices[vert[j]] ) {
                newGraph.AddEdge(newVertices[i], newVertices[vert[j]]);
            }
        }
    }
    return newGraph;
}

int FindSources(const Graph& newGraph) {
    int sourcesAmount = 0;
    int n = newGraph.VertexCount();
    std::vector<bool> check(n, 0);
    for( int i = 0; i < n; ++i ) {
        const std::vector<int> vert = newGraph.GetNextVertices(i);
        for( int j = 0; j < vert.size(); ++j ) {
            check[vert[j]] = 1;
        }
    }
    for( int i = 0; i < n; ++i ) {
        if( !check[i] ) {
            sourcesAmount++;
        }
    }
    return sourcesAmount;
}

int FindEffluents(const Graph& newGraph) {
    int effluentsAmount = 0;
    int n = newGraph.VertexCount();
    for( int i = 0; i < n; ++i ) {
        if( newGraph.GetNextVertices(i).empty() ) {
            effluentsAmount++;
        }
    }
    return effluentsAmount;
}

int main() {
    int n;
    int m;
    std::cin >> n >> m;
    Graph graph(n);
    Graph transposedGraph(n);
    for( int i = 0; i < m; ++i ) {
        int from;
        int to;
        std::cin >> from >> to;
        graph.AddEdge(from - 1, to - 1);
        transposedGraph.AddEdge(to - 1, from - 1);
    }
    int amount = 0;
    Graph newGraph = BuildCondensation(graph, transposedGraph, amount);
    if( amount == 1 ) {
        std::cout << 0;
    } else {
        int answer = std::max(FindSources(newGraph), FindEffluents(newGraph));
        std::cout << answer;
    }
    return 0;
}

