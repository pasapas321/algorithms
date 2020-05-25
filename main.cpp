#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>



class SegmentTree {
private:
    std::vector<int> tree;
    const size_t leafsAmount;
    const int infinity = 1e9;
    void Propagate(int vertex, size_t treeLeft, size_t treeRight);
    void Build(const std::vector<int>& sequence, int vertex, size_t treeLeft, size_t treeRight);
    std::vector<int> minValue;
    std::vector<int> newValue;
public:
    SegmentTree(const std::vector<int>& sequence);
    int GetSegmentMin(size_t left, size_t right, int vertex, size_t treeLeft, size_t treeRight);
    void Update(size_t left, size_t right, int value, int vertex, size_t treeLeft, size_t treeRight);
};

void SegmentTree::Build(const std::vector<int> &sequence, int vertex, size_t treeLeft, size_t treeRight) {
    if( treeRight - treeLeft == 1 ) {
        tree[vertex] = sequence[treeLeft];
    } else {
        size_t treeMed = (treeLeft + treeRight) / 2;
        Build(sequence, 2 * vertex + 1, treeLeft, treeMed);
        Build(sequence, 2 * vertex + 2, treeMed, treeRight);
        tree[vertex] = std::min(tree[2 * vertex + 1], tree[2 * vertex + 2]);
    }
    minValue[vertex] = tree[vertex];
}

SegmentTree::SegmentTree(const std::vector<int>& sequence) : leafsAmount(1 << static_cast<size_t>(ceil(log2(sequence.size())))) {
    tree.assign(2 * leafsAmount - 1, infinity);
    minValue.assign(tree.size(), infinity);
    Build(sequence, 0, 0, sequence.size());
    newValue.assign(tree.size(), infinity);
}

void SegmentTree::Propagate(int vertex, size_t treeLeft, size_t treeRight) {
    if( treeRight - treeLeft == 1 ) {
        return;
    }
    if( newValue[vertex] == infinity ) {
        return;
    }
    newValue[2 * vertex + 1] = newValue[vertex];
    minValue[2 * vertex + 1] = newValue[vertex];
    newValue[2 * vertex + 2] = newValue[vertex];
    minValue[2 * vertex + 2] = newValue[vertex];
    newValue[vertex] = infinity;
}

int SegmentTree::GetSegmentMin(size_t left, size_t right, int vertex, size_t treeLeft, size_t treeRight) {
    Propagate(vertex, treeLeft, treeRight);
    if( left >= treeRight || treeLeft >= right ) {
        return infinity;
    }
    if( treeLeft >= left && treeRight <= right ) {
        return minValue[vertex];
    }
    size_t treeMed = (treeLeft + treeRight) / 2;
    int leftRes = GetSegmentMin(left, right, 2 * vertex + 1, treeLeft, treeMed);
    int rightRes = GetSegmentMin(left, right, 2 * vertex + 2, treeMed, treeRight);
    return std::min(leftRes, rightRes);
}

void SegmentTree::Update(size_t left, size_t right, int value, int vertex, size_t treeLeft, size_t treeRight) {
    Propagate(vertex, treeLeft, treeRight);
    if( left >= treeRight || treeLeft >= right ) {
        return;
    }
    if( treeLeft >= left && treeRight <= right ) {
        newValue[vertex] = value;
        minValue[vertex] = value;
        return;
    }
    size_t treeMed = (treeLeft + treeRight) / 2;
    Update(left, right, value, 2 * vertex + 1, treeLeft, treeMed);
    Update(left, right, value, 2 * vertex + 2, treeMed, treeRight);
    minValue[vertex] = std::min(minValue[2 * vertex + 1], minValue[2 * vertex + 2]);
}

int main() {
    size_t N;
    std::cin >> N;
    std::vector<int> colors;
    for( size_t i = 0; i < N; ++i ) {
        int R;
        int G;
        int B;
        std::cin >> R >> G >> B;
        colors.push_back(R + G + B);
    }
    SegmentTree segmentTree(colors);
    size_t K;
    std::cin >> K;
    for( size_t i = 0; i < K; ++i ) {
        size_t C;
        size_t D;
        int R;
        int G;
        int B;
        size_t E;
        size_t F;
        std::cin >> C >> D >> R >> G >> B >> E >> F;
        segmentTree.Update(C, D + 1, R + G + B, 0, 0, N);
        std::cout << segmentTree.GetSegmentMin(E, F + 1, 0, 0, N) << " ";
    }
    return 0;
}

