#include <cassert>
#include <iostream>

#include "scanner.hpp"
#include "parser.hpp"
#include "variable.hpp"
#include "evaluator.hpp"

const char* GetSourceCode() {
    const char* code = "int a;\na = 1 + 1;\n println(a);\n";
    return code;
}

void TestScanner() {
    printf("scanner test:\n");
    const char* code = GetSourceCode();
    CS::TokenList token_list = CS::Scanner::Scan(code);
    for (auto &i : token_list) {
        std::cout << i.first << " : " << i.second <<std::endl;
    }
    printf("pass: scanner test\n");
}

void DumpSyntaxTree(CS::SyntaxTree* root, int interval) {
    if (!root) return;
    printf("%*s", interval, "");
    std::string line = "->";
    if (!root->value_.empty())
        line += root->value_;
    else if (root->type_ == CS::GetId("call_type"))
        line += "call";
    line += ":" +std::to_string(root->type_);
    printf("%-10s", line.c_str());
        DumpSyntaxTree(root->left_, 0);

    printf("\n");
        DumpSyntaxTree(root->right_, interval + 10);
    DumpSyntaxTree(root->next_, interval);
}

void TestParser() {
    printf("parser test:\n");
    const char* code = GetSourceCode();
    CS::TokenList token_list = CS::Scanner::Scan(code);
    CS::Parser parser;
    CS::SyntaxTree* syntax_tree = parser.Parse(token_list);
    DumpSyntaxTree(syntax_tree, 0);
    printf("pass: parser test\n");
}

void TestVariable() {
    printf("variable test\n");
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
    printf("pass: variable test\n");
}

void TestEvaluator() {
    printf("evaluator test\n");
    CS::Evaluator eval;
    std::cout << eval.Evaluate("int a;\n") << std::endl;
    std::cout << eval.Evaluate("a = 1+1;\n") << std::endl;
    std::cout << eval.Evaluate("println(a);\n") << std::endl;
    printf("pass: evaluator test\n");
}

int main()
{
    TestScanner();
    TestParser();
    //TestVariable();
    TestEvaluator();
    return 0;
}
