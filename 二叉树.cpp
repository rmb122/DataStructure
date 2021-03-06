#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>

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

    inline int max(int a, int b) {
        if(a > b) {
            return a;
        } else {
            return b;
        }
    }

    int _getDepth(treeNode* node, int currDepth) {
        if(node == nullptr) {
            return currDepth;
        } else {
            return max(_getDepth(node->left, currDepth + 1), _getDepth(node->right, currDepth + 1));
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

    int getDepth() {
        return _getDepth(root, 0);
    }

    std::vector<std::string> getPaths() {
        std::vector<std::string> paths;
        getPath(paths, "root", root);
        return paths;
    }

    void print() {
        printEx(root);
    }

    std::vector<int> transToArray(int depth) {
        std::vector<int> result;
        std::queue<treeNode*> temp;
        temp.push(root);
        result.reserve(static_cast<size_t>(pow(2, depth)));
        for(int i = 0; i < pow(2, depth); ++i) {
            treeNode* curr = temp.front();
            temp.pop();

            if(curr == nullptr) {
                result.push_back(INT32_MAX); //nullptr 保存为 INT32_MAX
                temp.push(nullptr);
                temp.push(nullptr);
            } else {
                result.push_back(curr->value);
                temp.push(curr->left);
                temp.push(curr->right);
            }
        }
        return result;
    }

    void printTree() { //支持最大 5 位数字
        int depth = getDepth();
        int currDepth = 0;
        std::vector<int> treeArray = transToArray(depth);

        auto getLength = [] (int depth) -> int {
            return 6.0 * pow(2, depth - 1) - 1;
        };

        auto putChars = [] (char chr, int count) -> void {
            for(int i = 0; i < count; ++i) {
                std::cout << chr;
            }
        };

        auto putNum = [] (int num) -> void {
            std::string nums = std::to_string(num);
            switch(nums.size()) { //非常暴力但有效的实现方法
            case 1:
                std::cout << "  " << nums << "  ";
                break;
            case 2:
                std::cout << " " << nums << "  ";
                break;
            case 3:
                std::cout << " " << nums << " ";
                break;
            case 4:
                std::cout << " " << nums;
                break;
            default:
                std::cout << nums;
            }
        };

        for(int i = 0 ; i < pow(2, depth) - 1; ++i) {
            if(i >= static_cast<int>(pow(2, currDepth)) - 1) {
                std::cout << std::endl; //结束上一行
                std::cout << currDepth;
                currDepth++;
                putChars(' ', getLength(depth - currDepth) - 2); //开始新一行的空
            }
            if(treeArray[i] != INT32_MAX) {
                putNum(treeArray[i]);
            } else {
                std::cout << "  x  ";
            }
            putChars(' ', (getLength(depth - currDepth) - 2) * 2 + 1);
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
    test.insert(12312);
    test.insert(1234);
    test.printTree();
    return 0;
}

