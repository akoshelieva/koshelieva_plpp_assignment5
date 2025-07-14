#include "functions.h"
#include <cmath>
namespace mathlib {
    double Pow(double base, double exp) { return std::pow(base, exp); }
    double Abs(double x)                { return std::abs(x);        }
    double Max(double a, double b)      { return (a > b) ? a : b;    }
    double Min(double a, double b)      { return (a < b) ? a : b;    }
}
