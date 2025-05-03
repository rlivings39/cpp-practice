#include "matrix_ops.hpp"
#include <iostream>
#include <random>

int main (int argc, char* argv[]) {
    constexpr size_t n = 102;
    std::vector<double> a(n*n);
    for (size_t k = 0; k < a.size(); ++k) {
        a[k] = k;
    }

    auto res = matrix_multiply(a, n, n, a, n, n);

    return res[12] == 0;
}
