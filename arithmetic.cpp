#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int dot = -1;              //记录dot所在的位置
string findDot(string str) //查找是否有小数点，如果有，就记录小数点的位置并抹去小数点
{
    string a = str;

    for (int i = 0; i < a.size(); i++)
    {
        dot = a.find(".", 1);
        if (dot != -1)
        {
            a.erase(dot, 1);
        }
    }
    return a;
}
double add(double a1, double a2) // double 加法的运算
{
    string m = to_string(a1);
    string n = to_string(a2);

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
    return ans;
}
int main()
{

    return 0;
}