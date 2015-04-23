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
                std::pair<string, int>& cur = token_list_[token_parsed_];
                if (IsStatement(cur)) {
                    cursor->next_ = Statement();
                } else if (IsAssignment(cur)) {
                    cursor->next_ = Assignment();
                } else if (IsCall(cur)) {
                    cursor->next_ = Call();
                } else {
                    cursor->next_ = Expression();
                }
                cursor = cursor->next_;
            }
            return root;
        }

        bool IsStatement(const pair<string, int>& token) const {
            return token.second < 10;
        }
        
        bool IsAssignment(const pair<string, int>& token) const {
            return token.second == 53;
        }

        bool IsCall(const pair<string, int>& token) const {
            return token.second == 53;
        }

        pair<string, int>& Consume() {
            return token_list_[token_parsed_++];
        }

        pair<string, int>& Spit() {
            if (token_parsed > 0)
                return token_list_[token_parsed_--];
            else
                return token_list_.front();
        }

        void SkipToken(const char* token) {
            std::assert(Consume()->first == token);
        }

        SyntaxTree* Statement() {
            SyntaxTree* type = new SyntaxTree(Consume());
            TokenNode* id = new SyntaxTree(Consume());
            SkipToken(";");
            type.PushLeft(id);
            return type;
        }

        SyntaxTree* Assignment() {
            SyntaxTree* id = new SyntaxTree(Consume());
            TokenNode* equal = new SyntaxTree(Consume());
            TokenNode* expr = Expression();
            SkipToken(";");
            equal.PushLeft(id);
            equal.PushRight(expr);
            return equal;
        }

        SyntaxTree* Call() {
            SyntaxTree* call = new SyntaxTree("", kLiterals()["call_type"]);
            TokenNode* fun = new SyntaxTree(Consume());
            SkipToken("(");
            TokenNode* arg = Expression();
            SkipToken(")");
            call.PushLeft(fun);
            call.PushRight(arg);
            return call;
        }

        SyntaxTree* Expression() {
            
        }

    private:
        TokenList token_list_;
        int token_parsed_;
};

class TokenNode {
    public:
        TokenNode():
            value_()
            type_(0),
            left_(nullptr),
            right_(nullptr),
            next_(nullptr), {
                
            }

        TokenNode(const string& token_value, int token_type):
            value_(token_value), 
            type_(token_type),
            left_(nullptr),
            right_(nullptr),
            next_(nullptr) {
            }

        TokenNode(const pair<string, int>& token):
            value_(token.value_),
            type_(token.type_),
            left_(nullptr),
            right_(nullptr),
            next_(nullptr) {

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
        
        string value_;
        int type_;
        TokenNode* left_;
        TokenNode* right_;
        TokenNode* next_;
};
}
