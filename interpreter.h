#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include <map>

struct Token {
    enum Type { NUMBER, OPERATOR, VARIABLE, FUNCTION, LPAREN, RPAREN, LBRACE, RBRACE } type;
    std::string value;
    Token(Type t, const std::string& v) : type(t), value(v) {}
};
struct FunctionDef {
    std::vector<std::string> params;
    std::vector<Token>       body;
};

class Interpreter {
private:
    std::map<std::string,double> vars;
    std::map<std::string,FunctionDef> userFuncs;
    char op;
    std::vector<Token> tokenize(const std::string& input);
    // double evaluateExpression(const std::vector<Token>& tokens, int& pos);

    double parseExpression(const std::vector<Token> &tokens, int &pos);

    double parseMultiplying(const std::vector<Token> &tokens, int &pos);

    double parseAdding(const std::vector<Token> &tokens, int &pos);

    double evaluateFunction(const std::string &func, const std::vector<double> &args);

public:
    Interpreter();
    double evaluateInput(const std::string& input);
};

#endif