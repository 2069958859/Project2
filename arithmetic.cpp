#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
#include "source.hpp"

using namespace std;

int dot = 0;               //记录dot所在的位置
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

bool isNegtive(string x) //返回是否为负数
{
    if (x.at(0) == '-')
    {
        return true;
    }
    return false;
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

string m, n;
int dot1 = 0, dot2 = 0;

void process(string mm, string nn)
{ //处理输入的数据，记录有几位小数，将小数位较少的用0补齐
    m = findDot(mm);
    dot1 = dot;
    n = findDot(nn);
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
}

string sub(string mm, string nn) // double 减法运算,计算mm-nn
{
    if (!isNegtive(mm) && isNegtive((nn)))
    {
        return add(mm, nn.substr(1, nn.length()));
    }
    else if (isNegtive(mm) && !isNegtive((nn)))
    {
        return "-" + add(mm.substr(1, mm.length()), nn);
    }
    else if (isNegtive(mm) && isNegtive((nn)))
    {
        return sub(nn.substr(1, nn.length()), mm.substr(1, mm.length()));
    }
    process(mm, nn);
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
    bool negative = false; //记录结果的正负

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
        else
        {
            break;
        }
    }
    ans = ans.substr(0, ans.size() - flag); //除去前面的0
    if (dot >= ans.length())
    {
        ans.append(dot - ans.length() + 1, '0');
    }
    reverse(ans.begin(), ans.end());

    if (dot != 0)
    {
        ans = ans.insert(ans.length() - dot, ".");
    }
    if (negative)
    {
        ans = "-" + ans;
    }
    return ans;
}

string add(string mm, string nn) // double 加法的运算
{
    if (mm.size() == 0 || allZero(mm))
    {
        if (nn.size() == 0)
        {
            return "0";
        }
        return nn;
    }
    if (nn.size() == 0 || allZero(nn))
    {
        if (mm.size() == 0)
        {
            return "0";
        }
        return mm;
    }
    if (!isNegtive(mm) && isNegtive((nn)))
    {
        return sub(mm, nn.substr(1, nn.length()));
    }
    else if (isNegtive(mm) && !isNegtive((nn)))
    {
        return sub(nn, mm.substr(1, mm.length()));
    }
    else if (isNegtive(mm) && isNegtive((nn)))
    {
        return "-" + add(nn.substr(1, nn.length()), mm.substr(1, mm.length()));
    }
    process(mm, nn);

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
    if (dot != 0)
    {
        ans = ans.insert(ans.length() - dot, ".");
    }
    return ans;
}

string multiply(string mm, string nn)
{
    int dotsum = 0;
    string m = findDot(mm);
    dotsum = dot;
    string n = findDot(nn);
    dotsum += dot;
    bool negative = false;
    if (isNegtive(mm))
    {
        m = m.substr(1, mm.length());
    }
    if (isNegtive(nn))
    {
        n = n.substr(1, nn.length());
    }
    if ((isNegtive(mm) && !isNegtive(nn)) || (!isNegtive(mm) && isNegtive(nn)))
    {
        negative = true;
    }
    if (m.size() < n.size() || (m.size() == n.size() && m < n))
    { //保证m大
        swap(m, n);
    }
    int a = m.size();
    int b = n.size();
    int flag = 0;
    string ans;
    for (int i = b - 1; i >= 0; i--)
    {
        flag = 0;
        string aa(b - i - 1, '0');
        for (int j = a - 1; j >= 0; j--)
        {

            flag = flag + (m[j] - '0') * (n[i] - '0');
            aa.append(to_string(flag % 10));
            flag = flag / 10;
        }
        if (flag != 0)
        {
            aa.append(to_string(flag));
        }

        reverse(aa.begin(), aa.end());
        ans = add(ans, aa);
    }

    if (dotsum != 0)
    {
        flag = 0;
        for (int i = 0; i < ans.size() - dotsum - 1; i++)
        {
            if (ans[i] == '0')
            {
                flag++;
            }
        }
        ans = ans.substr(flag, ans.size()); //除去前面的0

        ans = ans.insert(ans.length() - dotsum, ".");
    }
    if (negative)
    {
        ans = "-" + ans;
    }
    return ans;
}

// int main() {
//     cout << add("0.002", "0.3") << endl;
//     cout << sub("7", "15") << endl;
//     cout << multiply("6", "3") << endl;
//     return 0;
// }