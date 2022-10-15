#include <stack>
#include <vector>
#include <string>
using namespace std;

bool isalgExp(string exp);  //检查是否为“x=1"的形式
void algexp(string algexp); //处理代数式
int getPriority(char c);
string processSymbol(string exp); //将负数转化为0-a
bool isDouble(string x);          //返回是否是浮点数（整数也判断为true)
void getPostfix(string exp);      //将表达式转化为后缀表达式
string erasepostZero(string str); //去掉小数点后多余的0
void assignNum();
void calculate(vector<string> post); //计算后缀表达式

string findDot(string str); //查找是否有小数点，如果有，就记录小数有几位并抹去小数点
bool isNegtive(string x);
bool allZero(string str);           //检查是否为0
void process(string mm, string nn); //处理输入的数据，记录有几位小数，将小数位较少的用0补齐
string sub(string mm, string nn);   // double 减法运算(计算mm-nn)
string add(string mm, string nn);   // double 加法的运算
string multiply(string mm, string nn);