/*
Calculator like thing that handles expressions of the form:

  2 + 3*a - 4*xyz - 3

for any integer constants and identifiers. Only +/- are supported.
*/

#include <iostream>
#include <string>

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
  }
  return 0;
}
