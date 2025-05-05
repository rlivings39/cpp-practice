#include "matrix_ops.hpp"
#include <chrono>
#include <iostream>
#include <random>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage " << argv[0] << " num_rows\n";
    return 1;
  }
  std::size_t n = std::stoll(argv[1]);
  std::vector<double> a(n * n);
  for (size_t k = 0; k < a.size(); ++k) {
    a[k] = k;
  }

  auto start = std::chrono::steady_clock::now();
  auto res = matrix_multiply(a, n, n, a, n, n);
  auto end = std::chrono::steady_clock::now();

  std::cout << "Elapsed time for size (" << n << "," << n << "): "
            << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                     start)
            << ".\n";

  return res[12] == 0;
}
