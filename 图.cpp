#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <vector>

using std::list;
using std::queue;
using std::stack;
using std::vector;

using std::make_pair;
using std::pair;

using std::cout;
using std::endl;

class graphNode {
public:
    list<pair<unsigned int, unsigned int>> connectedNode; //<序号, 距离>
    int value;
    bool isVisited;

    graphNode(int value) : value(value), isVisited(false) {}
    graphNode() : value(0), isVisited(false) {}
};

class graph {
private:
    vector<graphNode> graphNodes;

    unsigned int min(unsigned int &a, unsigned int &b) {
        if(a > b) {
            return b;
        }
        return a;
    }

    unsigned int max(unsigned int &a, unsigned int &b) {
        if(a > b) {
            return a;
        }
        return b;
    }

    void initTree() {
        for (graphNode &node : graphNodes) {
            node.isVisited = false;
        }
    }

    void _travelDFS(graphNode &node) {
        cout << node.value << " ";
        node.isVisited = true;

        for (auto &nodeInfo : node.connectedNode) {
            if (!graphNodes[nodeInfo.first].isVisited) {
                _travelDFS(graphNodes[nodeInfo.first]);
            }
        }
    }

    void _travelBFS(graphNode &node, queue<unsigned int> &nodeQueue) {
        cout << node.value << " ";
        node.isVisited = true;

        for (auto &nodeInfo : node.connectedNode) {
            nodeQueue.push(nodeInfo.first);
        }

        while (!nodeQueue.empty()) {
            unsigned int temp;
            temp = nodeQueue.front();
            nodeQueue.pop();
            if (!graphNodes[temp].isVisited) {
                _travelBFS(graphNodes[temp], nodeQueue);
            }
        }
    }

    void _isTree(unsigned int currSerialNum, unsigned int lastSerialNum, bool &judge) {
        graphNodes[currSerialNum].isVisited = true;

        for (auto nodeInfo : graphNodes[currSerialNum].connectedNode) {
            if (nodeInfo.first != lastSerialNum) { //不是相邻节点进行下一步
                if (judge && !graphNodes[nodeInfo.first].isVisited) {
                    _isTree(nodeInfo.first, currSerialNum, judge);
                } else {
                    judge = false;
                    return;
                }
            }
        }
    }

    void _isRoot(graphNode &node, bool &judge) {
        node.isVisited = true; //判断是否是树, 因为是有向图, 所以不用判断是否是相邻节点

        for (auto &nodeInfo : node.connectedNode) {
            if (judge && !graphNodes[nodeInfo.first].isVisited) {
                _isRoot(graphNodes[nodeInfo.first], judge);
            } else {
                judge = false;
                return;
            }
        }
    }

    void _isJoint(graphNode &node, unsigned int &branchIndex, vector<unsigned int> &branches, unsigned int &jointIndex, bool &judge) {
        node.isVisited = true;

        for (auto &nodeInfo : node.connectedNode) {
            if (judge && (branches[nodeInfo.first] == UINT32_MAX || branches[nodeInfo.first] == branchIndex)) {
                //若没有访问过的继续递归, 如果是当前分支访问过的跳过; 否则跳出循环
                if (nodeInfo.first == jointIndex) {
                    //如果访问的是关节点就跳过, 否则两个分支都会访问一遍, 导致错误
                    continue;
                }
                if (!graphNodes[nodeInfo.first].isVisited) {
                    //没有访问过继续递归
                    branches[nodeInfo.first] = branchIndex;
                    _isJoint(graphNodes[nodeInfo.first], branchIndex, branches, jointIndex, judge);
                }
            } else {
                judge = false;
                return;
            }
        }
    }

    void _getKeyPath(unsigned int currSerialNum, vector<unsigned int> &earliestTime, vector<unsigned int> &keyTime, vector<unsigned int> currPath, vector<vector<unsigned int>> &keyPaths) {
        currPath.push_back(currSerialNum);
        if(!graphNodes[currSerialNum].connectedNode.size()) { //如果到达终点 (出度为 0), 返回函数且放入关键路径中
            keyPaths.push_back(currPath);
            return;
        }
        for(auto &nodeInfo : graphNodes[currSerialNum].connectedNode) {
            if(keyTime[nodeInfo.first] == earliestTime[nodeInfo.first]) {
                _getKeyPath(nodeInfo.first, earliestTime, keyTime, currPath, keyPaths);
            }
        }
    }

public:
    graph(unsigned int size) {
        graphNodes.resize(size);
    }

    graph(std::vector<int> nodes) {
        graphNodes.reserve(nodes.size());

        for (int value : nodes) {
            graphNodes.push_back(graphNode(value));
        }
    }

    void chageNodeVal(unsigned int serialNum, int value) {
        graphNodes[serialNum].value = value;
    }

    void connectNode(unsigned int fromSerialNum, unsigned int toSerialNum, unsigned int distance) {
        graphNodes[fromSerialNum].connectedNode.push_back(make_pair(toSerialNum, distance));
    }

    void connectNodeNoDirct(unsigned int firSerialNum, unsigned int secSerialNum, unsigned int distance) {
        graphNodes[firSerialNum].connectedNode.push_back(make_pair(secSerialNum, distance));
        graphNodes[secSerialNum].connectedNode.push_back(make_pair(firSerialNum, distance));
    }

    void travelDFS() {
        initTree();
        for (graphNode &node : graphNodes) {
            if (!node.isVisited) {
                _travelDFS(node);
            }
        }
    }

    void travelBFS() {
        initTree();
        queue<unsigned int> temp;
        for (graphNode &node : graphNodes) {
            if (!node.isVisited) {
                _travelBFS(node, temp);
            }
        }
    }

    bool isTree() { //无向图
        if (graphNodes.size() <= 1) {
            return true;
        }
        initTree();
        bool judge = true;
        _isTree(0, 0, judge);

        for (graphNode &node : graphNodes) {
            if (!node.isVisited) { //若存在没有被访问到的节点, 说明存在孤儿节点, 这个图不是树
                judge = false;
                break;
            }
        }
        return judge;
    }

    bool isRoot(unsigned int serialNum) {
        //有向图
        if (serialNum >= graphNodes.size()) {
            throw std::runtime_error("Invaild serialNum.");
        }
        initTree();
        bool judge = true;
        _isRoot(graphNodes[serialNum], judge); //遍历所有此节点连接的节点

        for (graphNode &node : graphNodes) {
            if (!node.isVisited) { //若存在没有被访问到的节点, 说明这不是根节点
                judge = false;
                break;
            }
        }
        return judge;
    }

    bool isJoint(unsigned int serialNum) { //无向图
        if (serialNum >= graphNodes.size()) {
            throw std::runtime_error("Invaild serialNum.");
        }
        initTree();
        vector<unsigned int> branches(graphNodes.size(), UINT32_MAX); //记录每个节点是被哪个枝干访问过的

        unsigned int currIndex = 0;
        bool judge = true;
        for (auto &nodeInfo : graphNodes[serialNum].connectedNode) {
            if (judge && branches[nodeInfo.first] == UINT32_MAX) {
                branches[nodeInfo.first] = currIndex;
                _isJoint(graphNodes[nodeInfo.first], currIndex, branches, serialNum, judge);
                currIndex++;
            } else {
                judge = false;
                break;
            }
        }
        return judge;
    }

    vector<unsigned int> topoSort() {
        vector<unsigned int> inDegreeRecord(graphNodes.size(), 0); //记录入度
        stack<unsigned int> currZeroinDegree;
        vector<unsigned int> result;

        for(graphNode &node : graphNodes) {
            for(auto &nodeInfo : node.connectedNode) {
                inDegreeRecord[nodeInfo.first]++; //有入度的加一
            }
        }

        for(unsigned int i = 0; i < inDegreeRecord.size(); ++i) {
            if(!inDegreeRecord[i]) {
                currZeroinDegree.push(i); //入度为 0 的压入栈
            }
        }

        while(!currZeroinDegree.empty()) {
            unsigned int curr = currZeroinDegree.top();
            result.push_back(curr); //出栈同时, 进入结果
            currZeroinDegree.pop();
            for(auto &nodeInfo : graphNodes[curr].connectedNode) {
                inDegreeRecord[nodeInfo.first]--; //遍历到的入度减一
                if(!inDegreeRecord[nodeInfo.first]) { //如果入度为 0, 入栈
                    currZeroinDegree.push(nodeInfo.first);
                }
            }
        }
        return result;
    }

    vector<vector<unsigned int>> getKeyPath() {
        vector<unsigned int> inDegreeRecord(graphNodes.size(), 0); //记录入度
        vector<unsigned int> earliestTime(graphNodes.size(), 0); //记录最早发生时间
        vector<unsigned int> keyTime(graphNodes.size(), UINT32_MAX); //记录关键时间
        vector<unsigned int> topo; //记录拓扑排序顺序
        stack<unsigned int> currZeroinDegree; //记录零入度的节点的栈
        vector<vector<unsigned int>> keyPaths; //记录关键路径

        for(graphNode &node : graphNodes) {
            for(auto &nodeInfo : node.connectedNode) {
                inDegreeRecord[nodeInfo.first]++; //有入度的加一
            }
        }

        for(unsigned int i = 0; i < inDegreeRecord.size(); ++i) {
            if(!inDegreeRecord[i]) {
                currZeroinDegree.push(i); //入度为 0 的压入栈
            }
        }

        while(!currZeroinDegree.empty()) {
            unsigned int curr = currZeroinDegree.top();
            topo.push_back(curr);
            currZeroinDegree.pop();
            for(auto &nodeInfo : graphNodes[curr].connectedNode) {
                inDegreeRecord[nodeInfo.first]--; //遍历到的入度减一
                if(!inDegreeRecord[nodeInfo.first]) { //如果入度为 0, 入栈
                    currZeroinDegree.push(nodeInfo.first);
                    unsigned int temp = earliestTime[curr] + nodeInfo.second; //当前遍历到的节点的距离
                    earliestTime[nodeInfo.first] = max(temp, earliestTime[nodeInfo.first]); //将距离修改为最大的
                }
            }
        }

        keyTime[topo.back()] = earliestTime[topo.back()]; //初始化终点的关键时间

        for(auto i = topo.rbegin(); i != topo.rend(); i++) { //按照拓扑排序的逆序
            for(auto &nodeInfo : graphNodes[*i].connectedNode) {
                unsigned int temp = keyTime[nodeInfo.first] - nodeInfo.second; //当前遍历到的节点的距离
                keyTime[*i] = min(keyTime[*i], temp);
            }
        }

        _getKeyPath(topo.front(), earliestTime, keyTime, {}, keyPaths);
        return keyPaths;
    }
};

int main() {
    graph test({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    /*
    test.connectNode(0, 5, 2);
    test.connectNode(0, 1, 2);
    test.connectNode(5, 4, 2);
    test.connectNode(5, 7, 2);
    test.connectNode(1, 2, 2);
    test.connectNode(1, 6, 2);
    test.connectNode(2, 3, 2);
    test.connectNode(7, 3, 2);
    */
    test.connectNode(0, 1, 3);
    test.connectNode(0, 2, 4);
    test.connectNode(0, 3, 5);
    test.connectNode(1, 4, 6);
    test.connectNode(2, 4, 3);
    test.connectNode(2, 5, 4);
    test.connectNode(3, 5, 3);
    test.connectNode(3, 8, 6);
    test.connectNode(4, 6, 5);
    test.connectNode(4, 7, 4);
    test.connectNode(5, 7, 3);
    test.connectNode(5, 8, 4);
    test.connectNode(6, 9, 5);
    test.connectNode(7, 9, 6);
    test.connectNode(8, 9, 4);

    test.travelDFS();
    cout << endl;
    test.travelBFS();
    cout << endl;

    test.getKeyPath();
    return 0;
}
