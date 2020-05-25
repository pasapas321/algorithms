#include <iostream>
#include <iomanip>
#include <stack>



class ImplicitTreap {
private:
    struct Node {
        Node(const std::string& value);
        void Fix();
        size_t treeSize;
        int priority;
        std::string value;
        Node* left;
        Node* right;
    };
    Node* root;
    std::pair<Node*, Node*> Split(Node* node, size_t position);
    Node* Merge(Node* firstNode, Node* secondNode);
    void Clear(Node* node);
public:
    ImplicitTreap();
    ~ImplicitTreap();
    void InsertAt(size_t position, const std::string& value);
    void DeleteAt(size_t position);
    std::string GetAt(size_t position) const;
    void Clear();
};

ImplicitTreap::ImplicitTreap() : root(nullptr) {}

void ImplicitTreap::Clear(Node* node) {
    if( node == nullptr ) {
        return;
    }
    Clear(node->left);
    Clear(node->right);
    delete node;
}

void ImplicitTreap::Clear()
{
    Clear(root);
    root = nullptr;
}

ImplicitTreap::~ImplicitTreap()
{
    Clear();
}

ImplicitTreap::Node::Node(const std::string& value)
                        : value(value)
                        , priority(rand())
                        , left(nullptr)
                        , right(nullptr)
                        , treeSize(1)
{}

void ImplicitTreap::Node::Fix() {
    treeSize = 1;
    if( left != nullptr ) {
        treeSize += left->treeSize;
    }
    if( right != nullptr ) {
        treeSize += right->treeSize;
    }
}

std::pair<ImplicitTreap::Node*, ImplicitTreap::Node*> ImplicitTreap::Split(Node* node, size_t position) {
    if( node == nullptr ) {
        return {nullptr, nullptr};
    }
    size_t size = (node->left != nullptr) ? (node->left->treeSize) : 0;
    if( size >= position ) {
        auto resultTree = Split(node->left, position);
        node->left = resultTree.second;
        node->Fix();
        return {resultTree.first, node};
    } else {
        auto resultTree = Split(node->right, position - size - 1);
        node->right = resultTree.first;
        node->Fix();
        return {node, resultTree.second};
    }
}

ImplicitTreap::Node* ImplicitTreap::Merge(Node* firstNode, Node* secondNode) {
    if( secondNode == nullptr ) {
        return firstNode;
    }
    if( firstNode == nullptr ) {
        return secondNode;
    }
    if( firstNode->priority > secondNode->priority ) {
        firstNode->right = Merge(firstNode->right, secondNode);
        firstNode->Fix();
        return firstNode;
    } else {
        secondNode->left = Merge(firstNode, secondNode->left);
        secondNode->Fix();
        return secondNode;
    }
}

void ImplicitTreap::InsertAt(size_t position, const std::string& value) {
    Node* current = new Node(value);
    auto resultTree = Split(root, position);
    Node* right = Merge(current, resultTree.second);
    root = Merge(resultTree.first, right);
}

void ImplicitTreap::DeleteAt(size_t position) {
    auto firstTree = Split(root, position);
    auto secondTree = Split(firstTree.second, 1);
    delete secondTree.first;
    root = Merge(firstTree.first, secondTree.second);
}

std::string ImplicitTreap::GetAt(size_t position) const {
    size_t size;
    Node* node = root;
    do {
        size = (node->left != nullptr) ? (node->left->treeSize) : 0;
        if( size == position ) {
            break;
        }
        if( position < size ) {
            node = node->left;
        } else {
            position -= size + 1;
            node = node->right;
        }
    } while( true );
    return node->value;
}

int main() {
    size_t k;
    std::cin >> k;
    ImplicitTreap stringArray;
    for( size_t i = 0; i < k; ++i ) {
        char command;
        std::cin >> command;
        if( command == '+' ) {
            size_t position;
            std::string value;
            std::cin >> position >> value;
            stringArray.InsertAt(position, value);
        } else if( command == '?' ) {
            size_t position;
            std::cin >> position;
            std::cout << stringArray.GetAt(position) << '\n';
        } else {
            size_t firstPosition;
            size_t secondPosition;
            std::cin >> firstPosition >> secondPosition;
            for( size_t j = firstPosition; j < secondPosition + 1; ++j ) {
                stringArray.DeleteAt(j);
            }
        }
    }
    return 0;
}


