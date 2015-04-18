#include "scanner.hpp"

namespace CS {
    typedef TokenNode SyntaxTree;

class Parser {

    public:
        Parser(): token_list_(), token_parsed_(0) {}

        SyntaxTree* Parse(const TokenList& token_list) {
            token_list_ = token_list;
            token_parsed_ = 0;
            SyntaxTree* root = new SyntaxTree();
            SyntaxTree* cursor = root;

            while (token_parsed_ < token_list_.length()) {
                SyntaxTree* node = Declaration();
                if (node == nullptr)
                    node = Assignment();
                if (node == nullptr)
                    node = Expression();
                if (node == nullptr)
                    node = Call();
                if (node == nullptr) {
                    std::cerr << "syntax error: " <<
                        token_list_[token_parsed_].first << std::endl;
                    exit(2);
                } else {
                    cursor->next = node;
                    cursor = cursor->next;
                }
            }
            return root;
        }

        SyntaxTree* Declaration() {
            
        }

        SyntaxTree* Assignment() {

        }

        SyntaxTree* Call() {

        }

        SyntaxTree* Expression() {
            int index = token_parsed_;
            auto cursor = token_list_[index];
            State state = START;
            while (index < token_list_.length()) {
            }
        }

    private:
        TokenList token_list_;
        int token_parsed_;
};

class TokenNode {
    public:
        TokenNode():
            value_(), type_(0) {
                
            }

        TokenNode(const string& token_value, int token_type):
            value_(token_value), 
            type_(token_type),
            left_(nullptr),
            right_(nullptr) {
            }

        void PushLeft(TokenNode* node) {
            left_ = node;
        }

        void PushRight(TokenNode* node) {
            right_ = node;
        }

        void Destory() {
            left_->~TokenNode();
            right_->~TokenNode();
            delete left_;
            delete right_;
        }

        ~TokenNode() {
            Destory();
        }

        string value() {
            return value_;
        }

        int type() {
            return type_;
        }
        
    private:
        string value_;
        int type_;
        TokenNode* left_;
        TokenNode* right_;
};
}
