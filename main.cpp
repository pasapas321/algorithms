#include <iostream>
#include <vector>
#include <queue>


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
    vertices[to].push_back(from);
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

std::vector<int> BFS(int vertex, const Graph& graph) {
    std::queue<int> vertices;
    vertices.push(vertex);
    int n = graph.VertexCount();
    std::vector<bool> used(n, false);
    std::vector<int> ways(n, 0);
    used[vertex] = true;
    while( !vertices.empty() ) {
        int from = vertices.front();
        vertices.pop();
        auto adjacent = graph.GetNextVertices(from);
        for( int i = 0; i < adjacent.size(); ++i ) {
            int to = adjacent[i];
            if( !used[to] ) {
                used[to] = true;
                vertices.push(to);
                ways[to] = ways[from] + 1;
            }
        }
    }
    return ways;
}

int FindMinDistance(int leon, int matilda, int milk, Graph graph) {
    std::vector<int> leonDistance = BFS(leon - 1, graph);
    std::vector<int> matildaDistance = BFS(matilda - 1, graph);
    std::vector<int> milkDistance = BFS(milk - 1, graph);
    int minDistance = 1e6;
    for( int i = 0; i < leonDistance.size(); ++i ) {
        int sum = leonDistance[i] + matildaDistance[i] + milkDistance[i];
        if( sum < minDistance ) {
            minDistance = sum;
        }
    }
    return minDistance;
}

int main()
{
    int n;
    int m;
    int leon;
    int matilda;
    int milk;
    std::cin >> n >> m >> leon >> matilda >> milk;
    Graph graph(n);
    for( int i = 0; i < m; ++i ) {
        int k;
        int l;
        std::cin >> k >> l;
        graph.AddEdge(k - 1, l - 1);
    }
    std::cout << FindMinDistance(leon, matilda, milk, graph);
    return 0;
}
