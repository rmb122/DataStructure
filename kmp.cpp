#include <vector>
#include <iostream>
#include <string>


using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::endl;


vector<unsigned int> getNextPos(string str) {
    vector<unsigned int> nextPos(str.size(), UINT32_MAX);
    unsigned int currPos = 1;
    unsigned int lastNextPos = UINT32_MAX;

    while(currPos < str.size()) {
        if(lastNextPos == UINT32_MAX || str[currPos - 1] == str[lastNextPos]) {
            lastNextPos++;
            nextPos[currPos] = lastNextPos;
            currPos++;
        } else {
            lastNextPos = nextPos[lastNextPos];
        }
    }
    return nextPos;
}


unsigned int kmp(string str, string subStr, unsigned int start = 0) {
    if(subStr.size() > str.size()) {
        return UINT32_MAX;
    }
    vector<unsigned int> nextPos = getNextPos(subStr);
    unsigned int strPos = start;
    unsigned int subStrPos = 0;
    while(strPos < str.size()) {
        if(subStrPos == UINT32_MAX || str[strPos] == subStr[subStrPos]) { //如果是匹配失败(子串位置归零, 从头开始匹配)或者当前子串正确(查找下一个字符是否正确)
            subStrPos++;
            strPos++;
        } else {
            subStrPos = nextPos[subStrPos]; //回到上一个匹配的地方
        }
    }
    if(subStrPos == subStr.size()) {
        return strPos - subStrPos + 1; //配到子串, 返回匹配到的 Index
    } else {
        return UINT32_MAX; //未能匹配到子串, 返回 UINT32_MAX
    }
}


int main() {
    auto a = kmp("abcdabfabcdabc", "abcdabc");
    return 0;
}