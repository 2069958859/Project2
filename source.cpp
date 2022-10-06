#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include "arithmetic.cpp"

using namespace std;

stack<char> symbolStack;   //存符号的栈
stack<double> fingerStack; //存数字的栈
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
    else if (c == '!')
    {
        return 4;
    }
    else
    {
        return 0;
    }
}
void processSymbol(string exp)
{ //将负数转化为0-a，这样之后的加减就不用考虑正负号了
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
        if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/' || exp[i] == '%' || exp[i] == '^' || exp[i] == '!')
        {
            if (symbolStack.empty() || symbolStack.top() == '(' || symbolStack.top() == '[' || symbolStack.top() == '{')
            {
                symbolStack.push(exp[i]);
            }
            else
            {
                while (!symbolStack.empty() && (getPriority(exp[i]) <= getPriority(symbolStack.top())))
                {
                    temp += symbolStack.top();
                    cout << temp << endl;
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
        // else if (exp[i] == ']')
        // {
        //     while (!symbolStack.empty() && symbolStack.top() != '[')
        //     {
        //         temp += symbolStack.top();
        //         symbolStack.pop();
        //         postfix.push_back(temp);
        //         temp = "";
        //     }
        //     if (!symbolStack.empty() && symbolStack.top() == '[')
        //     {
        //         symbolStack.pop();
        //     }
        // }
        // else if (exp[i] == '}')
        // {
        //     while (!symbolStack.empty() && symbolStack.top() != '{')
        //     {
        //         temp += symbolStack.top();
        //         symbolStack.pop();
        //         postfix.push_back(temp);
        //         temp = "";
        //     }
        //     if (!symbolStack.empty() && symbolStack.top() == '{')
        //     {
        //         symbolStack.pop();
        //     }
        // }
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

void calculate(string post) //计算后缀表达式
{
}
int main()
{
    string exptest = "2+3*12.4+(43*2+0.1)*11";
    getPostfix(exptest);
    for (int i = 0; i < postfix.size(); i++)
    {
        cout << postfix[i] << " ";
    }
    // cout << getPriority('+');
    return 0;
}