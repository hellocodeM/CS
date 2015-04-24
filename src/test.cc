#include <cassert>
#include <iostream>

#ifndef SCANNER
#define SCANNER
#include "scanner.hpp"
#endif

#ifndef PARSER
#define SCANNER
#include "parser.hpp"
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

void DumpSyntaxTree(CS::SyntaxTree* root) {
    if (!root) return;
    std::cout << root->value_ << " : " << root->type_ << std::endl;
    std::cout << "/  \\";
    DumpSyntaxTree(root->left_);
    DumpSyntaxTree(root->right_);
    std::cout << std::endl;
    DumpSyntaxTree(root->next_);
}

void TestParser() {
    std::cout << "parser test:" << std::endl;
    CS::Parser parser;
    const char* code = GetSourceCode();
    CS::TokenList token_list = CS::Scanner::Scan(code);
    CS::SyntaxTree* syntax_tree = parser.Parse(token_list);
    DumpSyntaxTree(syntax_tree);
}

int main()
{
    //TestScanner();
    TestParser();
    return 0;
}
