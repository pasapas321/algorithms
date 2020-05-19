#include <iostream>
#include <vector>



class Graph
{
private:
    int vertexNumber;
    std::vector<std::vector<int>> vertices;
public:
    explicit Graph(int n = 0) : vertexNumber(n) {vertices.resize(n);}
    void AddVertex();
    void AddEdge(int from, int to);
    bool HasEdge(int from, int to) const;
    const std::vector<int>& GetNextVertices(int from) const;
    int VertexCount() const;
};

void Graph::AddVertex() {
    vertexNumber++;
    vertices.push_back({});
}

void Graph::AddEdge(int from, int to) {
    vertices[from].push_back(to);
}

bool Graph::HasEdge(int from, int to) const {
    for( size_t i = 0; i < vertices[from].size(); ++i ) {
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

Graph BuildGraph(int n, int m, int firstLobeVertexNumber,
                 int& edgesNumber, std::vector<std::vector<int>>& stars) {
    Graph graph(firstLobeVertexNumber);
    for( int i = 0; i < n; ++i ) {
        for( int j = 0; j < m; ++j ) {
            if( (i - j) % 2 == 0 ) {
                if( i - 1 >= 0 && stars[i][j] != -1 && stars[i - 1][j] != -1 ) {
                    graph.AddEdge(stars[i][j], stars[i - 1][j] );
                    edgesNumber++;
                }
                if( j + 1 < m && stars[i][j] != -1 && stars[i][j + 1] != -1 ) {
                    graph.AddEdge(stars[i][j], stars[i][j + 1] );
                    edgesNumber++;
                }
                if( i + 1 < n && stars[i][j] != -1 && stars[i + 1][j] != -1 ) {
                    graph.AddEdge(stars[i][j], stars[i + 1][j] );
                    edgesNumber++;
                }
                if( j - 1 >= 0 && stars[i][j] != -1 && stars[i][j - 1] != -1 ) {
                    graph.AddEdge(stars[i][j], stars[i][j - 1] );
                    edgesNumber++;
                }
            }
        }
    }
    return graph;
}

bool Kuhn(int vertex, const Graph& graph, std::vector<int>& matching, std::vector<bool>& used) {
    if( used[vertex] ) {
        return false;
    }
    used[vertex] = true;
    auto adjacencyList = graph.GetNextVertices(vertex);
    for( size_t i = 0; i < adjacencyList.size(); ++i ) {
        int to = adjacencyList[i];
        if( matching[to] == -1 || Kuhn(matching[to], graph, matching, used) ) {
            matching[to] = vertex;
            return true;
        }
    }
    return false;
}

int main() {
    int n;
    int m;
    int a;
    int b;
    std::cin >> n >> m >> a >> b;
    int answer = 0;
    int starsAmount = 0;
    if( a < 2 * b ) {
        int firstLobeVertexNumber = 0;
        int secondLobeVertexNumber = 0;
        std::vector<std::vector<int>> stars(n);
        for( int i = 0; i < n; ++i ) {
            for( int j = 0; j < m; ++j ) {
                char symbol;
                std::cin >> symbol;
                if( symbol == '.' ) {
                    stars[i].push_back(-1);
                } else {
                    if( (i - j) % 2 == 0 ) {
                        stars[i].push_back(firstLobeVertexNumber);
                        firstLobeVertexNumber++;
                    } else {
                        stars[i].push_back(secondLobeVertexNumber);
                        secondLobeVertexNumber++;
                    }
                }
            }
        }
        int matchingSize = 0;
        if( firstLobeVertexNumber != 0 && secondLobeVertexNumber != 0 ) {
            int edgesNumber = 0;
            Graph graph = BuildGraph(n, m, firstLobeVertexNumber, edgesNumber, stars);
            std::vector<int> matching;
            std::vector<bool> used;
            if( edgesNumber != 0 ) {
                matching.assign(secondLobeVertexNumber, -1);
                for( int i = 0; i < firstLobeVertexNumber; ++i ) {
                    used.assign(firstLobeVertexNumber, false);
                    Kuhn(i, graph, matching, used);
                }
                for( int i = 0; i < secondLobeVertexNumber; ++i ) {
                    if( matching[i] != -1 ) {
                        matchingSize++;
                    }
                }
            }
        }
        starsAmount = firstLobeVertexNumber + secondLobeVertexNumber;
        answer = matchingSize * a + (starsAmount - matchingSize * 2) * b;
    } else {
        for( int i = 0; i < n; ++i ) {
            for( int j = 0; j < m; ++j ) {
                char symbol;
                std::cin >> symbol;
                if( symbol == '*' ) {
                    starsAmount++;
                }
            }
        }
        answer = starsAmount * b;
    }
    std::cout << answer;
    return 0;
}
