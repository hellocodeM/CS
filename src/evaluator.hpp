#include <unordered_map>

#define EVALUATOR_H

#ifndef SCANNER_H
#include "scanner.hpp"
#endif

#ifndef PARSER_H
#include "parser.hpp"
#endif

#ifndef VARIABLE_H
#include "variable.hpp"
#endif

namespace CS {
    class Block;
    typedef std::unordered_map<std::string, Variable> Context;

class Block {
    public:
        Block(): context_() {}

        Variable& Retrieve(const string& id) {
            return context_[id];
        }

        Variable& operator[] (const string& id) {
            return context_[id];
        }

    private:
        Context context_;
};


class Evaluator {
    public:
        Evaluator(): scanner_(), parser_() {}

        string Evaluate(const string& code) {
            return Evaluate(code.c_str());
        }

        string Evaluate(const char* code) {
            return "shit";
        }

    private:
        string BlockEvaluate(SyntaxTree* tree) {
            if (!tree) return string();
            while (tree) {
            /* distribute different kind of code */
                if (tree->type_ < 10) {
                    /* variable statement */
                    return Statement(tree);
                } else if (tree->type_ == 14) {
                    /* assignment */
                    return Assignment(tree);
                } else if (tree->type_ == 54) {
                    /* function call */
                    return Call(tree);
                } else if (tree->type_ == 32) {
                    /* if block */
                    return IfBlock(tree);
                } else if (tree->type_ == 34) {
                    /* while block */
                    return WhileBlock(tree);
                } else if (tree->type_ == 35) {
                    /* for block */
                    return ForBlock(tree);
                } else {
                    /* error */
                    std::cerr << "unknown SyntaxTree" << tree->value_ << std::endl;
                    exit(4);
                }
                tree = tree->next_;
            }
        }

        string Statement(SyntaxTree* tree) {
            int type = tree->type_;
            string id = tree->left_->value_;
            string res;

            context_[id].type_id = type;
            switch (type) {
                /* int */
                case 1: 
                    context_[id].v.i = 0;
                    res = id + " = 0";
                    break;
                /* double */
                case 2:
                    context_[id].d = 0.0;
                    res = id + " = 0.0";
                    break;
                /* pointer */
                case 3:
                    context_[id].p = nullptr;
                    res = id + " = nullptr";
                    break;
                default:
                    assert(false);
            }
            return res;
        }

        string Assignment(SyntaxTree* tree) {
            assert(tree->type_ == 14);
            string id = tree->left_->value_;
            SyntaxTree* expr = tree->right_;
            Variable value = Expression(expr);
            context_[id] = value;
            return id + " = " + value.toString();
        }

        Variable Expression(SyntaxTree* tree) {
            Variable res;
            if (tree->type_ == 51) {
            // current node is a number
                res.type_id = tree->type_;
                res.v.d = tree->value_;
            } else {
            // or it's a expression
                res.type_id = tree->left_.type_id;
                Variable lhs = Expression(tree->left_);
                Variable rhs = Expression(tree->right_);
                switch (tree->type_) {
                    case 10:
                        res.v = lhs + rhs;
                        break;
                    case 11:
                        res.v = lhs - rhs;
                        break;
                    case 12:
                        res.v = lhs * rhs;
                        break;
                    case 13:
                        res.v = lhs / rhs;
                        break;
                    case 14:
                        res.v = rhs;
                        break;
                    case 15:
                        res.v = (lhs == rhs);
                        break;
                    case 16:
                        res.v = !(lhs == rhs);
                        break;
                    default:
                        assert(false);
                }
            }
            return res;
        }

        string Call(SyntaxTree* tree) {
            assert(tree->left_->type_ == 54);
            string fun = tree->left_->value_;
            
            
        }

        string IfBlock(SyntaxTree* tree) {
            assert(tree->type_ == 32);
        }

        string WhileBlock(SyntaxTree* tree) {
            assert(tree->type_ == 34);

        }

        string ForBlock(SyntaxTree* tree) {
            assert(tree->type_ == 45);
        }

        /* members */
        Scanner scanner_;
        Parser parser_;
};
}
