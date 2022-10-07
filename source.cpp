#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <valarray>
#include "source.h"

using namespace std;

stack<char> symbolStack;    //存符号的栈
stack<string> fingerStack;  //存数字的栈
stack<double> doubleFinger; //以double类型存数字
vector<string> postfix;

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
    else if (c == '%' || c == '^')
    {
        return 3;
    }
    else
    { //数字的情况
        return 0;
    }
}

string processSymbol(string exp1)
{ //将负数转化为0-a，这样之后的加减就不用考虑正负号了
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
    }
}

int main()
{
    string exptest;
    getline(cin, exptest);
    exptest = processSymbol(exptest);

    getPostfix(exptest);
    calculate(postfix);
    cout << doubleFinger.top() << endl;
    return 0;
}