#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>


std::vector<int> readIntegersFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<int> numbersVec;
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return numbersVec;
    }
    std::string line;
    if (std::getline(file, line)) {
        std::stringstream lineAsStream(line);
        int num;
        while (lineAsStream >> num) {
            numbersVec.push_back(num);
        }
    }
    file.close();
    return numbersVec;
}

struct Node {
    Node *right;
    Node *left;
    int key;
    int height;
    Node(int value) : key(value), left(nullptr), right(nullptr), height(0) {}
};

class BinaryTree {
private:
    Node *root;
    void clear(Node *node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
    bool containsKey(Node *node, const int key) {
        if (node == nullptr) return false;
        if (node->key == key) return true;
        if (key < node->key) return containsKey(node->left, key);
        return containsKey(node->right, key);
    }
    Node *insert(Node *node, const int key) {
        if (node == nullptr) return new Node(key);
        if (key < node->key) node->left = insert(node->left, key);
        else if (key > node->key) node->right = insert(node->right, key);
        else return node;
        return balance(node);
    }
    Node *erase(Node *node, const int key) {
        if (node == nullptr) return node;
        if (key < node->key) {
            node->left = erase(node->left, key);
        } else if (key > node->key) {
            node->right = erase(node->right, key);
        } else {
            if (node->left == nullptr || node->right == nullptr) {
                if (node->left == nullptr && node->right == nullptr) {
                    delete node;
                    return nullptr;
                } else {
                    Node *temp = (node->left == nullptr) ? node->right : node->left;
                    delete node;
                    return temp;
                }
            } else {
                Node* successor = findMinKey(node->right);
                node->key = successor->key;
                node->right = erase(node->right, successor->key);
            }
        }
        return balance(node);
    }
    Node* findMinKey(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }
    int height(Node* node) {
        if (node == nullptr) return -1;
        return node->height;
    }
    int balanceFactor(Node* node) {
        return height(node->left) - height(node->right);
    }
    void updateHeight(Node* node) {
        if (node != nullptr) {
            node->height = 1 + std::max(height(node->left), height(node->right));
        }
    }
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }
    Node* balance(Node* node) {
        if (node == nullptr) return node;
        updateHeight(node);
        int bf = balanceFactor(node);
        if (bf > 1) {
            if (balanceFactor(node->left) < 0) node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (bf < -1) {
            if (balanceFactor(node->right) > 0) node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }
    void inorder(Node* node) const {
        if (!node) return;
        inorder(node->left);
        std::cout << node->key << ' ';
        inorder(node->right);
    }
    void preorder(Node* node) const {
        if (!node) return;
        std::cout << node->key << ' ';
        preorder(node->left);
        preorder(node->right);
    }
    void postorder(Node* node) const {
        if (!node) return;
        postorder(node->left);
        postorder(node->right);
        std::cout << node->key << ' ';
    }
public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree() { clear(root); }
    bool containsKey(const int key) { return containsKey(root, key); }
    void insert(const int key) { root = insert(root, key); }
    void erase(const int key) { root = erase(root, key); }
    void print_postorder() const { postorder(root); std::cout << '\n'; }
    void print_preorder() const  { preorder(root); std::cout << '\n'; }
    void print_inorder() const   { inorder(root);  std::cout << '\n'; }
    void prerOrderPrint(Node* /*node*/) {}
};

int main(int argc, char* argv[]) {
    if (argc < 3) return 0;
    const std::string buildFile = argv[1];
    const std::string deleteFile = argv[2];
    BinaryTree bt;
    for (int v : readIntegersFromFile(buildFile)) bt.insert(v);
    for (int d : readIntegersFromFile(deleteFile)) bt.erase(d);
    bt.print_postorder();
    bt.print_preorder();
    bt.print_inorder();
    return 0;
}
