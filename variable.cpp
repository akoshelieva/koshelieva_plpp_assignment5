#include "variable.h"

void Variables::set(const std::string& name, double value) {
    vars[name] = value;
}
double Variables::get(const std::string& name) const {
    auto it = vars.find(name);
    return (it != vars.end()) ? it->second : 0.0;
}

