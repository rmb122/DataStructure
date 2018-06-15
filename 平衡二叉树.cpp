#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <queue>

class treeNode {
    friend class tree;

    treeNode* left;
    treeNode* right;
    treeNode* parent;
    int value;
    int height;

    treeNode(treeNode* left, treeNode* right, treeNode* parent, int value, int height) : left(left), right(right), parent(parent), value(value), height(height) {};
    treeNode() : left(nullptr), right(nullptr), parent(nullptr), value(0), height(0) {};
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

    inline int getHeight(treeNode* &node) {
        if(node == nullptr) {
            return 0;
        } else {
            return node->height;
        }
    }

    treeNode* leftRotate(treeNode* node) {
        treeNode* temp = node->right;
        node->right = temp->left;
        temp->parent = node->parent;
        node->parent = temp;
        temp->left = node;
        if(temp->parent != nullptr) {
            if(temp->parent->left == node) {
                temp->parent->left = temp;
            } else {
                temp->parent->right = temp;
            }
        }
        reCalcHeight(node);
        reCalcHeight(temp);
        return temp;
    }

    treeNode* rightRotate(treeNode * node) {
        treeNode* temp = node->left;
        node->left = temp->right;
        temp->parent = node->parent;
        node->parent = temp;
        temp->right = node;
        if(temp->parent != nullptr) { //判断是 RL 还是 LL
            if(temp->parent->left == node) {
                temp->parent->left = temp;
            } else {
                temp->parent->right = temp;
            }
        }
        reCalcHeight(node);
        reCalcHeight(temp);
        return temp;
    }

    void reCalcHeight(treeNode * node) {
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    treeNode* balance(treeNode * node) {
        if(getHeight(node->left) - getHeight(node->right) >= 2) {
            if(getHeight(node->left->left) > getHeight(node->left->right)) { //LL
                node = rightRotate(node);
            } else { //LR
                leftRotate(node->left);
                node = rightRotate(node);
            }
        } else {
            if(getHeight(node->right) - getHeight(node->left) >= 2) {
                if(getHeight(node->right->right) > getHeight(node->right->left)) { //RR
                    node = leftRotate(node);
                } else { //RL
                    rightRotate(node->right);
                    node = leftRotate(node);
                }
            }
        }
        return node;
    }

public:
    ~tree() {
        delTree(root);
    }

    int getDepth() {
        return _getDepth(root, 0);
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

    void insert(int value) {
        if(root == nullptr) {
            root = new treeNode(nullptr, nullptr, nullptr, value, 1);
        } else {
            treeNode* curr = root;
            while (true) {
                if (value >= curr->value) {
                    if (curr->right == nullptr) {
                        curr->right = new treeNode(nullptr, nullptr, curr, value, 1);
                        break;
                    } else {
                        curr = curr->right;
                    }
                } else {
                    if (curr->left == nullptr) {
                        curr->left = new treeNode(nullptr, nullptr, curr, value, 1);
                        break;
                    } else {
                        curr = curr->left;
                    }
                }
            }

            while(curr != nullptr) {
                reCalcHeight(curr);
                curr = balance(curr);
                curr = curr->parent; //不断平衡以及计算高度直至 root
            }
        }
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
    test.printTree();
    std::cout << std::endl;
    test.print();
    return 0;
}

