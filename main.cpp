#include <iostream>
#include "interpreter.h"

int main() {
    Interpreter interp;
    std::string input;
    while (std::getline(std::cin, input)) {
        double result = interp.evaluateInput(input);
        std::cout << result << std::endl;
    }
    return 0;
}
