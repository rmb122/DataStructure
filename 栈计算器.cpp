#include <stdexcept>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>

template<typename type>
class stack {
private:
    type* data;

    size_t length;
public:
    size_t count;

    stack(unsigned int length) : length(length), count(0) {
        data = new type[length];
        //memset(data, 0, length * sizeof(type));
    }

    ~stack() {
        delete[] data;
    }

    void resize(size_t length) {
        delete[] data;
        data = new type[length];
        this->length = length;
        count = 0;
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == length;
    }

    type top() {
        if (count <= 0) {
            throw std::runtime_error("Stack is empty");
        }
        return data[count - 1];
    }

    void push(type val) {
        if (count == length) {
            throw std::runtime_error("Stack is full");
        }

        data[count++] = val;
    }

    type pop() {
        if (count <= 0) {
            throw std::runtime_error("Stack is empty");
        }
        type val = data[--count];
        return val;
    }

    void dumpVal() {
        for (unsigned int i = 0; i < length; ++i) {
            std::cout << "[" << i << "]";
            std::cout << " " << data[i] << " ";
        }
    }
};


class calculator {
private:
    stack<char> symbols;
    stack<double> nums;

    std::string::iterator beg;
    std::string::iterator curritr;
    std::string::iterator end;

    void pushNums();
    void pushSym();
    void calcTheTop();
public:
    calculator() : symbols(1), nums(1) { };
    double calc(std::string expression);
};


void calculator::pushNums() {
    bool hasDot = false;
    double num = 0;
    int digits = 1; //小数点后的位数

    while (true) {
        if (curritr != end && *curritr == '.') {
            if (hasDot == true) {
                throw std::runtime_error("Unexcepted dot in number.");
            }
            hasDot = true;
            ++curritr;
            continue;
        }

        if (curritr != end && *curritr <= 57 && *curritr >= 48) {
            if (hasDot) {
                num += (*curritr - 48) / pow(10, digits);
                ++digits;
                ++curritr;
            } else {
                num *= 10;
                num += *curritr - 48;
                ++curritr;
            }
            continue;
        }

        nums.push(num); //不是数字或'.'就将数字存入栈中，并退出循环和函数
        return;
    }
}

void calculator::pushSym() {
    symbols.push(*curritr);
    ++curritr;
}

void calculator::calcTheTop() {
    char oper = symbols.pop();
    double num1 = nums.pop();

    if (oper == '$') {
        nums.push(-num1);
        return;
    }

    double num2 = nums.pop();

    if (oper == '+') {
        nums.push(num2 + num1);
        return;
    }
    if (oper == '-') {
        nums.push(num2 - num1);
        return;
    }
    if (oper == '*') {
        nums.push(num2 * num1);
        return;
    }
    if (oper == '/') {
        nums.push(num2 / num1);
        return;
    }
}

double calculator::calc(std::string expression) {
    symbols.resize(expression.size());
    nums.resize(expression.size()); //用表达式的长度构造栈，保证不会溢出

    beg = expression.begin();
    curritr = expression.begin();
    end = expression.end();

    while (curritr != end) { //这里想了下不是if else嵌套地狱就是continue地狱，我选择continue地狱，看起来舒服点233
        if (*curritr <= 57 && *curritr >= 48) {
            pushNums();
            continue;
        }

        if (*curritr == '+' || *curritr == '-' || *curritr == '*' || *curritr == '/' || *curritr == '(' || *curritr == ')') {
            if (*curritr == '-') { //如果是-号且前面紧跟着一个运算符，代表负号
                if (curritr == beg || *(curritr - 1) == '+' || *(curritr - 1) == '-' || *(curritr - 1) == '*' || *(curritr - 1) == '/' || *(curritr - 1) == '(') {
                    symbols.push('$'); //以$储存
                    curritr++;
                    continue;
                }
            }

            if (*curritr == ')') {
                while (symbols.top() != '(') {
                    calcTheTop();
                }
                symbols.pop(); //pop掉(
                curritr++; //这里因为不是调用pushSym()，需要手动++
                continue;
            }

            if (symbols.isEmpty() || symbols.top() == '(') {
                pushSym();
                continue;
            }

            if (symbols.top() == '*' || symbols.top() == '/') {
                if (*curritr == '*' || *curritr == '/' || *curritr == '+' || *curritr == '-') {
                    calcTheTop();
                    continue;
                }
                pushSym();
                continue;
            }

            if (symbols.top() == '+' || symbols.top() == '-') {
                if (*curritr == '+' || *curritr == '-') {
                    calcTheTop();
                    continue;
                }
                pushSym();
                continue;
            }

            if (symbols.top() == '$') {
                calcTheTop();
                continue;
            }
        }

        throw std::runtime_error("Unexcepted symbol.");
    }

    while (!symbols.isEmpty()) {
        calcTheTop();
    }

    return nums.top();
}

class ioInterface {
private:
    calculator calc;

public:
    void start() {
        std::cout << "Here is a simple calculator, input expressions to calc it." << std::endl;
        std::string query;
        while (true) {
            std::cout << ">>>";
            std::cin >> query;
            try {
                double result = calc.calc(query);
                std::cout << "The result is " << result << "." << std::endl;
            } catch (std::runtime_error) {
                std::cout << "Your expressions is invaild." << std::endl;
            }
        }
    }
};

class test {
    calculator calc;
public:
    void run(std::vector<std::string> exps) {
        for (auto exp : exps) {
            std::cout << "Now runing in \"" << exp << "\", result is " << calc.calc(exp) << std::endl;
        }
    }
};
