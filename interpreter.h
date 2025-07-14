#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include <map>

struct Token {
    enum Type { NUMBER, OPERATOR, VARIABLE, FUNCTION } type;
    std::string value;
    Token(Type t, const std::string& v) : type(t), value(v) {}
};

class Interpreter {
private:
    std::map<std::string, double> vars;
    char op;
    std::vector<Token> tokenize(const std::string& input);
    double evaluateExpression(const std::vector<Token>& tokens, int& pos);
    double evaluateFunction(const std::string& func, const std::vector<Token>& args, int& pos);

public:
    Interpreter();
    double evaluateInput(const std::string& input);
};

#endif