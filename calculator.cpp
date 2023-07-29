#include <iostream>
#include <string>
using namespace std;

double calculate();

//converts string to int
int toInt(string exp) {
    int res = 0;
    for (int i = 0; i < exp.size(); i++) {
        res = res * 10 + (int)(exp[i] - '0');
    }
    return res;
}

//determines wheather input operation is + or -
bool isMinOrPlus(string exp) {
    if (exp[0] == '+' || exp[0] == '-')return true;
    return false;
}

//determies wheather input is '/' or *
bool isMultOrDiv(string exp) {
    if (exp[0] == '/' || exp[0] == '*') return true;
    return false;
}

//does multiplication or division
void multOrDiv(string exp, string exp2, double &a, double &b) {
    cin >> exp2;
    if (!(exp2[0] == '(')) {
        if (exp[0] == '*') b = b * toInt(exp2);
        if (exp[0] == '/') {
            if (toInt(exp2) == 0) {
                cout << "undefined" << endl;
                return;
            }
            else b = b / toInt(exp2); 
        }
    }
    else {
        if (exp[0] == '*')  b = b * calculate();
        else if (exp[0] == '/') b = b / calculate();
    }

}
//does addition or substraction
void minusOrPlus(string exp, string exp2, double& a, double& b, string &op) {
    if (op[0] == '+') a = a + b;
    else if (op[0] == '-') a = a - b;

    op = "";
    op.append(1, exp[0]);

    cin >> exp2;
    if (!(exp2[0] == '(')) b = toInt(exp2);
    else {
        b = b / calculate();
    }
}
double calculate() {
    string exp = "0";
    string exp2 = "0";
    string op = "+";
    double a = 0;
    double b = 0;
    string temp = "";


    cin >> temp;
    if (temp[0] != '(') b = toInt(temp);
    else b = calculate();

    while (true) {
        cin >> exp;

        if (exp[0] == ')' || exp[0] == '=') {
            if (op[0] == '+') return a + b;
            if (op[0] == '-') return a - b;
        }
        // * / case
        if (isMultOrDiv(exp)) multOrDiv(exp, exp2, a, b);
        // case + - 
        else if (isMinOrPlus(exp)) minusOrPlus(exp, exp2, a, b, op);
    }
}

int main() {
    double result =  calculate();
    cout <<result << endl;
    
    return 0;
}
