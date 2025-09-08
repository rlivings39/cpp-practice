/*
Calculator like thing that handles expressions of the form:

  2 + 3*a - 4*xyz - 3

for any integer constants and identifiers. Only +/- are supported.
*/

#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

struct calc_err : public std::runtime_error {
  calc_err(const std::string &msg) : std::runtime_error(msg) {}
};

std::string simplify(const std::string &input) {
  std::unordered_map<std::string, int> var_table;

  std::size_t idx{0};
  int sign{1};
  while (idx < input.size()) {
    auto c = input[idx];
    if (std::isspace(c)) {
      ++idx;
      continue;
    }
    // Start lexing and parsing
    if (c == '+') {
      sign = 1;
    } else if (c == '-') {
      sign = -1;
    } else if (std::isdigit(c)) {
      // Parse number
    } else if (std::isalpha(c)) {
      // Parse identifier
    } else {
      std::string msg(idx, ' ');
      msg += "^";
      std::string err = std::string("Failed to parse '") + c + "':\n\n" +
                        input + "\n" + msg + "\n";
      throw calc_err(err);
    }
    ++idx;
  }
  return input;
}

int main(int argc, char *argv[]) {
  std::string prompt;
  std::cout << "Enter an expression of the form 2 + 3*a - 4*xyz - 3. Ctrl+C to "
               "quit.\n";
  while (true) {
    std::cout << ">> ";
    std::flush(std::cout);
    std::getline(std::cin, prompt);
    // TODO trim
    if (prompt == "quit") {
      break;
    }
    try {
      auto result = simplify(prompt);
      std::cout << "  " << result << "\n";
    } catch (calc_err &e) {
      std::cerr << e.what();
    }
  }
  return 0;
}
