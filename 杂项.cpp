#include <vector>
#include <algorithm>

class first {
    /*
    1. 求下列语句段的时间复杂度：
    (1) for (i=1; i<n; i++)
    for (j=1; j<= i; j++)  x++;
    (2)  i = 1; while (i<n) i = i*2;
    (3) for (i=1; i<=n; i++)
    for (j=1; j<=n; j++)
    for (k=1; k<=n; k++)
    x++;
    （4）for (i=1; i<n; i++)
    for (j=1; j<n; j++) x++;
    for (k=1; k<n;k++) x++;
    */

    /*
    作答:
    (1) o(n^2)
    (2) o(log2(n))
    (3) o(n^3)
    (4) o(n^2+n) = o(n^2)
    */
};

class second {
public:
    double calc(std::vector<double> parameters, double x) {
        double result = 0;
        double tempX = 1;
        for (unsigned int i = 0; i < parameters.size(); ++i) {
            result += (parameters[i] * tempX);
            tempX *= x;
        }
        return result;
    }
};

class third {
    void getPowerSetEx(std::vector<int> &set, std::vector<std::vector<int>> &result, std::vector<int> last, unsigned int pos) {
        if (pos < set.size()) {  //PS: 这里的last表示的是上一次的意思_(:3」∠)_
            for (unsigned int i = 0; i < set.size() - pos; ++i) {
                std::vector<int> temp = last;
                temp.push_back(set[pos + i]);
                result.push_back(temp);
                getPowerSetEx(set, result, temp, pos + i + 1);
            }
        }
    }

public:
    std::vector<std::vector<int>> getPowerSet(std::vector<int> set) {
        std::vector<std::vector<int>> result;
        result.push_back({}); //空集
        getPowerSetEx(set, result, {}, 0);
        return result;
    }
};



bool compare(const std::pair<int, int> &a, const std::pair<int, int> &b) {
    if (a.second < b.second) {
        return true;
    } else {
        return false;
    }
}

class fourth { //这基本上就是上一题求幂集改+优化一下。。。想不出更好的了 (逃
    void getResult(std::vector<std::pair<int, int>> &items, std::vector<std::vector<std::pair<int, int>>> &result, int &weight, int &maxDepth, std::vector<std::pair<int, int>> last, unsigned int pos, int sum, int depth) {
        if (depth > maxDepth) {
            return;

        }

        if (pos < items.size()) {
            for (unsigned int i = 0; i < items.size() - pos; ++i) {
                int tempSum = sum + items[pos + i].second;
                if (tempSum > weight) { //如果此时已经比重量大了，再往下试只会更大(所以要求数据从小到大排序)
                    return;
                }
                std::vector<std::pair<int, int>> temp = last;
                temp.emplace_back(items[pos + i]);
                if (tempSum == weight) {
                    result.push_back(temp);
                }
                getResult(items, result, weight, maxDepth, temp, pos + i + 1, tempSum, depth + 1);
            }
        }
    }

public:
    std::vector<std::vector<std::pair<int, int>>> backPack(int s, int n, std::vector<int> items) {
        std::vector<std::pair<int, int>> itemCopy; //保存原来的位置信息
        for (unsigned int i = 0; i < items.size(); ++i) {
            itemCopy.emplace_back(i + 1, items[i]);
        }

        std::vector<std::vector<std::pair<int, int>>> result;
        std::sort(itemCopy.begin(), itemCopy.end(), compare); //按照从小到大排序

        getResult(itemCopy, result, s, n, {}, 0, 0, 0);
        return result;
    }
};
