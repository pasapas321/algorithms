#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>



class R2Q {
private:
    std::vector<int> sequence;
    size_t length;
    std::vector<size_t> binaryLogarithm;
    std::vector<std::vector<std::pair<int, int>>> sparseTable;
    std::pair<int, int> GetMinPair(std::pair<int, int> a, std::pair<int, int> b);
public:
    R2Q(const std::vector<int>& sequence);
    int SecondStatistics(size_t a, size_t b);
};

std::pair<int, int> R2Q::GetMinPair(std::pair<int, int> a, std::pair<int, int> b) {
    std::pair<int, int> minPair;
    std::vector<int> order{a.first, a.second, b.first, b.second};
    std::sort(order.begin(), order.end());
    minPair.first = order[0];
    for( size_t i = 0; i < 3; ++i ) {
        if (order[i + 1] != order[0]) {
            minPair.second = order[i + 1];
            break;
        }
    }
    return minPair;
}

R2Q::R2Q(const std::vector<int>& sequence) : sequence(sequence), length(sequence.size()) {
    size_t n = ceil(log2(length));
    std::vector<std::pair<int, int>> tableString(length, {0, 0});
    sparseTable.assign(n, tableString);
    
    for( size_t i = 0; i < length + 1; ++i ) {
        binaryLogarithm.push_back(static_cast<size_t>(ceil(log2(i))));
    }
    
    for( size_t i = 0; i < length; ++i ) {
        sparseTable[0][i].first = sequence[i];
        sparseTable[0][i].second = sequence[i];
    }
    
    for( size_t i = 0; i < sparseTable.size() - 1; i++ ) {
        for( size_t j = 0; j < length - (1 << (i + 1)) + 1; ++j ) {
            std::pair<int, int> minPair = GetMinPair(sparseTable[i][j], sparseTable[i][j + (1 << i)]);
            sparseTable[i + 1][j].first = minPair.first;
            sparseTable[i + 1][j].second = minPair.second;
        }
    }
}

int R2Q::SecondStatistics(size_t a, size_t b) {
    size_t index = binaryLogarithm[b - a + 1] - 1;
    auto answer = GetMinPair(sparseTable[index][a], sparseTable[index][b - (1 << index) + 1]).second;
    return answer;
}

int main() {
    size_t N;
    size_t M;
    std::cin >> N >> M;
    std::vector<int> sequence(N);
    for( size_t i = 0; i < N; ++i ) {
        int element;
        std::cin >> element;
        sequence[i] = element;
    }
    R2Q r2q(sequence);
    for( size_t i = 0; i < M; ++i ) {
        size_t left;
        size_t right;
        std::cin >> left >> right;
        std::cout << r2q.SecondStatistics(left - 1, right - 1) << '\n';
    }
    return 0;
}
