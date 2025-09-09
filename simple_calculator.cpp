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
#include <vector>

namespace simple_calc {
enum class TokenKind { Plus, Minus, Mul, IntConst, Id };
struct Token {
  Token(TokenKind aKind, std::string aId, int aCoeff)
      : kind(aKind), id(aId), coeff(aCoeff) {}
  Token(TokenKind aKind) : kind(aKind) {}
  TokenKind kind;
  std::string id = "";
  int coeff = 0;
};

struct calc_err : public std::runtime_error {
  calc_err(const std::string &msg) : std::runtime_error(msg) {}
};

void fail(std::string msg, const std::string &input, int idx) {
  std::string pointer_line(idx, ' ');
  pointer_line += "^";
  std::string err = msg + "':\n\n" + input + "\n" + pointer_line + "\n";
  throw calc_err(err);
}

int read_int(const std::string &input, size_t &idx) {
  size_t end{idx};
  while (end < input.size() && std::isdigit(input[end])) {
    ++end;
  }
  if (end == idx) {
    fail(std::string("Expected an int constant found '") + input[idx] + "'",
         input, idx);
  }
  int res = std::atoi(input.substr(idx, end - idx).c_str());
  idx = end;
  return res;
}

std::string read_identifier(const std::string &input, size_t &idx) {
  std::string id{""};
  size_t end{idx};
  while (end < input.size() && std::isalpha(input[end])) {
    ++end;
  }
  if (end == idx) {
    fail(std::string("Expected identifier. Found '") + input[end] + "'", input,
         idx);
  }
  return "";
}

std::vector<Token> tokenize(const std::string &input) {
  std::vector<Token> tokens;
  size_t idx{0};
  while (idx < input.size()) {
    auto c = input[idx];
    if (std::isspace(c)) {
      ++idx;
      continue;
    }
    if (c == '+') {
      tokens.emplace_back(TokenKind::Plus);
    } else if (c == '-') {
      tokens.emplace_back(TokenKind::Minus);
    } else if (c == '*') {
      tokens.emplace_back(TokenKind::Mul);
    } else if (std::isdigit(c)) {
      int coeff = read_int(input, idx);
      tokens.emplace_back(TokenKind::IntConst, "0", coeff);
    } else if (std::isalpha(c)) {
      auto id = read_identifier(input, idx);
      tokens.emplace_back(TokenKind::Id, id, 0);
    } else {
      fail("Unexpected token.", input, idx);
    }
  }
  return tokens;
}

void check_end(std::string msg, const std::string &input, size_t idx) {
  if (idx >= input.size()) {
    fail(msg, input, idx);
  }
}

// std::pair<int, std::string> parse_id_and_coeff(const std::string &input,
//                                                size_t &idx) {
//   check_end("Expected an operand found end of string.", input, idx);
//   auto c = input[idx];
//   int coeff{0};
//   if (std::isdigit(c)) {
//     coeff = parse_int(input, idx);
//   }
// }

void expect(const std::string &input, size_t &idx, char c) {
  check_end(std::string("Found end of string. Expected '") + c + "'", input,
            idx);
  if (input[idx] != c) {
    fail(std::string("Expected '") + c + "'. Found '" + input[idx] + "'", input,
         idx);
  }
  ++idx;
}

bool skip_whitespace(const std::string &input, size_t &idx) {
  bool skipped = false;
  while (idx < input.size() && std::isspace(input[idx])) {
    ++idx;
    skipped = true;
  }
  return skipped;
}

std::string simplify(const std::string &input) {
  std::unordered_map<std::string, int> var_table;
  auto tokens = tokenize(input);

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
} // namespace simple_calc
