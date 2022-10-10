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
vector<string> postfix;     // expression前缀表达式形式
vector<string> dataName;    //存代数式的数据名称
vector<string> Data;        //存代数式的数据值

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
{ //检查是否为“x=1"的形式

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
    if (isalgExp(algexp))
    {
        dataName.push_back(algexp.substr(0, equalmark));
        Data.push_back(algexp.substr(equalmark + 1, algexp.size()));
        // for (int i = equalmark + 1; i < algexp.size(); i++)
        // {
        //     temp += algexp[i];
        //     while (algexp.length() > i + 1 && (isdigit(algexp[i + 1]) || algexp[i + 1] == '.'))
        //     {
        //         temp = temp + algexp[i + 1];
        //         i++;
        //     }

        //     Data.push_back(temp);
        // }
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
        if (exp[i] == ' ')
        { //处理输入的空格
        }
        else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/' || exp[i] == '%' || exp[i] == '^')
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
        {
            if (isdigit(exp[i])) //数字的情况
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
            else
            { //非法输入
                cout << "not valid input!" << endl;
                exit(0); //直接结束程序
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

void assignNum() //从栈中取值用来计算
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

string erasepostZero(string str) //去掉小数点后多余的0
{
    string ans;
    int dot = str.find(1, '.');
    int zeros = 0;
    for (int i = str.size() - 1; i >= dot; i--)
    {
        if (str[i] == '0' || str[i] == '.')
        {
            zeros++;
        }
        else
        {
            break;
        }
    }
    ans = str.substr(0, str.size() - zeros);
    return ans;
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
                cout << "错误：除数为0" << endl;
                exit(0);
            }
            else
            {
                fingerStack.push(erasepostZero(to_string(d1 / d2)));
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
            fingerStack.push(erasepostZero(to_string(pow(d1, d2))));
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
            fingerStack.push(erasepostZero(to_string(fmod(d1, d2))));
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
            fingerStack.push(erasepostZero(to_string(sqrt(d2))));
            doubleFinger.push(sqrt(d2)); // sqrt 函数开平方
        }
    }
}

bool isExit(string exp)
{
    if (exp == "exit")
    {
        return true;
    }
    return false;
}

void clearStack()
{
    while (!symbolStack.empty())
    {
        symbolStack.pop();
    }
    while (!fingerStack.empty())
    {
        fingerStack.pop();
    }
    while (!doubleFinger.empty())
    {
        doubleFinger.pop();
    }
}

void help()
{
    cout << "This is an instruction to the calculator user." << endl;
    cout << "constant value:" << endl;
    cout << "E=2.718281828459" << endl;
    cout << "PI=3.14159265358" << endl;
    cout << endl;
    cout << "Supported functions:" << endl;
    cout << "1. Basic mathematical calculation: + - * / % ^" << endl;
    cout << "example-1.1:" << endl;
    cout << "3*4" << endl;
    cout << "12" << endl;
    cout << "example-1.2:" << endl;
    cout << "3.45/3.2" << endl;
    cout << "1.078125" << endl;
    cout << endl;

    cout << "2. calculation with brackets: ( ) [ ] { }" << endl;
    cout << "example-2.1:" << endl;
    cout << "(4.5+6.8)*1.2" << endl;
    cout << "13.56" << endl;
    cout << "example-2.2:" << endl;
    cout << "{2+3*[5+6/(2+3)]*(2+3)}*2" << endl;
    cout << "190.0" << endl;
    cout << endl;

    cout << "3. Definition with any names made up of letters: x, y, zz, hello ,and can be defined by an expression" << endl;
    cout << "example-3.1:" << endl;
    cout << "x=3" << endl;
    cout << "y=6" << endl;
    cout << "x+2*y" << endl;
    cout << "21" << endl;
    cout << "example-3.2:" << endl;
    cout << "hello=1" << endl;
    cout << "world=2" << endl;
    cout << "x=3" << endl;
    cout << "x+x*hello-world" << endl;
    cout << "4" << endl;
    cout << endl;

    cout << "4. Some single math functions:" << endl;
    cout << "example-4.1:" << endl;
    cout << "sqrt(3)" << endl;
    cout << "1.732051" << endl;
    cout << "example-4.2:" << endl;
    cout << endl;

    cout << "5. Arbitrary Precision:" << endl;
    cout << "Any length of integer or decimal value are supported in this calculator." << endl;
    cout << "example-5.1:" << endl;
    cout << "6.333333333333333333333333333333333333333333*2+22.222222222222222222222222222222222222" << endl;
    cout << "34.888888888888888888888888888888888888666666" << endl;
    cout << endl;
}

int main()
{
    string expression;
    while (true)
    {
        getline(cin, expression);
        algexp(expression);
        if (expression == "")
        {
        }
        else if (isExit(expression))
        {
            break;
        }

        else if (expression == "help")
        {
            help();
        }
        else
        { //替换常数
            int placePI = expression.find("PI");
            while (placePI != -1)
            {
                expression.replace(placePI, 2, "3.14159265358");
                placePI = expression.find("PI");
            }
            int placeE = expression.find("E");
            while (placeE != -1)
            {
                expression.replace(placeE, 1, "2.718281828459");
                placeE = expression.find("E");
            }

            while (isalgExp(expression)) //是给变量赋值的式子
            {
                getline(cin, expression);
                algexp(expression);
            }
            for (int i = 0; i < dataName.size(); i++)
            { //将式子中的变量名替换成值
                int place = expression.find(dataName[i]);
                while (place != -1)
                {
                    expression.replace(place, dataName[i].length(), "(" + Data[i] + ")");
                    place = expression.find(dataName[i]);
                }
            }
            expression = processSymbol(expression);
            getPostfix(expression);
            calculate(postfix);
            cout << fingerStack.top() << endl;

            dataName.clear(); //清空之前的信息
            Data.clear();
            postfix.clear();
            clearStack(); //将栈清空
        }
    }
    return 0;
}
