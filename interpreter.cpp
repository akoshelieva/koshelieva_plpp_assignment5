#include "interpreter.h"
#include "functions.h"
#include "variable.h"
#include <sstream>
#include <cctype>
#include <stdexcept>
using namespace mathlib;

Interpreter::Interpreter() : op('+') {}

std::vector<Token> Interpreter::tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::string token;
    for (char c : input) {
        if (std::isspace(c)) {
            if (!token.empty()) {
                Token::Type type = std::isdigit(token[0]) ? Token::NUMBER : Token::VARIABLE;
                tokens.emplace_back(type, token);
                token.clear();
            }
            continue;
        }
        if (std::isdigit(c) || c == '.' || std::isalpha(c)) {
            token += c;
            continue;
        }
        if (!token.empty()) {
            Token::Type type = std::isdigit(token[0]) ? Token::NUMBER : Token::VARIABLE;
            tokens.emplace_back(type, token);
            token.clear();
        }
        switch (c) {
            case '+': case '-': case '*': case '/': case '=': case ',':
                tokens.emplace_back(Token::OPERATOR, std::string(1, c));
                break;
            /*case '=':
                tokens.emplace_back(Token::OPERATOR, "=");
                break;*/
            case '(':
                tokens.emplace_back(Token::LPAREN, "(");
                break;
            case ')':
                tokens.emplace_back(Token::RPAREN, ")");
                break;
            case '{':
                tokens.emplace_back(Token::LBRACE, "{");
                break;
            case '}':
                tokens.emplace_back(Token::RBRACE, "}");
                break;
            /*case ',':
                tokens.emplace_back(Token::OPERATOR, ",");
                break;*/
        }
    }
    if (!token.empty()) {
        Token::Type type = std::isdigit(token[0]) ? Token::NUMBER : Token::VARIABLE;
        tokens.emplace_back(type, token);
    }
    return tokens;
}

double Interpreter::parseExpression(const std::vector<Token>& tokens, int& pos) {
    double value = parseMultiplying(tokens, pos);
    while (pos < tokens.size() &&
           tokens[pos].type == Token::OPERATOR &&
           (tokens[pos].value == "+" || tokens[pos].value == "-")) {
        char op = tokens[pos].value[0];
        pos++;
        double rhs = parseMultiplying(tokens, pos);
        if (op == '+') value += rhs;
        else value -= rhs;
    }
    return value;
}

double Interpreter::parseMultiplying(const std::vector<Token>& tokens, int& pos) {
    double value = parseAdding(tokens, pos);
    while (pos < tokens.size() &&
           tokens[pos].type == Token::OPERATOR &&
           (tokens[pos].value == "*" || tokens[pos].value == "/")) {
        char op = tokens[pos].value[0];
        pos++;
        double a = parseAdding(tokens, pos);
        if (op == '*') value *= a;
        else value /= a;
    }
    return value;
}

double Interpreter::parseAdding(const std::vector<Token>& tokens, int& pos) {
    const Token& t = tokens[pos];

    if (t.type == Token::LPAREN) {
        pos++;
        double v = parseExpression(tokens, pos);
        if (pos < tokens.size() && tokens[pos].type == Token::RPAREN) pos++;
        return v;
    }

    if (t.type == Token::VARIABLE && pos + 1 < tokens.size() && tokens[pos + 1].type == Token::LPAREN) {
        std::string name = t.value;
        pos += 2;
        std::vector<double> args;
        while (pos < tokens.size() && tokens[pos].value != ")") {
            args.push_back(parseExpression(tokens, pos));
            if (pos < tokens.size() && tokens[pos].value == ",") pos++;
        }
        if (pos < tokens.size() && tokens[pos].value == ")") pos++;
        return evaluateFunction(name, args);
    }

    if (t.type == Token::OPERATOR && t.value == "-") { //унарний мінус і плюс
        pos++;
        return -parseAdding(tokens, pos);
    }
    if (t.type == Token::OPERATOR && t.value == "+") {
        pos++;
        return  parseAdding(tokens, pos);
    }

    if (t.type == Token::NUMBER)   { double v = std::stod(t.value); pos++; return v; }
    if (t.type == Token::VARIABLE) { double v = vars[t.value];      pos++; return v; }

    throw std::runtime_error("Unexpected token");
}

double Interpreter::evaluateFunction(const std::string& f,
                                     const std::vector<double>& a) {
    if (f == "abs") { if (a.size()!=1) throw std::runtime_error("abs needs 1 arg");
        return Abs(a[0]); }
    if (f == "pow") { if (a.size()!=2) throw std::runtime_error("pow needs 2 args");
        return Pow(a[0], a[1]); }
    if (f == "max") { if (a.size()!=2) throw std::runtime_error("max needs 2 args");
        return Max(a[0], a[1]); }
    if (f == "min") { if (a.size()!=2) throw std::runtime_error("min needs 2 args");
        return Min(a[0], a[1]); }

    auto it = userFuncs.find(f);
    if (it == userFuncs.end()) throw std::runtime_error("Unknown function: " + f);
    const FunctionDef& def = it->second;
    if (def.params.size() != a.size())
        throw std::runtime_error("Arg count mismatch in call to " + f);

    std::map<std::string,double> old = vars;
    for (size_t i=0;i<a.size();++i) vars[def.params[i]] = a[i];
    int p = 0;
    double res = parseExpression(def.body, p);
    vars = old;
    return res;
}

double Interpreter::evaluateInput(const std::string& in) {
    std::vector<Token> t = tokenize(in);
    if (t.empty()) return 0;
    int p = 0;

    if (t[0].value == "def") {
        if (t.size() < 5) throw std::runtime_error("Bad def syntax");
        std::string fname = t[1].value;
        p = 3;
        std::vector<std::string> params;
        while (p < t.size() && t[p].value != ")") {
            params.push_back(t[p].value);
            if (t[p+1].value == ",") p += 2; else p++;
        }
        if (t[p].value != ")") throw std::runtime_error("Missing ')'");
        ++p;
        if (t[p].value != "{") throw std::runtime_error("Missing '{'");
        ++p;
        std::vector<Token> body;
        int depth = 1;
        while (p < t.size() && depth) {
            if (t[p].value == "{") ++depth;
            if (t[p].value == "}") --depth;
            if (!depth) break;
            body.push_back(t[p++]);
        }
        userFuncs[fname] = FunctionDef{params, body};
        return 0;
    }

    if (t.size() >= 3 && t[0].value=="var" && t[2].value=="=") {
        std::string v = t[1].value;
        int exprPos = 3;
        double val = parseExpression(t, exprPos);
        vars[v] = val;
        return val;
    }
    return parseExpression(t, p);
}
