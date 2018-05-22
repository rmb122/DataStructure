#include <iostream>
#include <vector>
#include <string>

class treeNode {
    friend class tree;

    treeNode* left;
    treeNode* right;
    treeNode* parent;
    int value;
    bool isLthread;
    bool isRthread;

    treeNode(treeNode* left, treeNode* right, int value) : left(left), right(right), value(value), isLthread(false), isRthread(false) {};
    treeNode(treeNode* left, treeNode* right, treeNode* parent, int value) : left(left), right(right), parent(parent), value(value), isLthread(false), isRthread(false) {};
    treeNode() : left(nullptr), right(nullptr), value(0), isLthread(false), isRthread(false) {};
};

class tree {
    treeNode* root = nullptr;

    void delTree(treeNode* node) {
        if(node == nullptr) {
            return;
        }
        if(!node->isLthread) {
            delTree(node->left);
        }
        if(!node->isRthread) {
            delTree(node->right);
        }
        delete node;
    }

    void printExPre(treeNode* node) {
        std::cout << node->value << " ";
        if (node->left != nullptr) {
            printExPre(node->left);
        }
        if (node->right != nullptr) {
            printExPre(node->right);
        }
    }

    void printExMid(treeNode* node) {
        if (node->left != nullptr) {
            printExMid(node->left);
        }
        std::cout << node->value << " ";
        if (node->right != nullptr) {
            printExMid(node->right);
        }
    }

    void printExLast(treeNode* node) {
        if (node->left != nullptr) {
            printExLast(node->left);
        }
        if (node->right != nullptr) {
            printExLast(node->right);
        }
        std::cout << node->value << " ";
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

    void makeTreeLeadedExPre(treeNode* curr, treeNode* &pre) {
        if(curr == nullptr) {
            return;
        }

        if(curr->left == nullptr) {
            curr->isLthread = true;
            curr->left = pre;
        }

        if(pre != nullptr && pre->right == nullptr) {
            pre->isRthread = true;
            pre->right = curr;
        }

        pre = curr;

        if(!curr->isLthread) { //防止陷入死循环
            makeTreeLeadedExPre(curr->left, pre);
        }
        if(!curr->isRthread) {
            makeTreeLeadedExPre(curr->right, pre);
        }
    }


    void makeTreeLeadedExMid(treeNode* curr, treeNode* &pre) {
        if(curr == nullptr) {
            return;
        }

        if(!curr->isLthread) { //防止陷入死循环
            makeTreeLeadedExMid(curr->left, pre);
        }
        if(curr->left == nullptr) {
            curr->isLthread = true;
            curr->left = pre;
        }

        if(pre != nullptr && pre->right == nullptr) {
            pre->isRthread = true;
            pre->right = curr;
        }

        pre = curr;

        if(!curr->isRthread) {
            makeTreeLeadedExMid(curr->right, pre);
        }
    }

    void makeTreeLeadedExLast(treeNode* curr, treeNode* &pre) {
        if(curr == nullptr) {
            return;
        }

        if(!curr->isLthread) { //防止陷入死循环
            makeTreeLeadedExLast(curr->left, pre);
        }
        if(!curr->isRthread) {
            makeTreeLeadedExLast(curr->right, pre);
        }

        if(curr->left == nullptr) {
            curr->isLthread = true;
            curr->left = pre;
        }

        if(pre != nullptr && pre->right == nullptr) {
            pre->isRthread = true;
            pre->right = curr;
        }

        pre = curr;
    }

public:
    ~tree() {
        delTree(root);
    }

    void insert(int value) {
        if (root == nullptr) {
            root = new treeNode(nullptr, nullptr, nullptr, value);
        } else {
            treeNode* curr = root;
            while (true) {
                if (value >= curr->value) {
                    if (curr->right == nullptr) {
                        curr->right = new treeNode(nullptr, nullptr, curr, value);
                        return;
                    } else {
                        curr = curr->right;
                    }
                } else {
                    if (curr->left == nullptr) {
                        curr->left = new treeNode(nullptr, nullptr, curr, value);
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

    void printPre() {
        printExPre(root);
    }

    void printMid() {
        printExMid(root);
    }

    void printLast() {
        printExLast(root);
    }

    void makeTreeLeadedPre() {
        treeNode* temp = nullptr;
        makeTreeLeadedExPre(root, temp);
    }

    void makeTreeLeadedMid() {
        treeNode* temp = nullptr;
        makeTreeLeadedExMid(root, temp);
    }

    void makeTreeLeadedLast() {
        treeNode* temp = nullptr;
        makeTreeLeadedExLast(root, temp);
    }

    void printLeadedPre() {
        treeNode* curr = root;
        while(curr != nullptr) {
            std::cout << curr->value << " ";
            if(!curr->isLthread) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
    }

    void printLeadedMid() {
        treeNode* curr = root;
        if(curr == nullptr) {
            return;
        }

        while(!curr->isLthread && curr->left != nullptr) {
            curr = curr->left;
        }

        while(curr != nullptr) {
            if(curr->isRthread) {
                std::cout << curr->value << " ";
                curr = curr->right;
            } else {
                std::cout << curr->value << " ";
                curr = curr->right;
                while(curr != nullptr && !curr->isLthread) {
                    curr = curr->left;
                }
            }
        }
    }

    void printLeadedLast() {
        treeNode* curr = root;
        if(curr == nullptr) {
            return;
        }

        while(curr->left != nullptr) {
            curr = curr->left;
        }

        while(curr != nullptr) {
            std::cout << curr->value << " ";
            if(curr->isRthread) {
                curr = curr->right;
            } else {
                curr = curr->parent;
            }

        }
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

    /*
        test.printPre(); //10 5 4 -100 -21 15 14 12 17 20 21 12312312 1234
        std::cout << std::endl;

        test.makeTreeLeadedPre();
        test.printLeadedPre(); //10 5 4 -100 -21 15 14 12 17 20 21 12312312 1234
        std::cout << std::endl;
    */
    /*
        test.printMid(); //-100 -21 4 5 10 12 14 15 17 20 21 1234 12312312
        std::cout << std::endl;

        test.makeTreeLeadedMid();
        test.printLeadedMid(); //-100 -21 4 5 10 12 14 15 17 20 21 1234 12312312
        std::cout << std::endl;
    */
    test.printLast(); //-21 -100 4 5 12 14 1234 12312312 21 20 17 15 10
    std::cout << std::endl;

    test.makeTreeLeadedLast();
    test.printLeadedLast(); //-21 -100 4 5 12 14 1234 12312312 21 20 17 15 10
    std::cout << std::endl;
    return 0;
}

