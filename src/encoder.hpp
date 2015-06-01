/**
 * Convert a syntax tree to instructions, which could be execute by a virtual machine using stack model.
 */

#ifndef ENCODER_HPP
#define ENCODER_HPP
#include <unordered_map>
#include <string>

#include "parser.hpp"
#include "opcode.hpp"

namespace CS {
    namespace Encoder {
        using OpCode::InstructionTable;
        using OpCode::SymbolTable;
        using OpCode::StackModel;

        typedef std::unordered_map<std::string, int> Context;

        class Encoder {

            public:
                Encoder(): stack_() {}

                ~Encoder() {}

                std::pair<InstructionTable*, SymbolTable*>
                Encode(SyntaxTree* syntax_tree) {
                    BlockEvaluate(syntax_tree);
                    return stack_.Export();
                }


            private:
                void BlockEvaluate(SyntaxTree* tree) {
                    Context context;
                    while (tree) {
                        if (tree->type_ == 36 || tree->type_ == 37) {
                            Statement(tree, context);
                        } else if (tree->type_ == GetId("=")) {
                            Assignment(tree, context);
                        } else if (tree->type_ == GetId("call_type")) {
                            Call(tree, context);
                        } else if (tree->type_ == GetId("if")) {
                            IfBlock(tree, context);
                        } else if (tree->type_ == GetId("while")) {
                            WhileBlock(tree, context);
                        } else if (tree->type_ == GetId("for")) {
                            ForBlock(tree, context);
                        } else {
                            std::cerr << "syntax error: " << tree->value_ << std::endl;
                            exit(4);
                        }
                        tree = tree->next_;
                    }
                }
                
                void Statement(SyntaxTree* tree, Context& context) {
                    assert(tree->type_ == 36 ||
                            tree->type_ == 37);
                    /* int type */
                    int pos = 0;
                    if (tree->type_ == 36) {
                        pos = stack_.Action("alloc", 4);
                    /* double type */
                    } else if (tree->type_ == 37) {
                        pos = stack_.Action("alloc", 8);
                    }
                    context.emplace(tree->value_, pos);
                }

                void Assignment(SyntaxTree* tree, Context& context) {
                    assert(tree->type_ == GetId("="));
                    Expression(tree->right_, context);
                    stack_.Action("mov", context[tree->value_]);
                    stack_.Action("pop");
                }

                void Call(SyntaxTree* tree, Context& context) {
                    TokenNode* args = tree->right_;
                    while (args) {
                        Expression(args, context);
                        args = args->next_;
                    }
                    stack_.Action("call", tree->left_->value_.data());
                }

                void Expression(SyntaxTree* tree, Context& context) {
                    if (tree->left_ == nullptr && tree->right_ == nullptr) {
                        if (tree->type_ == GetId("int") ||
                            tree->type_ == GetId("double")) {
                            stack_.Action("push", std::stoi(tree->value_));
                        } else if (tree->type_ == GetId("identifier_type")){
                            stack_.Action("load", context[tree->value_]);
                        }
                    } else if (tree->type_ == GetId("call_type")) {
                        Call(tree, context);
                    } else {
                        Expression(tree->left_, context);
                        Expression(tree->right_, context);
                        switch (tree->type_) {
                            case 10:
                                stack_.Action("add");
                                break;
                            case 11:
                                stack_.Action("sub");
                                break;
                            case 12:
                                stack_.Action("mul");
                                break;
                            case 13:
                                stack_.Action("div");
                                break;
                            default:
                                assert(false);
                        }
                        stack_.Action("pop");
                    }
                }

                void IfBlock(SyntaxTree* tree, Context& context) {
                    assert(tree->type_ == GetId("if"));
                    
                }

                void WhileBlock(SyntaxTree* tree, Context& context) {
                    assert(tree->type_ = GetId("while"));

                }

                void ForBlock(SyntaxTree* tree, Context& context) {
                    assert(tree->type_ == GetId("for"));

                }

                StackModel stack_;
        };
    }
}
#endif
