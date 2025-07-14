#include "interpreter.h"
#include "functions.h"
#include "variable.h"
#include <sstream>
#include <cctype>
#include <stdexcept>

Interpreter::Interpreter() : op('+') {}

std::vector<Token> Interpreter::tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::string token;
    for (char c : input) {
        if (std::isspace(c)) {
            if (!token.empty()) {
                tokens.emplace_back(std::isdigit(token[0]) ? Token::NUMBER : Token::VARIABLE, token);
                token.clear();
            }
            continue;
        }
        if (std::isdigit(c) || c == '.' || isalpha(c)) {
            token += c;
        } else {
            if (!token.empty()) {
                tokens.emplace_back(
                    std::isdigit(token[0]) ? Token::NUMBER :
                    (isalpha(token[0]) && token.find('(') != std::string::npos) ? Token::FUNCTION : Token::VARIABLE,
                    token);
                token.clear();
            }
            if (c == '+' || c == '-' || c == '*' || c == '/') {
                tokens.emplace_back(Token::OPERATOR, std::string(1, c));
            }
        }
    }
    if (!token.empty()) {
        tokens.emplace_back(std::isdigit(token[0]) ? Token::NUMBER : Token::VARIABLE, token);
    }
    return tokens;
}

double Interpreter::evaluateExpression(const std::vector<Token>& tokens, int& pos) {
    double result = 0;
    while (pos < tokens.size()) {
        const Token& token = tokens[pos];
        if (token.type == Token::NUMBER) {
            double value = std::stod(token.value);
            result = (op == '+') ? result + value : result - value;
        } else if (token.type == Token::OPERATOR) {
            op = token.value[0];
        } else if (token.type == Token::VARIABLE) {
            result = vars[token.value];
        } else if (token.type == Token::FUNCTION) {
            pos++;
            std::vector<Token> args;
            while (pos < tokens.size() && tokens[pos].value != ")") {
                args.push_back(tokens[pos]);
                pos++;
            }
            int argPos = 0;
            result = evaluateFunction(token.value, args, argPos);
        } else if (token.value == ")") {
            break;
        }
        pos++;
    }
    return result;
}

double Interpreter::evaluateFunction(const std::string& func, const std::vector<Token>& args, int& pos) {
    if (args.size() < 2) throw std::runtime_error("Function requires 2 arguments");
    double arg1 = std::stod(args[0].value);
    double arg2 = std::stod(args[1].value);
    if (func == "pow") return pow(arg1, arg2);
    if (func == "abs") return abs(arg1);
    if (func == "max") return max(arg1, arg2);
    if (func == "min") return min(arg1, arg2);
    throw std::runtime_error("Unknown function: " + func);
}

double Interpreter::evaluateInput(const std::string& input) {
    std::vector<Token> tokens = tokenize(input);
    int pos = 0;
    if (tokens.empty()) return 0;

    if (tokens.size() >= 3 && tokens[0].value == "var" && tokens[2].value == "=") {
        std::string varName = tokens[1].value;
        int exprPos = 3;
        double value = evaluateExpression(tokens, exprPos);
        vars[varName] = value;
        return value;
    }

    return evaluateExpression(tokens, pos);
}