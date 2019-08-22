#include <vector>
#include <iostream>
#include <string>


using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::endl;


vector<unsigned int> getNextPos(string str) {
    vector<unsigned int> nextPos = {UINT32_MAX};
    nextPos.resize(str.size(), 0);

    unsigned int currPos = 2;
    unsigned int lastNextPos = 0;

    while(currPos < str.size()) {
        if(str[currPos - 1] == str[lastNextPos]) {
            nextPos[currPos] = lastNextPos + 1;
            lastNextPos++;
            currPos++;
        } else {
            lastNextPos = 0;
            currPos++;
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
    while(strPos < str.size() && (subStrPos < subStr.size())) {
        if(str[strPos] == subStr[subStrPos]) { 
            subStrPos++;
            strPos++;
        } else {
            subStrPos = nextPos[subStrPos]; //回到上一个匹配的地方
            if (subStrPos == UINT32_MAX) {
                subStrPos = 0;
                strPos++;
            }  
         }
    }
    if(subStrPos == subStr.size()) {
        return strPos - subStrPos; //配到子串, 返回匹配到的 Index
    } else {
        return UINT32_MAX; //未能匹配到子串, 返回 UINT32_MAX
    }
}


int main() {
    auto a = kmp("caksjdoquwejkhdkkjaksabcdeabcdeabcdf123psaodo", "abcdeabcdf");
    printf("%d", a);
    return 0;
}
