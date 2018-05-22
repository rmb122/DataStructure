#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>

class node {
    friend class huffmanTree;

    node* left;
    node* right;
    unsigned int weight;

    node(node* left, node* right, unsigned int weight) : left(left), right(right), weight(weight) {};
    node() : left(nullptr), right(nullptr), weight(0) {};
};

class huffmanTree {
private:
    node* root;

    unsigned int min(unsigned int first, unsigned int second, unsigned int third) {
        if(first < second) {
            if(first < third) {
                return 1;
            }
        } else {
            if(second < third) {
                return 2;
            }
        }
        return 3;
    }

    void _delTree(node* curr) {
        if(curr == nullptr) {
            return;
        }
        _delTree(curr->left);
        _delTree(curr->right);
        delete curr;
    }

    void _calcWPL(node* curr, unsigned int &WPL) {
        if(curr->left != nullptr) {
            WPL += curr->weight;
            _calcWPL(curr->left, WPL);
            _calcWPL(curr->right, WPL);
        } else {
            return;
        }
    }

    void _getEncode(node* curr, std::vector<std::pair<unsigned int, std::string>> &encodeData, std::string currPath) {
        if(curr->left != nullptr) {
            _getEncode(curr->left, encodeData, currPath + "0");
            _getEncode(curr->right, encodeData, currPath + "1");
        } else {
            encodeData.push_back(std::make_pair(curr->weight, currPath));
        }
    }

public:
    void delTree() {
        _delTree(root);
    }

    ~huffmanTree() {
        delTree();
    }

    void buildTree(std::vector<unsigned int> weights) {
        if(weights.size() == 0) {
            throw std::runtime_error("At least one value in weights.");
            return;
        }

        std::sort(weights.begin(), weights.end());
        std::list<node*> firNodes; //两个链表分别保存，使得两个链表的第一个节点的权重是当前链表中最小的
        std::list<node*> secNodes; //使得每次只要判断 3 种状况即可，时间复杂度 o(n)

        for(auto weight : weights) {
            firNodes.push_back(new node(nullptr, nullptr, weight));
        }

        while(firNodes.size() + secNodes.size() > 1) {
            unsigned int first = UINT32_MAX;  //因为要取最小值，默认取uint32的最大数
            unsigned int second = UINT32_MAX;
            unsigned int third = UINT32_MAX;

            if(firNodes.size() >= 2) { //得到第一个链表的前两个节点的权重的和
                first = (*firNodes.begin())->weight + (*(++firNodes.begin()))->weight;
            }
            if(secNodes.size() >= 2) { //得到第二个链表的前两个节点的权重的和
                second = (*secNodes.begin())->weight + (*(++secNodes.begin()))->weight;
            }
            if(firNodes.size() >= 1 && secNodes.size() >= 1) { //得到二个链表的第一个节点的权重的和
                third = firNodes.front()->weight + secNodes.front()->weight;
            }

            node* newNode = nullptr;
            switch(min(first, second, third)) {
            case 1:
                newNode = new node(*firNodes.begin(), *(++firNodes.begin()), first); //新建节点，权重为前两个节点的和
                firNodes.erase(firNodes.begin());  //删掉前两个节点
                firNodes.erase(firNodes.begin());

                if(firNodes.size() == 0) {
                    firNodes.push_back(newNode);
                    break;
                }
                if(first > firNodes.front()->weight) {
                    secNodes.push_back(newNode);
                } else {
                    firNodes.insert(firNodes.begin(), newNode);
                }
                break;

            case 2:
                newNode = new node(*secNodes.begin(), *(++secNodes.begin()), second);
                secNodes.erase(secNodes.begin());  //删掉前两个节点
                secNodes.erase(secNodes.begin());

                if(firNodes.size() == 0) {
                    firNodes.push_back(newNode);
                    break;
                }
                if(second > firNodes.front()->weight) {
                    secNodes.push_back(newNode);
                } else {
                    firNodes.insert(firNodes.begin(), newNode);
                }
                break;

            case 3:
                newNode = new node(firNodes.front(), secNodes.front(), third);
                firNodes.erase(firNodes.begin());  //删掉前两个节点
                secNodes.erase(secNodes.begin());

                if(firNodes.size() == 0) {
                    firNodes.push_back(newNode);
                    break;
                }
                if(third > firNodes.front()->weight) {
                    secNodes.push_back(newNode);
                } else {
                    firNodes.insert(firNodes.begin(), newNode);
                }
                break;
            }
        }

        if(firNodes.size() == 1) {
            root = firNodes.front();
        } else {
            root = secNodes.front();
        }
    }

    unsigned int calcWPL() {
        if(root == nullptr) {
            throw std::runtime_error("Build huffman tree first.");
            return 0;
        }
        unsigned int WPL = 0;
        _calcWPL(root, WPL);
        return WPL;
    }

    std::vector<std::pair<unsigned int, std::string>> getEncode() {
        if(root == nullptr) {
            throw std::runtime_error("Build huffman tree first.");
            return {};
        }
        std::vector<std::pair<unsigned int, std::string>> encodeData;
        _getEncode(root, encodeData, "");
        return encodeData;
    }

    unsigned int getDecode(std::string code) {
        if(root == nullptr) {
            throw std::runtime_error("Build huffman tree first.");
            return 0;
        }
        node* curr = root;
        for(char direction : code) {
            if(curr == nullptr) {
                throw std::runtime_error("Please input vaild code.");
            }
            if(direction == '0') {
                curr = curr->left;
                continue;
            }
            if(direction == '1') {
                curr = curr->right;
                continue;
            }
            throw std::runtime_error("Unexpected char in code.");
        }

        if(curr->left != nullptr) {
            throw std::runtime_error("Please input vaild code.");
        }
        return curr->weight;
    }
};

int main() {
    huffmanTree a;
    a.buildTree({50, 40, 38, 30, 25, 20, 15, 10, 3, 32});
    auto b = a.getEncode();
    for(auto i : b) {
        std::cout << i.first << ", " << i.second << std::endl;
    }
    for(auto i : b) {
        std::cout << i.second << ", " << a.getDecode(i.second) << ", " << i.first << std::endl;
    }
    return 0;
}
