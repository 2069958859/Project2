#include <stack>
#include <vector>
#include <string>
using namespace std;

int getPriority(char c);
string processSymbol(string exp);
void getPostfix(string exp);
void assignNum();
void calculate(vector<string> post);

string findDot(string str);
bool isNegtive(string x);
bool allZero(string str);
void process(string mm, string nn);
string sub(string mm, string nn); // double 减法运算,计算mm-nn
string add(string mm, string nn); // double 加法的运算
string multiply(string mm, string nn);