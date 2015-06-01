/**
 * 
 * An interpreter working on the syntaxtree without any optimization.
 * An unsolved problem is function call: right now I manually 
 * register some functions, but actually it's not a elegant solution.
 */

#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP
#include <unordered_map>
#include <memory>

#include "scanner.hpp"
#include "parser.hpp"
#include "variable.hpp"
#include "function.hpp"

namespace CS {
    using std::shared_ptr;
    typedef std::unordered_map<std::string, Variable> Context;
class Block {
    public:
        Block(): context_(), up_(nullptr), down_(nullptr) {}

        Block(Block* up, Block* down):
            up_(up), down_(down) {
            }


        Variable& Retrieve(const string& id) {
            return context_[id];
        }

        Variable& Retrieve(const char* id) {
            return context_[id];
        }

        Variable& operator[] (const string& id) {
            return context_[id];
        }

        Variable& operator[] (const char* id) {
            return context_[id];
        }

        /* return a function pointer */
        FunPtr Load(const string& id) {
            return fun_table_[id];
        }

        Context context_;
        FunctionTable fun_table_;
        Block* up_;
        Block* down_;
};


class Evaluator {
    public:
        Evaluator(): scanner_(), parser_(), global_context_() {
            InitFunctionTable();
        }

        string Evaluate(const string& code) {
            return Evaluate(code.c_str());
        }

        string Evaluate(const char* code) {
            TokenList&& token_list = scanner_.Scan(code);
            shared_ptr<SyntaxTree> tree(parser_.Parse(token_list));
            return BlockEvaluate(tree.get(), global_context_);
        }

    private:

        void InitFunctionTable() {
            Function::RegisterFunctions(global_context_.fun_table_);
        }

        string BlockEvaluate(SyntaxTree* tree, Block& context) {
            if (!tree) return string();
            
            /* distribute different kind of code */
            if (tree->type_ == 36 || tree->type_ == 37 ||
                    tree->type_ == 38) {
                /* variable statement */
                return Statement(tree, context);
            } else if (tree->type_ == GetId("=")) {
                /* assignment */
                return Assignment(tree, context);
            } else if (tree->type_ == GetId("call_type")) {
                /* function call */
                return Call(tree, context);
            } else if (tree->type_ == GetId("if")) {
                /* if block */
                return IfBlock(tree, context);
            } else if (tree->type_ == GetId("while")) {
                /* while block */
                return WhileBlock(tree, context);
            } else if (tree->type_ == GetId("for")) {
                /* for block */
                return ForBlock(tree, context);
            } else {
                /* error */
                std::cerr << "syntax error: " << tree->value_ << std::endl;
                exit(4);
            }
        }

        string Statement(SyntaxTree* tree, Block& context) {
            int type = tree->type_;
            string id = tree->left_->value_;
            string res;

            switch (type) {
                /* int */
                case 36: 
                    context[id] = Variable(0);
                    res = id + " = 0";
                    break;
                /* double */
                case 37:
                    context[id] = Variable(0.0);
                    res = id + " = 0.0";
                    break;
                /* pointer */
                case 38:
                    context[id] = Variable(nullptr);
                    res = id + " = nullptr";
                    break;
                default:
                    assert(false);
            }
            return res;
        }

        string Assignment(SyntaxTree* tree, Block& context) {
            assert(tree->type_ == 14);
            string id = tree->left_->value_;
            SyntaxTree* expr = tree->right_;
            Variable value = Expression(expr, context);
            context[id] = value;
            return id + " = " + value.to_string();
        }

        Variable Expression(SyntaxTree* tree, Block& context) {
            if (tree->type_ == GetId("int") ||
                    tree->type_ == GetId("double")) {
            // current node is a number
                return Variable(tree->value_, tree->type_);
            } else {
            // or it's a expression
                Variable res(tree->left_->value_, tree->left_->type_);
                Variable lhs = Expression(tree->left_, context);
                Variable rhs = Expression(tree->right_, context);
                switch (tree->type_) {
                    case 10:
                        res = lhs + rhs;
                        break;
                    case 11:
                        res = lhs - rhs;
                        break;
                    case 12:
                        res = lhs * rhs;
                        break;
                    case 13:
                        res = lhs / rhs;
                        break;
                    //case 14:
                    //    res.v = rhs;
                    //    break;
                    //case 15:
                    //    res.v = (lhs == rhs);
                    //    break;
                    //case 16:
                    //    res.v = !(lhs == rhs);
                    //    break;
                    default:
                        assert(false);
                }
                return res;
            }
        }

        string Call(SyntaxTree* tree, Block& context) {
            assert(tree->type_ == GetId("call_type"));
            string fun = tree->left_->value_;
            FunPtr f = context.Load(fun);
            f();
            return "";
        }

        string IfBlock(SyntaxTree* tree, Block& context) {
            assert(tree->type_ == 32);
            return "shit";
        }

        string WhileBlock(SyntaxTree* tree, Block& context) {
            assert(tree->type_ == 34);
            return "shit";
        }

        string ForBlock(SyntaxTree* tree, Block& context) {
            assert(tree->type_ == 45);
            return "shit";
        }

        /* members */
        Scanner scanner_;
        Parser parser_;
        Block global_context_;
};
}
#endif
