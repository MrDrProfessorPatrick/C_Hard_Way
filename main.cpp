#include <iostream>
#include <string>

#define MODE_CNT 4u

int leaves = 0;

typedef enum {
    kNoChild,
    kLeftChild,
    kRightChild,
    kBothChildren,
} ChildMode;

ChildMode modes[MODE_CNT] = {
    kNoChild,
    kLeftChild,
    kRightChild,
    kBothChildren,
};  

struct Node {
    int x;
    int y;
    Node *left;
    Node *right;
    
    Node(int t_x, int t_y) : x(t_x), y(t_y), left(nullptr), right(nullptr) {}

    int SumCords() { return x + y; };
};

static ChildMode GetChildMode(Node *node, int d);
void GenerateTree(Node *node, int d);
void PrintTree(const std::string &prefix, const Node *node, bool isLeft);
void InorderTraversal(Node *root);
void DeleteTree(Node *root);

int main() {
    Node *node = new Node(1, 1);

    GenerateTree(node, 3);
    InorderTraversal(node);
    PrintTree("", node, false);

    std::cout << std::endl;
    std::cout << "Total leaves: " << leaves << std::endl;
    std::cout << "Tree exists: " << node->left << std::endl;

    DeleteTree(node);

    std::cout << "Tree deleted: " << node->left << std::endl;

    return 0;
}

void GenerateTree(Node *node, int d) {
    if ((d + d - node->SumCords()) == 1) {
        return;
    }
        
    ChildMode m = GetChildMode(node, d);

    if (m == kBothChildren) {
        Node *lc = new Node(node->x, node->y + 1);
        Node *rc = new Node(node->x + 1, node->y);

        node->left = lc;
        node->right = rc;

        GenerateTree(node->left, d);
        GenerateTree(node->right, d);
    } else if (m == kLeftChild) {
        Node *lc = new Node(node->x, node->y + 1);

        node->left = lc;

        GenerateTree(node->left, d);
    } else if (m == kRightChild) {
        Node *rc = new Node(node->x + 1,node->y);

        node->right = rc;

        GenerateTree(node->right, d);
    }
}

static ChildMode GetChildMode(Node *node, int d) {
    short int m = kNoChild;

    if ((d - node->x) > 0) {
        m |= kRightChild;
    }

    if ((d - node->y) > 0) {
        m |= kLeftChild;
    }
        
    return modes[m];
}

void PrintTree(const std::string &prefix, const Node *node, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──");

        // print the value of the node
        std::cout << node->x << node->y << std::endl;

        // enter the next tree level - left and right branch
        PrintTree(prefix + (isLeft ? "│   " : "    "), node->left, true);
        PrintTree(prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

void InorderTraversal(Node *node) {

    if (node == nullptr) {
        return;
    }

    if (node->left == nullptr && node->right == nullptr) {
        leaves++;
    }

    InorderTraversal(node->left);

    // std::cout << node->x << node->y << std::endl;

    InorderTraversal(node->right);
}

void DeleteTree(Node *node) {
    if (node == nullptr) {
        return;
    }

    DeleteTree(node->left);
    DeleteTree(node->right);

    // std::cout << node->x << node->y << std::endl;  
    delete node;
}