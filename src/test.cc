#include <cassert>
#include <iostream>

#ifndef SCANNER_H
#include "scanner.hpp"
#endif

#ifndef PARSER_H
#include "parser.hpp"
#endif

#ifndef VARIABLE_H
#include "variable.hpp"
#endif

const char* GetSourceCode() {
    const char* code = "int a;\na = 1 + 1;\n println(a);\n";
    return code;
}

void TestScanner() {
    std::cout << "scanner test:" << std::endl;
    const char* code = GetSourceCode();
    CS::TokenList token_list = CS::Scanner::Scan(code);
    for (auto &i : token_list) {
        std::cout << i.first << " : " << i.second <<std::endl;
    }
}

void DumpSyntaxTree(CS::SyntaxTree* root, int interval) {
    if (!root) return;
    printf("%*s", interval, "");
    std::string line = "->";
    if (!root->value_.empty())
        line += root->value_;
    else if (root->type_ == 54)
        line += "call";
    printf("%-10s", line.c_str());
        DumpSyntaxTree(root->left_, 0);

    printf("\n");
        DumpSyntaxTree(root->right_, interval + 10);
    DumpSyntaxTree(root->next_, interval);
}

void TestParser() {
    std::cout << "parser test:" << std::endl;
    const char* code = GetSourceCode();
    CS::TokenList token_list = CS::Scanner::Scan(code);
    CS::Parser parser;
    CS::SyntaxTree* syntax_tree = parser.Parse(token_list);
    DumpSyntaxTree(syntax_tree, 0);
}

void TestVariable() {
    CS::Variable a(1);
    CS::Variable b(1.0);
    CS::Variable c(2);
    CS::Variable p(static_cast<void*>(&a));
    // int and double
    std::cout << (a + b).to_string() << std::endl;
    std::cout << (a - b).to_string() << std::endl;
    std::cout << (a * b).to_string() << std::endl;
    std::cout << (a / b).to_string() << std::endl;
    // int and int 
    std::cout << (a + c).to_string() << std::endl;
    // int and pointer
    std::cout << (a + p).to_string() << std::endl;
}

int main()
{
    //TestScanner();
    //TestParser();
    TestVariable();
    return 0;
}
