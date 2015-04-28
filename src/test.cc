#include <cassert>
#include <iostream>
#include <memory>

#include "scanner.hpp"
#include "parser.hpp"
#include "variable.hpp"
#include "evaluator.hpp"
#include "opcode.hpp"
#include "encoder.hpp"
#include "serializer.hpp"


using namespace CS;

const char* GetSourceCode() {
    const char* code = "int a;\na = 1 + 1;\n println(a);\n";
    return code;
}

void TestScanner() {
    printf("------scanner test------\n");
    const char* code = GetSourceCode();
    CS::TokenList token_list = CS::Scanner::Scan(code);
    for (auto &i : token_list) {
        std::cout << i.first << " : " << i.second <<std::endl;
    }
    printf("-----pass: scanner test------\n\n");
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
    using namespace CS;
    printf("------parser test------\n");
    const char* code = GetSourceCode();
    CS::TokenList token_list = CS::Scanner::Scan(code);
    CS::Parser parser;
    std::shared_ptr<SyntaxTree> syntax_tree(parser.Parse(token_list));
    DumpSyntaxTree(syntax_tree.get(), 0);
    printf("-----pass: parser test------\n\n");
}

void TestVariable() {
    printf("------variable test------\n");
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
    printf("-----pass: variable test------\n\n");
}

void TestEvaluator() {
    printf("-----evaluator test------\n");
    CS::Evaluator eval;
    std::cout << eval.Evaluate("int a;\n") << std::endl;
    std::cout << eval.Evaluate("a = 1+1;\n") << std::endl;
    std::cout << eval.Evaluate("println(a);\n") << std::endl;
    printf("-----pass: evaluator test------\n\n");
}

void TestOpCode() {
    printf("-----opcode test------\n");
    using namespace CS::OpCode;
    int op = GetOpCode("push");
    int val = 1;
    long long i = MakeOpCode(op, val);
    printf("%016llx\n", i);
    assert(i == 0x0000000100000001);
    auto instruction = SplitOpCode(i);
    assert(instruction.first == GetOpCode("push"));
    assert(instruction.second == 1);
    printf("-----pass: opcode test------\n\n");
}


void TestStackModel() {
    printf("-----stack model test-----\n");
    
    /* int a; a = 1 + 1; println(a); */
    using namespace CS::OpCode;
    StackModel stack;
    stack.Action("alloc", 4);
    stack.Action("push", 1);
    stack.Action("push", 1);
    stack.Action("add");
    stack.Action("pop");
    stack.Action("mov", 0);
    stack.Action("pop");
    stack.Action("load", 0);
    stack.Action("call", "println");
    auto res = stack.Export(); 
    InstructionTable instructions = *(res.first);
    SymbolTable symbols = *(res.second);
    for (long long i : instructions) {
        printf("%016llx\n", i);
    }
    printf("-----pass: stack model test-----\n\n");
}

void TestEncoder() {
    printf("-----encoder test------\n");

    using namespace CS;
    
    const char* code = GetSourceCode();
    Parser parser;
    Encoder::Encoder encoder;

    TokenList token_list = Scanner::Scan(code);
    std::shared_ptr<SyntaxTree> syntax_tree(parser.Parse(token_list));
    auto instruction_symbol = encoder.Encode(syntax_tree.get());
    
    /* instructions */
    for (long long i : *(instruction_symbol.first)) {
        printf("%016llx\n", i);
    }

    /* symbols */
    for (auto i: *(instruction_symbol.second)) {
        printf("%s -> %d\n", i.first.data(), i.second);
    }
    
    printf("-----pass: encoder test------\n\n");
}

void TestSerializer() {
    printf("-----serializer test-----\n");
    using namespace CS;
    using namespace OpCode;
    
    Parser parser;
    Encoder::Encoder encoder;
    
    const char* code = GetSourceCode();
    TokenList token_list = Scanner::Scan(code);
    std::shared_ptr<SyntaxTree> syntax_tree(parser.Parse(token_list));
    auto instruction_symbol = encoder.Encode(syntax_tree.get());
    auto inst = instruction_symbol.first;
    auto symb = instruction_symbol.second;
    /* serialize */
    Serializer::Serialize(instruction_symbol, "./test.oc");
    
    /* deserialize */
    auto deserialized = Serializer::Deserialize("./test.oc");
    auto inst_d = deserialized.first;
    auto symb_d = deserialized.second;

    assert(*inst == *inst_d);
    assert(*symb == *symb_d);
    remove("./test.oc");
    printf("-----pass: serializer test-----\n\n");
}

int main()
{
    TestScanner();
    TestParser();
    TestVariable();
    TestEvaluator();
    TestOpCode();
    TestStackModel();
    TestEncoder();
    TestSerializer();
    printf("\n------pass all test !------\n\n");
    return 0;
}
