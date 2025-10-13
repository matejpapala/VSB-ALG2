#include <iostream>
#include <vector>

struct Node
{
    Node *right;
    Node *left;
    int key;
    int height;
    Node(int value) : key(value), left(nullptr), right(nullptr), height(0) {}
};

class BinaryTree
{
private:
    Node *root;
    void clear(Node *node)
    {
        if (node == nullptr)
        {
            return;
        }
        else
        {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    bool containsKey(Node *node, const int key)
    {
        if (node == nullptr)
        {
            return false;
        }
        if (node->key == key)
        {
            return true;
        }
        if (key < node->key)
        {
            return containsKey(node->left, key);
        }
        else
        {
            return containsKey(node->right, key);
        }
    }

    Node *insert(Node *node, const int key)
    {
        if (node == nullptr)
        {
            return new Node(key);
        }
        if (key < node->key)
        {
            node->left = insert(node->left, key);
        }
        else if (key > node->key)
        {
            node->right = insert(node->right, key);
        }
        else
        {
            return node;
        }
        return balance(node);
    }

    Node *erase(Node *node, const int key)
    {
        if (node == nullptr)
        {
            return node;
        }
        if (key < node->key)
        {
            node->left = erase(node->left, key);
        }
        else if (key > node->key)
        {
            node->right = erase(node->right, key);
        }
        else
        {
            if (node->left == nullptr || node->right == nullptr)
            {
                if (node->left == nullptr and node->right == nullptr)
                {
                    delete node;
                    return nullptr;
                }
                else
                {
                    Node *temp;
                    if (node->left == nullptr)
                    {
                        temp = node->right;
                    }
                    else
                    {
                        temp = node->left;
                    }
                    delete node;
                    return temp;
                }
            } else {
                Node* successor = findMinKey(node->right);
                node->key = successor->key;
                node->right = erase(node->right, successor->key);
            }
        }
        return node;
    }

    Node* findMinKey(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    int height(Node* node) {
        if (node == nullptr) {
            return -1;
        }
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
        updateHeight(node);
        int bf = balanceFactor(node);
        if(bf > 1) {
            if(balanceFactor(node->left) < 0) {
                //LR rotace
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        if(bf < -1) {
            if(balanceFactor(node->right) > 0) {
                //RL rotace
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }
    }

public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree()
    {
        clear(root);
    }
    bool containsKey(const int key)
    {
        return containsKey(root, key);
    }
    void insert(const int key)
    {
        root = insert(root, key);
    }
    void erase(const int key)
    {
        root = erase(root, key);
    }
    void prerOrderPrint(Node* node) {
        if (node == nullptr) {
            return;
        }
        std::cout << node->key << " ";
        prerOrderPrint(node->left);
        prerOrderPrint(node->right);
    }

};

int main()
{
    return 0;
}