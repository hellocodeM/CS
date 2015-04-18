#include "scanner.hpp"
#include <cassert>
#include <iostream>


void TestScanner() {
    std::cout << "scanner test:" << std::endl;
    CS::Scanner scanner;
    const char* code = "int a = 5\na = 1 + 1\nprintln(a)\n";
    CS::TokenList token_list = scanner.Scan(code);
    for (auto &i : token_list) {
        std::cout << i.first << " : " << i.second <<std::endl;
    }
}

int main()
{
    TestScanner();
    return 0;
}
