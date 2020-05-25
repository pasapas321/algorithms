#include <iostream>
#include <vector>
#include <cmath>



class LCA {
private:
    void FillSparseTable(unsigned int node, unsigned int parent);
    bool isAncestor(unsigned int a, unsigned int b);
    unsigned int timer;
    size_t nodesNumber;
    unsigned int tableSize;
    std::vector<std::vector<unsigned int>>& tree;
    std::vector<unsigned int> timeIn;
    std::vector<unsigned int> timeOut;
    std::vector<std::vector<unsigned int>> ancestors;
public:
    LCA(std::vector<std::vector<unsigned int>>& parents);
    unsigned int ResponseToRequest(unsigned int a, unsigned int b);
    unsigned int ResponseToRequest(std::pair<unsigned int, unsigned int> query);
};

LCA::LCA(std::vector<std::vector<unsigned int>>& parents)
        : tree(parents)
        , nodesNumber(parents.size())
        , tableSize(static_cast<unsigned int>(ceil(log2(nodesNumber))))
        , timeIn(nodesNumber)
        , timeOut(nodesNumber)
        , ancestors(nodesNumber, std::vector<unsigned int>(tableSize + 1))
        , timer(0)
{
    FillSparseTable(0, 0);
}

unsigned int LCA::ResponseToRequest(unsigned int a, unsigned int b) {
    if( isAncestor(a, b) ) {
        return a;
    }
    if( isAncestor(b, a) ) {
        return b;
    }
    for( int i = tableSize; i >= 0; --i ) {
        if( !isAncestor(ancestors[a][i], b) ) {
            a = ancestors[a][i];
        }
    }
    return ancestors[a][0];
}

unsigned int LCA::ResponseToRequest(std::pair<unsigned int, unsigned int> query) {
    return ResponseToRequest(query.first, query.second);
}

void LCA::FillSparseTable(unsigned int node, unsigned int parent) {
    timeIn[node] = timer++;
    ancestors[node][0] = parent;
    for( int i = 1; i <= tableSize; ++i ) {
        ancestors[node][i] = ancestors[ancestors[node][i - 1]][i - 1];
    }
    for( size_t i = 0; i < tree[node].size(); ++i ) {
        unsigned int to = tree[node][i];
        if( to != parent ) {
            FillSparseTable(to, node);
        }
    }
    timeOut[node] = timer++;
}

bool LCA::isAncestor(unsigned int a, unsigned int b) {
    return timeIn[a] <= timeIn[b] && timeOut[b] <= timeOut[a];
}

class NextQuery {
private:
    unsigned int a;
    unsigned int b;
    unsigned long long x;
    unsigned long long y;
    unsigned long long z;
    unsigned int n;
public:
    NextQuery(unsigned int a, unsigned int b, unsigned long long x, unsigned long long y, unsigned long long z, unsigned int n);
    std::pair<unsigned int, unsigned int> ResponseToRequest(unsigned int result);
};

NextQuery::NextQuery(unsigned int a, unsigned int b, unsigned long long x, unsigned long long y, unsigned long long z, unsigned int n)
                    : a(a)
                    , b(b)
                    , x(x)
                    , y(y)
                    , z(z)
                    , n(n)
{}

std::pair<unsigned int, unsigned int> NextQuery::ResponseToRequest(unsigned int result) {
    std::pair<unsigned int, unsigned int> response((a + result) % n, b);
    a = (a * x + b * y + z) % n;
    b = (b * x + a * y + z) % n;
    return response;
}

int main() {
    unsigned int n;
    unsigned int m;
    std::cin >> n >> m;
    std::vector<std::vector<unsigned int>> parents(n);
    for( unsigned int i = 1; i < n; ++i ) {
        unsigned int x;
        std::cin >> x;
        parents[x].push_back(i);
    }
    unsigned int aFirst;
    unsigned int aSecond;
    std::cin >> aFirst >> aSecond;
    unsigned long long x;
    unsigned long long y;
    unsigned long long z;
    std::cin >> x >> y >> z;
    NextQuery nextQuery(aFirst, aSecond, x, y, z, n);
    LCA lca(parents);
    unsigned int answer = 0;
    unsigned int result = 0;
    for( unsigned int i = 0; i < m; ++i ) {
        auto response = nextQuery.ResponseToRequest(result);
        result = lca.ResponseToRequest(response);
        answer += result;
    }
    std::cout << answer;
    return 0;
}

