#include <vector>

#include "matrix_ops.hpp"
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

std::string print_size(std::size_t nrows, std::size_t ncols) {
  std::string res =
      "(" + std::to_string(nrows) + " x " + std::to_string(ncols) + ")";
  return res;
}
