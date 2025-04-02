#pragma "once"

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Print the size of a matrix in a nice way
 *
 * @param nrows
 * @param ncols
 * @return std::string
 */
std::string print_size(std::size_t nrows, std::size_t ncols);

/**
 * @brief Naive implementation of matrix multiplication for column-major
 * matrices
 *
 * @tparam T
 * @param a
 * @param anrows
 * @param ancols
 * @param b
 * @param bnrows
 * @param bncols
 * @return std::vector<T>
 */
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
  for (std::size_t outcol = 0; outcol < outNcols; ++outcol) {
    for (std::size_t outrow = 0; outrow < outNrows; ++outrow) {
      T val{0};
      for (std::size_t k = 0; k < innerDim; ++k) {
        val += a[outrow + k * anrows] * b[k + outcol * bnrows];
      }
      res[outrow + outcol * outNrows] = val;
    }
  }
  return res;
}

/**
 * @brief Render the column-major data in matrix as an nrows x ncols matrix
 *
 * @tparam T
 * @param matrix
 * @param nrows
 * @param ncols
 * @return auto
 */
template <typename T>
auto matrix_to_string(T &&matrix, std::size_t nrows, std::size_t ncols) {
  std::stringstream ress;
  for (std::size_t row = 0; row < nrows; ++row) {
    ress << "\t";
    for (std::size_t col = 0; col < ncols; ++col) {
      ress << matrix[col * nrows + row] << "\t";
    }
    ress << "\n";
  }
  std::string res = ress.str();
  return res;
}
