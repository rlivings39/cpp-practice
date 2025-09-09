/*
Calculator like thing that handles expressions of the form:

  2 + 3*a - 4*xyz - 3

for any integer constants and identifiers. Only +/- are supported.
*/

#include <cctype>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace simple_calc {
enum class TokenKind { Plus, Minus, Mul, IntConst, Id };
struct Token {
  Token(TokenKind aKind, std::string aId, int aCoeff)
      : kind(aKind), id(aId), coeff(aCoeff) {}
  Token(TokenKind aKind) : kind(aKind) {}

  std::string to_string() const {
    std::string res;
    switch (this->kind) {

    case TokenKind::Plus:
      res += "+";
      break;
    case TokenKind::Minus:
      res += "-";
      break;
    case TokenKind::Mul:
      res += "*";
      break;
    case TokenKind::IntConst:
      res += "Int(" + std::to_string(this->coeff) + ")";
      break;
    case TokenKind::Id:
      res += "Id(" + this->id + ")";
      break;
    }

    return res;
  }
  TokenKind kind;
  std::string id = "";
  int coeff = 0;
};
using TokenVec = std::vector<Token>;
struct calc_err : public std::runtime_error {
  calc_err(const std::string &msg) : std::runtime_error(msg) {}
};

void fail(std::string msg, const std::string &input, int idx) {
  std::string pointer_line(idx, ' ');
  pointer_line += "^";
  std::string err = msg + ":\n\n" + input + "\n" + pointer_line + "\n";
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
  id = input.substr(idx, end - idx);
  idx = end;
  return id;
}

TokenVec tokenize(const std::string &input) {
  TokenVec tokens;
  size_t idx{0};
  while (idx < input.size()) {
    auto c = input[idx];
    if (std::isspace(c)) {
      ++idx;
      continue;
    }
    if (c == '+') {
      tokens.emplace_back(TokenKind::Plus);
      ++idx;
    } else if (c == '-') {
      tokens.emplace_back(TokenKind::Minus);
      ++idx;
    } else if (c == '*') {
      tokens.emplace_back(TokenKind::Mul);
      ++idx;
    } else if (std::isdigit(c)) {
      int coeff = read_int(input, idx);
      tokens.emplace_back(TokenKind::IntConst, "0", coeff);
    } else if (std::isalpha(c)) {
      auto id = read_identifier(input, idx);
      tokens.emplace_back(TokenKind::Id, id, 0);
    } else {
      fail(std::string("Unexpected input '") + c + "'", input, idx);
    }
  }
  return tokens;
}

void fails(std::string msg) {
  throw calc_err(msg + "\n");
}

void check_not_end(TokenVec tokens, size_t idx, std::string msg) {
  if (idx >= tokens.size()) {
    fails("Unexpected end of input." + msg);
  }
}

const Token &peek(const TokenVec &tokens, size_t idx) {
  check_not_end(tokens, idx, "");
  return tokens[idx];
}

const Token &expect(const TokenVec &tokens, size_t idx, TokenKind kind) {
  check_not_end(tokens, idx, "");
  auto &&next = tokens[idx];
  if (next.kind != kind) {
    fails("Unexpected token" + next.to_string());
  }
  return next;
}

std::pair<int, std::string> parse_operand(const TokenVec &tokens, size_t &idx) {
  int coeff{0};
  std::string id = "";
  auto &&next = peek(tokens, idx);
  if (next.kind == TokenKind::IntConst) {
    // 123 or 124*id
    coeff = next.coeff;
    ++idx;
    if (idx < tokens.size() && tokens[idx].kind == TokenKind::Mul) {
      ++idx;
      check_not_end(tokens, idx, "");
      auto id_token = expect(tokens, idx, TokenKind::Id);
      id = id_token.id;
      ++idx;
    }
  } else {
    // id
    auto id_token = expect(tokens, idx, TokenKind::Id);
    id = id_token.id;
    coeff = 1;
    ++idx;
  }
  return {coeff, id};
}

std::string simplify(const std::string &input) {
  // Use map so variables are sorted by name for stable output
  std::map<std::string, int> var_table;
  auto tokens = tokenize(input);
  std::cout << "Tokens [\n";
  for (const auto &token : tokens) {
    std::cout << token.to_string() + ",\n";
  }
  std::cout << "]\n";

  if (tokens.empty()) {
    return "";
  }

  // Input must start with an operand
  int coeff;
  std::string id;
  size_t idx{0};
  std::tie(coeff, id) = parse_operand(tokens, idx);
  var_table[id] += coeff;
  // If any tokens remain, continue parsing an operator followed by an operand
  while (idx < tokens.size()) {
    auto &&token = tokens[idx];
    int sign = 1;
    switch (token.kind) {
    case TokenKind::Plus:
      sign = 1;
      break;
    case TokenKind::Minus:
      sign = -1;
      break;
    default:
      fails("Expected + or - found " + token.to_string());
      break;
    }
    // Consume operator
    ++idx;
    std::tie(coeff, id) = parse_operand(tokens, idx);
    var_table[id] += sign * coeff;
    // TODO always finish?
  }

  std::string res;
  for (const auto &entry : var_table) {
    std::tie(id, coeff) = entry;
    if (coeff == 0) {
      continue;
    }
    if (!res.empty()) {
      res += coeff >= 0 ? " + " : " - ";
    }
    if (id != "") {
      if (coeff != 1) {
        res += std::to_string(std::abs(coeff));
        res += "*" + id;
      } else {
        res += id;
      }
    } else {
      res += std::to_string(std::abs(coeff));
    }
  }
  return res;
}

} // namespace simple_calc

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
      auto result = simple_calc::simplify(prompt);
      std::cout << "  " << result << "\n";
    } catch (simple_calc::calc_err &e) {
      std::cerr << e.what();
    }
  }
  return 0;
}
