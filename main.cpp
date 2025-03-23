#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

void help() {
  std::cout << "Hello, from cpp-practice!\n";
}

/**
 * @brief Simple matrix type
 *
 * @tparam T
 */
template <typename T> class Matrix {
public:
  Matrix(std::size_t nrows, std::size_t ncols)
      : fNrows(nrows), fNcols(ncols), fData(nrows * ncols) {}

  std::size_t numel() {
    return fNrows * fNcols;
  }
  std::vector<T> &data() {
    return this->fData;
  }

private:
  std::size_t fNrows;
  std::size_t fNcols;
  std::vector<T> fData;
};
/**
 * @brief Create a matrix of type T with size nrows x ncols
 *
 * @tparam T
 * @param nrows
 * @param ncols
 * @return decltype(auto)
 */
template <typename T>
decltype(auto) make_matrix(std::size_t nrows, std::size_t ncols) {
  auto res = std::vector<T>(nrows * ncols);
  return res;
}

template <typename T>
auto matrix_to_string(T &&matrix, std::size_t nrows, std::size_t ncols) {
  std::stringstream ress;
  for (std::size_t row = 0; row < nrows; ++row) {
    ress << "\t";
    for (std::size_t col = 0; col < ncols; ++col) {
      ress << matrix[row * ncols + col] << "\t";
    }
    ress << "\n";
  }
  std::string res = ress.str();
  return res;
}

std::string print_size(std::size_t nrows, std::size_t ncols) {
  std::string res =
      "(" + std::to_string(nrows) + " x " + std::to_string(ncols) + ")";
  return res;
}

template <typename T>
std::vector<T> matrix_multiply(const std::vector<T> &a, std::size_t anrows,
                               std::size_t ancols, const std::vector<T> &b,
                               std::size_t bnrows, std::size_t bncols) {
  std::size_t outNrows = anrows, outNcols = bncols, innerDim = ancols;
  std::vector<T> res(outNrows * outNcols);
  if (ancols != bnrows) {
    throw std::runtime_error(
        "Inner dimension size mismatch: " + print_size(anrows, ancols) + " * " +
        print_size(bnrows, bncols));
  }

  // Sizes are good. Multiply.
  for (std::size_t outrow = 0; outrow < outNrows; ++outrow) {
    for (std::size_t outcol = 0; outcol < outNcols; ++outcol) {
      T val{0};
      for (std::size_t k = 0; k < innerDim; ++k) {
        val += a[outrow * ancols + k]*b[k*bnrows + outcol];
      }
      res[outrow * outNcols + outcol] = val;
    }
  }
  return res;
}

int main(int, char **) {
  std::size_t aNrows = 3, aNcols = 3;
  std::vector<double> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  std::size_t bNrows = 3, bNcols = 3;
  std::vector<double> b = {1, 0, 0, 0, 1, 0, 0, 0, 1};

  auto x = make_matrix<double>(2, 3);

  std::cout << "a =\n" << matrix_to_string(a, aNrows, aNcols);
  std::cout << "b =\n" << matrix_to_string(b, bNrows, bNcols);

  auto c = matrix_multiply(a, 3, 3, b, 3, 2);

  std::cout << "c = \n" << matrix_to_string(c, 3, 2);
}
