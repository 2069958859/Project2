#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <valarray>
#include "source.hpp"

using namespace std;

stack<char> symbolStack;    //存符号的栈
stack<string> fingerStack;  //存数字的栈
stack<double> doubleFinger; //以double类型存数字
vector<string> postfix;
vector<string> dataName; //存代数式的数据名称
vector<string> Data;     //存代数式的数据值

double ans;

int getPriority(char c) // get 优先级
{
    if (c == '+' || c == '-')
    {
        return 1;
    }
    else if (c == '*' || c == '/')
    {
        return 2;
    }
    else if (c == '%' || c == '^' || c == 'S')
    {
        return 3;
    }
    else
    { //数字的情况
        return 0;
    }
}

int equalmark = 0;

bool isalgExp(string exp)
{

    equalmark = exp.find("=", 1);
    if (equalmark != -1)
    {
        return true;
    }
    return false;
}

void algexp(string algexp)
{ //处理代数式
    string dataname = "";
    string temp = "";
    double data;
    if (isalgExp(algexp))
    {
        dataName.push_back(algexp.substr(0, equalmark));
        for (int i = equalmark + 1; i < algexp.size(); i++)
        {
            temp += algexp[i];
            while (algexp.length() > i + 1 && (isdigit(algexp[i + 1]) || algexp[i + 1] == '.'))
            {
                temp = temp + algexp[i + 1];
                i++;
            }

            Data.push_back(temp);
        }
    }
}

string processSymbol(string exp1)
{ //将负数转化为0-a
    string exp = exp1;
    for (size_t i = 0; i < exp.size(); i++)
    {
        if (exp[i] == '-')
        {
            if (i == 0)
            {
                exp.insert(0, 1, '0');
            }
            else if (exp[i - 1] == '(')
            { //例如：2*(-1)
                exp.insert(i, 1, '0');
            }
        }
    }
    return exp;
}

void getPostfix(string exp) //将表达式转化为后缀表达式
{
    string temp;
    for (size_t i = 0; i < exp.size(); i++)
    {
        char flag;
        switch (exp[i])
        {
        case ')':
            flag = '(';
            break;
        case ']':
            flag = '[';
            break;
        case '}':
            flag = '{';
        }
        temp = "";
        if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/' || exp[i] == '%' || exp[i] == '^')
        {
            if (symbolStack.empty() || symbolStack.top() == '(' || symbolStack.top() == '[' ||
                symbolStack.top() == '{')
            {
                symbolStack.push(exp[i]);
            }
            else
            {
                while (!symbolStack.empty() && (getPriority(exp[i]) <= getPriority(symbolStack.top())))
                {
                    temp += symbolStack.top();
                    // cout << temp << endl;
                    symbolStack.pop();
                    postfix.push_back(temp);
                    temp = "";
                }
                symbolStack.push(exp[i]);
            }
        }
        else if (exp[i] == '(' || exp[i] == '[' || exp[i] == '{')
        {
            symbolStack.push(exp[i]);
        }
        else if (exp[i] == ')' || exp[i] == ']' || exp[i] == '}')
        {
            while (!symbolStack.empty() && symbolStack.top() != flag)
            {
                temp += symbolStack.top();
                symbolStack.pop();
                postfix.push_back(temp);
                temp = "";
            }
            if (!symbolStack.empty() && symbolStack.top() == flag)
            {
                symbolStack.pop();
            }
        }
        else
        { //数字的情况
            if (isdigit(exp[i]))
            {
                temp += exp[i];
                while (exp.length() > i + 1 && (isdigit(exp[i + 1]) || exp[i + 1] == '.'))
                {
                    temp = temp + exp[i + 1];
                    i++;
                }
                postfix.push_back(temp);
            }
            else if (exp[i] == 's' && exp[i + 1] == 'q' && exp[i + 2] == 'r' &&
                     exp[i + 3] == 't')
            { // math function sqrt(x)
                temp += exp[i + 5];
                while (exp.length() > i + 6 && (isdigit(exp[i + 6]) || exp[i + 6] == '.'))
                {
                    temp = temp + exp[i + 6];
                    i++;
                }
                i = i + 6;
                postfix.push_back(temp);
                symbolStack.push('S'); //作为sqrt函数的记号
            }
        }
    }
    while (!symbolStack.empty())
    {
        temp = "";
        temp += symbolStack.top();
        symbolStack.pop();
        postfix.push_back(temp);
    }
}

string num1, num2;

void assignNum()
{
    if (!fingerStack.empty())
    {
        num2 = fingerStack.top();
        fingerStack.pop();
        doubleFinger.pop();
    }
    if (!fingerStack.empty())
    {
        num1 = fingerStack.top();
        fingerStack.pop();
        doubleFinger.pop();
    }
}

void calculate(vector<string> post) //计算后缀表达式
{
    string temp;
    double d1 = 0, d2 = 0, num = 0;
    for (int i = 0; i < post.size(); i++)
    {
        temp = post[i]; // string
        if (isdigit(temp[0]))
        {
            num = atof(temp.c_str()); //将该字符对应的字符串转化成double类型
            doubleFinger.push(num);   //以double类型存入
            fingerStack.push(temp);   //以字符串类型存入
        }
        else if (temp == "+")
        {
            assignNum();
            fingerStack.push(add(num1, num2));
            doubleFinger.push(stod(add(num1, num2)));
            //            cout << fingerStack.top() << endl;
        }
        else if (temp == "-")
        {
            assignNum();
            fingerStack.push(sub(num1, num2));
            doubleFinger.push(stod(sub(num1, num2)));
        }
        else if (temp == "*")
        {
            assignNum();
            fingerStack.push(multiply(num1, num2));
            doubleFinger.push(stod(multiply(num1, num2)));
        }
        else if (temp == "/")
        {
            if (!fingerStack.empty())
            {
                d2 = doubleFinger.top();
                fingerStack.pop();
                doubleFinger.pop();
            }
            if (!fingerStack.empty())
            {
                d1 = doubleFinger.top();
                fingerStack.pop();
                doubleFinger.pop();
            }
            if (d2 == 0)
            {
                cout << "错误：被除数为0" << endl;
            }
            else
            {
                fingerStack.push(to_string(d1 / d2));
                doubleFinger.push(d1 / d2);
            }
        }
        else if (temp == "^")
        {
            if (!fingerStack.empty())
            {
                d2 = doubleFinger.top();
                fingerStack.pop();
                doubleFinger.pop();
            }
            if (!fingerStack.empty())
            {
                d1 = doubleFinger.top();
                fingerStack.pop();
                doubleFinger.pop();
            }
            fingerStack.push(to_string(pow(d1, d2)));
            doubleFinger.push(pow(d1, d2));
        }
        else if (temp == "%")
        {
            if (!fingerStack.empty())
            {
                d2 = doubleFinger.top();
                fingerStack.pop();
                doubleFinger.pop();
            }
            if (!fingerStack.empty())
            {
                d1 = doubleFinger.top();
                fingerStack.pop();
                doubleFinger.pop();
            }
            fingerStack.push(to_string(fmod(d1, d2)));
            doubleFinger.push(fmod(d1, d2)); // fmod可以小数求余
        }
        else if (temp == "S")
        { // sqrt(x) function
            if (!fingerStack.empty())
            {
                d2 = doubleFinger.top();
                fingerStack.pop();
                doubleFinger.pop();
            }
            fingerStack.push(to_string(sqrt(d2)));
            doubleFinger.push(sqrt(d2)); // sqrt 函数开平方
        }
    }
}

int main()
{
    string exptest;
    do
    {
        getline(cin, exptest);
        algexp(exptest);
    } while (isalgExp(exptest));

    for (int i = 0; i < dataName.size(); i++)
    { //将式子中的变量名替换成值
        int place = exptest.find(dataName[i]);
        if (place != -1)
        {
            exptest.replace(place, dataName[i].length(), Data[i]);
        }
        else
        {
            cout << "variables are not be defined" << endl;
            return 0;
        }
    }
    exptest = processSymbol(exptest);

    getPostfix(exptest);
    calculate(postfix);
    cout << doubleFinger.top() << endl;
    return 0;
}