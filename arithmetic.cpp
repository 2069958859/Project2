#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int dot = -1;              //记录dot所在的位置
string findDot(string str) //查找是否有小数点，如果有，就记录小数有几位并抹去小数点
{
    string a = str;
    dot = a.find(".", 1);
    if (dot != -1)
    {
        a.erase(dot, 1);
        dot = str.length() - dot - 1; //计算小数点后有几位
    }
    else
    { //无小数点，即有0位小数
        dot = 0;
    }
    return a;
}

bool allZero(string str)
{ //检查是否为0
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] != '0')
        {
            return false;
        }
    }
    return true;
}

string sub(string mm, string nn) // double 减法运算,计算mm-nn
{
    if (mm == nn || (allZero(mm) && allZero(nn)))
    {
        return "0";
    }
    else if (allZero(mm))
    {
        return "-" + nn;
    }
    else if (allZero(nn))
    {
        return mm;
    }
    int dot1, dot2;
    bool negative = false; //记录结果的正负
    string m = findDot(mm);
    dot1 = dot;
    string n = findDot(nn);
    dot2 = dot;
    if (dot1 != 0 || dot2 != 0)
    { //补齐位数
        if (dot1 >= dot2)
        { // m的小数位多
            n.append(dot1 - dot2, '0');
            dot = dot1;
        }
        else if (dot1 < dot2)
        {
            m.append(dot2 - dot1, '0');
            dot = dot2;
        }
    }

    if (m.size() < n.size() || (m.size() == n.size() && m < n))
    { //保证m大
        swap(m, n);
        negative = true;
    }
    int a = m.size() - 1;
    int b = n.size() - 1;
    string ans;
    int flag = 0;
    while (a >= 0 || b >= 0)
    {
        int num1, num2;
        num1 = (a >= 0) ? m[a--] - '0' : 0;
        num2 = (b >= 0) ? n[b--] - '0' : 0;
        num1 = num1 - flag; //减去借位
        if (num1 < num2)
        {
            num1 = num1 + 10;
            flag = 1;
        }
        else
        {
            flag = 0;
        }
        ans += num1 - num2 + '0';
    }
    flag = 0;
    for (int i = ans.size() - 1; i >= 0; i--)
    {
        if (ans[i] == '0')
        {
            flag++;
        }
    }
    ans = ans.substr(0, ans.size() - flag); //除去前面的0
    reverse(ans.begin(), ans.end());
}

string add(string mm, string nn) // double 加法的运算
{
    int dot1, dot2;

    string m = findDot(mm);
    dot1 = dot;
    string n = findDot(nn);
    dot2 = dot;
    if (dot1 != 0 || dot2 != 0)
    { //补齐位数
        if (dot1 >= dot2)
        { // m的小数位多
            n.append(dot1 - dot2, '0');
            dot = dot1;
        }
        else if (dot1 < dot2)
        {
            m.append(dot2 - dot1, '0');
            dot = dot2;
        }
    }

    int a = m.size() - 1;
    int b = n.size() - 1;
    string ans;
    int flag = 0;
    while (a >= 0 || b >= 0 || flag != 0)
    {
        int num1, num2;
        num1 = (a >= 0) ? m[a--] - '0' : 0;
        num2 = (b >= 0) ? n[b--] - '0' : 0;

        flag = flag + num1 + num2; //计算两位的和
        ans += '0' + flag % 10;
        flag = flag / 10; //进位
    }
    reverse(ans.begin(), ans.end());
    ans = ans.insert(ans.length() - dot, ".");
    return ans;
}

int main()
{
    cout << add("99999999999999.22222222", "123.0231");
    return 0;
}