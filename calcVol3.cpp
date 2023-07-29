#include <iostream>

using namespace std;

// Structure for a linked list node
struct Node { 
    char value; //saves operation or digit
    Node* next; 
};

// Linked list class
class LinkedList {
private:
    Node* head;
    //Node* tail;

public:
    LinkedList() {
        head = NULL;
        //tail = NULL;
    }

    // Function to add a node at the end of the linked list
    void addNode(char value) {
        Node* newNode = new Node;
        newNode->value = value;
        newNode->next = NULL;

        if (!head) head = newNode;
        else {
            Node* iter = head;
            while (iter->next) iter = iter->next;

            iter->next = newNode;
        }
    }
    //returns head of the list
    Node* getHead() {
        return head;
    }
};

class Calculator {
private:
    LinkedList expression;

    //case pluse or subtract
    double prim(Node*& node) {
        double res = Term(node);
        while (node && (node->value == '-' || node->value == '+')) {
            char curOp = node->value;
            node = node->next;
            double primary = Term(node);
            res = performOperation(res, primary, curOp);
        }
        return res;
    }

    //case multiplication and division
    double Term(Node*& node) {
        double res = expr(node);
        while (node && (node->value == '*' || node->value == '/')) {
            char curOp = node->value;
            node = node->next;
            double term = expr(node);
            res = performOperation(res, term, curOp);
        }
        return res;
    }

    //parse ()
    double expr(Node*& node) {
        double res = 0;
        if (!node) throw runtime_error("runtime error");
        if (node->value == '(') {
            node = node->next;
            res = prim(node);
            if (!node || node->value != ')') throw runtime_error("no matching parentheses ");
            node = node->next;
        }
        else if (isdigit(node->value)) {
            while (node && isdigit(node->value)) {
                res = res * 10 + (node->value - '0');
                node = node->next;
            }
        }
        else {
            throw runtime_error("enter correct expression");
        }
        return res;

    }

    // Function to perform arithmetic operations
    double performOperation(double num1, double num2, char operation) {
        switch (operation) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (num2 != 0) return num1 / num2;
            else {
                cout<<"Undefined" << endl;
                break;
            }
        }

        return 0;
    }
public:
    // Function to write expression into the linked list
    void addOp(char ch) {
        expression.addNode(ch);
    }

    // Function to start calculations
    double calculate(char expre) {
        Node* head = expression.getHead();
        return prim(head);
    }
};


int main() {
    Calculator calculator;

    string expre;
    cout << "Enter expression with integers (I can't deal with doubles :// ):"<<endl;
    cin >> expre;

    // Add each character to the calculator's expression
    for (char ch : expre)  calculator.addOp(ch);

    double result = calculator.calculate(expre[0]);
    cout <<  result << endl;

    return 0;
}
