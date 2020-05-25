#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>



class SegmentTree {
private:
    std::vector<std::pair<int, int>> tree;
    const size_t leafsAmount;
public:
    SegmentTree(const std::vector<int>& sequence);
    int GetSegmentMax(size_t root, size_t left, size_t right, size_t a, size_t b);
    void IncreaseSegment(size_t root, size_t left, size_t right, size_t a, size_t b, int value);
    const size_t GetLeafsAmount();
};

SegmentTree::SegmentTree(const std::vector<int>& sequence) : leafsAmount(1 << static_cast<size_t>(ceil(log2(sequence.size())))) {
    tree.assign(2 * leafsAmount - 1, {0, 0});
    for( size_t i = 0; i < sequence.size(); ++i ) {
        tree[tree.size() / 2 + i].first = sequence[i];
    }
    for( int i = static_cast<int>(tree.size()) / 2 - 1; i >= 0; --i ) {
        tree[i].first = std::max(tree[2 * i + 1].first, tree[2 * i + 2].first);
    }
}

int SegmentTree::GetSegmentMax(size_t root, size_t left, size_t right, size_t a, size_t b) {
    if( a > b || left > right ) {
        return 0;
    }
    if( a == left && b == right ) {
        return tree[root].first + tree[root].second;
    }
    size_t median = (left + right) / 2;
    int leftMax = GetSegmentMax(2 * root + 1, left, median, a, std::min(b, median));
    int rightMax = GetSegmentMax(2 * root + 2, median + 1, right, std::max(a, median + 1), b);
    return std::max(rightMax, leftMax) + tree[root].second;
}

void SegmentTree::IncreaseSegment(size_t root, size_t left, size_t right, size_t a, size_t b, int value) {
    if( a > b || left > right ) {
        return;
    }
    if( a == left && b == right ) {
        tree[root].second += value;
        return;
    }
    size_t median = (left + right) / 2;
    IncreaseSegment(2 * root + 1, left, median, a, std::min(b, median), value);
    IncreaseSegment(2 * root + 2, median + 1, right, std::max(a, median + 1), b, value);
    tree[root].first = std::max(tree[2 * root + 1].first + tree[2 * root + 1].second, tree[2 * root + 2].first + tree[2 * root + 2].second);
}

const size_t SegmentTree::GetLeafsAmount() {
    return leafsAmount;
}

int main() {
    size_t N;
    std::cin >> N;
    std::vector<int> tickets(N - 1);
    for( size_t i = 0; i < N - 1; ++i ) {
        std::cin >> tickets[i];
    }
    SegmentTree segmentTree(tickets);
    int capacity;
    std::cin >> capacity;
    size_t inquiry;
    std::cin >> inquiry;
    for( size_t i = 0; i < inquiry; ++i ) {
        size_t start;
        size_t finish;
        int amount;
        std::cin >> start >> finish >> amount;
        size_t leafs = segmentTree.GetLeafsAmount();
        if( segmentTree.GetSegmentMax(0, 0, leafs - 1, start, finish - 1) + amount > capacity) {
            std::cout << i << " ";
        } else {
            segmentTree.IncreaseSegment(0, 0, leafs - 1, start, finish - 1, amount);
        }
    }
    return 0;
}
