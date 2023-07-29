#include <iostream>
#include <map>
#include <sstream>
#include <string>
using namespace std;
// Types
enum class Operation : char {
    name,
    number,
    end,
    plus = '+',
    minus = '-',
    mul = '*',
    div = '/',
    print = ';',
    assign = '=',
    lp = '(',
    rp = ')'
};

struct Token {
    Operation op; // value representing the type of the token
    string strValue; //A string to hold the name of a variable (used for name tokens).
    double NumValue; //A double to hold the numeric value of a token (used for number tokens).
};

class Token_stream {
public:
    Token_stream(istream& s) : ip{ &s }, owns{ false } {}
    /*The class takes ownership of the stream when using the pointer constructor (owns becomes true)*/
    Token_stream(istream* p) : ip{ p }, owns{ true } {} 
   ~Token_stream() { close(); } //The destructor to clean up the input stream if it owns
    Token get();                       // read and return next token
    Token& current() {return currTok; };  // Returns the most recently read token without reading the next one.
    void set_input(istream& s) { close(); ip = &s; owns = false; }
    /*Used to set a new input stream. If the class owns the previous stream, it will close it before setting the new one*/
    void set_input(istream* p) { close(); ip = p; owns = true; }

private: 
    void close() { if (owns) delete ip; }
    istream* ip; //pointer to an input stream
    bool owns; // does the Token_stream own the istream?
    Token currTok{ Operation::end }; // current token
};

// variables
Token_stream TokStr{ cin };  // use input from cin
map<string, double> table; // Map that will hold variable names and their corresponding values.
int noErrors { 0 }; // An integer that keeps track of the number of errors encountered during evaluation.

// Functions
double term(bool get);
double expr(bool get);
double prim(bool get);
double error(const string& s);
void calculate();


double expr(bool get) {  // add and subtract

    double left = term(get);

    for (;;) {
        switch (TokStr.current().op) {
        case Operation::plus:
            left += term(true);
            break;

        case Operation::minus:
            left -= term(true);
            break;
        default:
            return left;
        }
    }
}

double prim(bool get) {  // handle primaries
    if (get) TokStr.get();  // read next token
    switch (TokStr.current().op) {
    case Operation::number: { // floating-point constant
        double v = TokStr.current().NumValue;
        TokStr.get();
        return v;
    }
/*radius = 6378.388;
the calculator will reach case Kind::name and execute
double& v = table["radius"];
// ... expr() calculates the value to be assigned ...
v = 6378.388;*/
                    
    case Operation::name: {
        double& v = table[TokStr.current().strValue];  // find the corresponding
        if (TokStr.get().op == Operation::assign)
            v = expr(true);  // ’=’ seen: assignment
        return v;
    }

    case Operation::minus:  // unary minus
        return -prim(true);

    case Operation::lp: {
        auto e = expr(true);
        if (TokStr.current().op != Operation::rp) return error("')' expected");
        TokStr.get();  // eat ’)’
        return e;
    }
    default:
        return error("primary expected");
    }
}



double term(bool get){  // multiply and divide{
    double left = prim(get); //left variable will hold the intermediate result of the expression.
    for (;;) { //
        switch (TokStr.current().op) {
        case Operation::mul:
            left *= prim(true);
            break;
        case Operation::div:
            if (auto d = prim(true)) {
                left /= d;
                break;
            }
            return error("divide by 0");
        default:
            return left;
        }
    }
}




double error(const string& s) {
    noErrors++;
    cerr << "error: " << s << '\n';
    return 1;
}

Token Token_stream::get() {
    char ch;
   //*ip>>ch;
    do {  // skip whitespace except ’\n’
        if (!ip->get(ch)) return currTok = { Operation::end };
    } while (ch != '\n' && isspace(ch));
    switch (ch) {
    case ';':
    case '\n':
        return currTok = { Operation::print };
    case '*':
    case '/':
    case '+':
    case '-':
    case '(':
    case ')':
    case '=':
        return currTok = { static_cast<Operation>(ch) };
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
        ip->putback(ch);  // put the first digit (or .) back into the input stream
        *ip >> currTok.NumValue;  // read number into ct
        currTok.op = Operation::number;
        return currTok;

    default:  // NAME, NAME=, or error
        if (isalpha(ch)) {
            string string_value{ch};
            while (ip->get(ch) && isalnum(ch))
                string_value += ch;  // append ch to end of string_value
            ip->putback(ch);
            return currTok = { Operation::name, string_value };
        }

        error("bad token");
        return currTok = { Operation::print };
    }
}

void calculate() {
    for (;;) {
        TokStr.get();
        if (TokStr.current().op == Operation::end) break;
        if (TokStr.current().op == Operation::print) continue;
        cout << expr(false) << '\n';
    }
}

int main(int argc, char* argv[]) {
    table["pi"] = 3.1415926535897932385;  // insert predefined names
    table["e"] = 2.7182818284590452354;



    calculate();

    return noErrors;
}