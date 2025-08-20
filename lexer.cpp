#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include <gtest/gtest.h>

namespace ry {
enum class TokenKind {
  KwInt,
  KwVoid,
  KwReturn,
  KwIf,
  KwElse,
  LeftParen,
  RightParen,
  LeftCurly,
  RightCurly,
  LeftSquare,
  RightSquare,
  Semicolon,
  Identifier,
  IntConstant,
  UnaryBitwiseComplement,
  LogicalNot,
  Decrement,
  Minus,
  Plus,
  Times,
  Divide,
  Remainder,
  And,
  Or,
  Less,
  LessEq,
  Greater,
  GreaterEq,
  Equal,
  NotEqual,
  Assign,
  Question,
  Colon,
  Eof,
  Bogus,
};

std::string kind_to_str(TokenKind kind) {
  switch (kind) {

  case TokenKind::KwInt:
    return "KwInt";
  case TokenKind::KwVoid:
    return "KwVoid";
  case TokenKind::KwReturn:
    return "KwReturn";
  case TokenKind::KwIf:
    return "KwIf";
  case TokenKind::KwElse:
    return "KwElse";
  case TokenKind::LeftParen:
    return "LeftParen";
  case TokenKind::RightParen:
    return "RightParen";
  case TokenKind::LeftCurly:
    return "LeftCurly";
  case TokenKind::RightCurly:
    return "RightCurly";
  case TokenKind::LeftSquare:
    return "LeftSquare";
  case TokenKind::RightSquare:
    return "RightSquare";
  case TokenKind::Semicolon:
    return "Semicolon";
  case TokenKind::Identifier:
    return "Identifier";
  case TokenKind::IntConstant:
    return "IntConstant";
  case TokenKind::UnaryBitwiseComplement:
    return "UnaryBitwiseComplement";
  case TokenKind::LogicalNot:
    return "LogicalNot";
  case TokenKind::Decrement:
    return "Decrement";
  case TokenKind::Minus:
    return "Minus";
  case TokenKind::Plus:
    return "Plus";
  case TokenKind::Times:
    return "Times";
  case TokenKind::Divide:
    return "Divide";
  case TokenKind::Remainder:
    return "Remainder";
  case TokenKind::And:
    return "And";
  case TokenKind::Or:
    return "Or";
  case TokenKind::Less:
    return "Less";
  case TokenKind::LessEq:
    return "LessEq";
  case TokenKind::Greater:
    return "Greater";
  case TokenKind::GreaterEq:
    return "GreaterEq";
  case TokenKind::Equal:
    return "Equal";
  case TokenKind::NotEqual:
    return "NotEqual";
  case TokenKind::Assign:
    return "Assign";
  case TokenKind::Question:
    return "Question";
  case TokenKind::Colon:
    return "Colon";
  case TokenKind::Eof:
    return "Eof";
  case TokenKind::Bogus:
    return "Bogus";
  }
  return "";
}

/**
 * @brief Allow Google test to pretty print TokenKind values
 *
 * @param kind
 * @param os
 */
void PrintTo(const TokenKind &kind, std::ostream *os) {
  *os << kind_to_str(kind);
}

using TokenData = std::variant<int, std::string, std::monostate>;

struct Token {
  Token(TokenKind kind, std::size_t text_start, std::size_t text_end,
        TokenData data)
      : kind(kind), text_start(text_start), text_end(text_end), data(data) {}
  TokenKind kind;
  std::size_t text_start;
  std::size_t text_end;
  TokenData data;
};

// TODO why do I need extra braces?
const std::array<std::pair<std::string, TokenKind>, 33> PATTERN_MAP{{
    {"^int\\b", TokenKind::KwInt},
    {"^void\\b", TokenKind::KwVoid},
    {"^return\\b", TokenKind::KwReturn},
    {"^if\\b", TokenKind::KwIf},
    {"^else\\b", TokenKind::KwElse},
    {"^\\(", TokenKind::LeftParen},
    {"^\\)", TokenKind::RightParen},
    {"^\\{", TokenKind::LeftCurly},
    {"^\\}", TokenKind::RightCurly},
    {"^\\[", TokenKind::LeftSquare},
    {"^\\]", TokenKind::RightSquare},
    {"^;", TokenKind::Semicolon},
    {"^[a-zA-z_]\\w*\\b", TokenKind::Identifier},
    {"^[0-9]+\\b", TokenKind::IntConstant},
    {"^~", TokenKind::UnaryBitwiseComplement},
    {"^!", TokenKind::LogicalNot},
    {"^--", TokenKind::Decrement},
    {"^\\-", TokenKind::Minus},
    {"^\\+", TokenKind::Plus},
    {"^\\*", TokenKind::Times},
    {"^/", TokenKind::Divide},
    {"^%", TokenKind::Remainder},
    {"^&&", TokenKind::And},
    {"^\\|\\|", TokenKind::Or},
    {"^<", TokenKind::Less},
    {"^<=", TokenKind::LessEq},
    {"^>", TokenKind::Greater},
    {"^>=", TokenKind::GreaterEq},
    {"^==", TokenKind::Equal},
    {"^!=", TokenKind::NotEqual},
    {"^=", TokenKind::Assign},
    {"^\\?", TokenKind::Question},
    {"^:", TokenKind::Colon},
}};

std::vector<Token> lex(std::string code) {
  std::vector<Token> tokens;
  std::size_t idx{0};

  while (idx < code.size()) {
    auto it = code.cbegin() + idx;
    auto c = *it;
    // Skip whitespace
    if (std::isspace(c)) {
      ++idx;
      continue;
    }
    auto match_kind{TokenKind::Bogus};
    std::string match_str = "";

    for (auto [pattern, kind] : PATTERN_MAP) {
      auto re = std::regex(pattern);
      std::smatch m;
      if (std::regex_search(it, code.cend(), m, re) &&
          m.length() > match_str.size()) {
        match_str = m.str();
        match_kind = kind;
      }
    }
    if (match_kind == TokenKind::Bogus) {
      throw new std::runtime_error(
          std::string("Failed to lex: ")
              .append(std::string_view(it, code.end())));
    }
    TokenData token_data{std::monostate()};
    switch (match_kind) {
    case TokenKind::IntConstant: {
      token_data = std::atoi(match_str.c_str());
      break;
    }
    case TokenKind::Identifier: {
      token_data = match_str;
      break;
    }
    default: {
      break;
    }
    }
    tokens.emplace_back(match_kind, idx, idx + match_str.size(), token_data);
    idx += match_str.size();
  }
  tokens.emplace_back(TokenKind::Eof, idx, idx, TokenData{std::monostate()});
  return tokens;
}
TEST(Lex, BasicMain) {
  auto main = R"(
  int main(void) {
    int x = 3;
    if x > 4
      return 0;
    else
      return 5;
  }
)";
  auto tokens = lex(main);
  std::string diagnostic = "Unexpected tokens of kind [\n";
  std::for_each(tokens.cbegin(), tokens.cend(), [&diagnostic](auto token) {
    diagnostic.append("  " + kind_to_str(token.kind) + ",\n");
  });
  diagnostic += "\n]";
  {
    EXPECT_EQ(tokens.size(), 24) << diagnostic;
    auto main_id_token = tokens[1];
    auto main_token_data = main_id_token.data;
    ASSERT_EQ(main_id_token.kind, TokenKind::Identifier);

    ASSERT_TRUE(std::holds_alternative<std::string>(main_token_data));
    ASSERT_EQ(main_token_data.index(), 1);
    auto main_name = std::get<1>(main_id_token.data);
    EXPECT_EQ(main_name, "main");
  }

  {
    auto int_token = tokens[0];
    ASSERT_EQ(int_token.kind, TokenKind::KwInt);
    auto int_data = int_token.data;
    ASSERT_TRUE(std::holds_alternative<std::monostate>(int_data));
    ASSERT_EQ(int_data.index(), 2);
    EXPECT_EQ(std::get<2>(int_data), std::monostate());
  }
  {
    auto int_const_token = tokens[9];
    ASSERT_EQ(int_const_token.kind, TokenKind::IntConstant);
    auto int_const_data = int_const_token.data;
    ASSERT_TRUE(std::holds_alternative<int>(int_const_data));
    ASSERT_EQ(int_const_data.index(), 0);
    EXPECT_EQ(std::get<0>(int_const_data), 3);
  }
}
} // namespace ry
