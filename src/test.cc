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

int main()
{
    //TestScanner();
    TestParser();
    return 0;
}
