#include <iostream>
#include <vector>
#include <string>

class treeNode {
    friend class tree;

    treeNode* left;
    treeNode* right;
    int value;

    treeNode(treeNode* left, treeNode* right, int value) : left(left), right(right), value(value) {};
    treeNode() : left(nullptr), right(nullptr), value(0) {};
};

class tree {
    treeNode* root = nullptr;

    void delTree(treeNode* node) {
        if(node == nullptr) {
            return;
        }
        delTree(node->left);
        delTree(node->right);
        delete node;
    }

    void printEx(treeNode* node) {
        if (node->left != nullptr) {
            printEx(node->left);
        }

        std::cout << node->value << " ";

        if (node->right != nullptr) {
            printEx(node->right);
        }
    }

    void getPath(std::vector<std::string> &paths, std::string currPath, treeNode* node) {
        if(node == nullptr) {
            return;
        }

        std::string data = std::to_string(node->value);
        data = "->" + data;
        currPath += data;

        getPath(paths, currPath, node->right);
        getPath(paths, currPath, node->left);

        if(node->right == nullptr && node->left == nullptr) {
            paths.push_back(currPath);
        }
    }

public:
    ~tree() {
        delTree(root);
    }

    void insert(int value) {
        if (root == nullptr) {
            root = new treeNode(nullptr, nullptr, value);
        } else {
            treeNode* curr = root;
            while (true) {
                if (value >= curr->value) {
                    if (curr->right == nullptr) {
                        curr->right = new treeNode(nullptr, nullptr, value);
                        return;
                    } else {
                        curr = curr->right;
                    }
                } else {
                    if (curr->left == nullptr) {
                        curr->left = new treeNode(nullptr, nullptr, value);
                        return;
                    } else {
                        curr = curr->left;
                    }
                }
            }
        }
    }

    std::vector<std::string> getPaths() {
        std::vector<std::string> paths;
        getPath(paths, "root", root);
        return paths;
    }

    void print() {
        printEx(root);
    }

};

int main() {
    tree test;
    test.insert(10);
    test.insert(5);
    test.insert(15);
    test.insert(14);
    test.insert(17);
    test.insert(4);
    test.insert(20);
    test.insert(-100);
    test.insert(-21);
    test.insert(12);
    test.insert(21);
    test.insert(12312312);
    test.insert(1234);

    auto paths = test.getPaths();
    for(int i = 0; i < paths.size(); i++) {
        std::cout << paths[i] << std::endl;
    }
    std::cout << std::endl;
    return 0;
}

