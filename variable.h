

#ifndef VARIABLE_H
#define VARIABLE_H
#include <map>
#include <string>

class Variables {
public:
    void set(const std::string& name, double value);
    double get(const std::string& name) const;
private:
    std::map<std::string, double> vars;
};




#endif //VARIABLE_H
