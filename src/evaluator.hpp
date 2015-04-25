
#ifndef SCANNER
#define SCANNER
#include "scanner.hpp"
#endif

#ifndef PARSER
#define PARSER
#include "parser.hpp"
#endif

namespace CS {

union Value {
    int i;
    double d;
    void* p;
};

struct Variable {
    Value v;
    int type_id;

    string toString() {
        switch (type_id) {
            case 1:
                return std::to_string(v.i);
            case 2:
                return std::to_string(v.v);
            case 3:
                return std::to_string(v.d);
            case 4:
                return std::to_string(v.p);
            default:
                return string();
        }
    }
};

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
                } else if {
                    /* assignment */
                    return Assignment(tree);
                } else if {
                    /* function call */
                    return Call(tree);
                } else if {
                    /* if block */
                    return IfBlock(tree);
                } else if {
                    /* while block */
                    return WhileBlock(tree);
                } else if {
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
                /* void */
                case 2:
                    context_[id].v = 0;
                    res = id + " = 0";
                    break;
                /* double */
                case 3:
                    context_[id].d = 0.0;
                    res = id + " = 0.0";
                    break;
                /* pointer */
                case 4:
                    context_[id].p = nullptr;
                    res = id + " = nullptr";
                    break;
                default:
                    assert(false);
            }
            return res;
        }

        string Assignment(SyntaxTree* tree) {
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

        }

        string IfBlock(SyntaxTree* tree) {

        }

        string WhileBlock(SyntaxTree* tree) {

        }

        string ForBlock(SyntaxTree* tree) {

        }

        /* members */
        Scanner scanner_;
        Parser parser_;
};
}
