#include <stack>

#include "scanner.hpp"

namespace CS {
    using std::pair;
    using std::string;
    using std::vector;

    class TokenNode;
    typedef TokenNode SyntaxTree;

class TokenNode {
    public:
        // constructors 
        TokenNode(const string& token_value, int token_type):
            value_(token_value), 
            type_(token_type),
            left_(nullptr),
            right_(nullptr),
            next_(nullptr) {
            }

        TokenNode():
            TokenNode("", 0) {
            }


        TokenNode(const TokenPair& token):
            TokenNode(token.first, token.second) {
            }


        void PushLeft(TokenNode* node) {
            left_ = node;
        }

        void PushRight(TokenNode* node) {
            right_ = node;
        }

        void PushNext(TokenNode* node) {
            next_ = node;
        }

        ~TokenNode() {
            if (left_)
                left_->~TokenNode();
            if (right_)
                right_->~TokenNode();
            delete left_;
            delete right_;
        }

        string& value() {
            return value_;
        }

        int type() {
            return type_;
        }

        TokenNode* next() {
            return next_;
        }
    private:
        
        string value_;
        int type_;
        TokenNode* left_;
        TokenNode* right_;
        TokenNode* next_;
};

class Parser {

    public:
        // trivial constructor
        Parser(): token_list_(), token_parsed_(0) {}

        SyntaxTree* Parse(const TokenList& token_list) {
            token_list_ = token_list;
            token_parsed_ = 0;
            SyntaxTree* root = new SyntaxTree();
            SyntaxTree* cursor = root;

            while (HasNext()) {
                auto cur = GetToken();
                if (IsStatement(cur)) {
                    cursor->PushNext(Statement());
                } else if (IsAssignment(cur)) {
                    cursor->PushNext(Assignment());
                } else if (IsCall(cur)) {
                    cursor->PushNext(Call());
                } else {
                    cursor->PushNext(Expression());
                }
                cursor = cursor->next();
            }
            return root;
        }

    private:
        // helper functions
        bool IsOperator(const TokenPair& token) {
            return Scanner::IsOperator(token.first);
        }

        bool IsStatement(const TokenPair& token) {
            return token.second < 10;
        }
        
        bool IsAssignment(const TokenPair& token) {
            return token.second == 53 &&
                    HasNext() &&
                    NextToken().first == "=";
        }

        bool IsCall(const TokenPair& token) {
            return token.second == 53 &&
                    HasNext() &&
                    NextToken().first == "(";
        }

        bool IsBinaryOperator(const TokenPair& token) {
            return IsOperator(token) &&
                token.first != ";" &&
                token.first != "\"";
        }

        void Next() {
            ++token_parsed_;
        }

        void Prev() {
            --token_parsed_;
        }

        pair<string, int>& NextToken() {
            return token_list_[token_parsed_+1];
        }

        pair<string, int>& Consume() {
            return token_list_[token_parsed_++];
        }

        pair<string, int>& GetToken() {
            return token_list_[token_parsed_];
        }

        pair<string, int>& GetNext() {
            return token_list_[token_parsed_+1];
        }

        inline bool HasNext() {
            return token_parsed_ < token_list_.size();
        }

        void SkipToken(const char* token) {
            assert(Consume().first == token);
        }

        int Priority(char ch) {
            switch (ch) {
                case '+':
                case '-':
                    return 1;
                case '*':
                case '/':
                case '%':
                    return 2;
                case '(':
                    return 3;
                default:
                    assert(false);
            }
        }
        // main force
        SyntaxTree* Statement() {
            SyntaxTree* type = new SyntaxTree(Consume());
            TokenNode* id = new SyntaxTree(Consume());
            SkipToken(";");
            type->PushLeft(id);
            return type;
        }

        SyntaxTree* Assignment() {
            SyntaxTree* id = new SyntaxTree(Consume());
            TokenNode* equal = new SyntaxTree(Consume());
            TokenNode* expr = Expression();
            SkipToken(";");
            equal->PushLeft(id);
            equal->PushRight(expr);
            return equal;
        }

        SyntaxTree* Call() {
            SyntaxTree* call = new SyntaxTree("", kLiterals()["call_type"]);
            TokenNode* fun = new SyntaxTree(Consume());
            SkipToken("(");
            TokenNode* arg = Expression();
            SkipToken(")");
            call->PushLeft(fun);
            call->PushRight(arg);
            return call;
        }

        SyntaxTree* Expression() {
            SyntaxTree* expr = new SyntaxTree();
            TokenList tokens;
            while (HasNext() && IsBinaryOperator(NextToken())) {
                Next();
                tokens.push_back(GetToken());
            }
            // convert it to postfix
            TokenList postfix;
            std::stack<TokenPair> operators;
            for (auto& i : tokens) {
                char ch = i.first.front();
                if (IsBinaryOperator(i)) {
                    while (!operators.empty() && operators.top().first != "(" &&
                            Priority(ch) <= Priority(operators.top().first.front())) {
                        postfix.push_back(operators.top()), operators.pop();
                    }
                    operators.push(i);
                } else {
                    postfix.push_back(i);
                }
            }
            while (!operators.empty()) 
                postfix.push_back(operators.top()), operators.pop();

            // construct syntax tree
            std::stack<TokenNode*> node_stack;
            for (auto &i : tokens) {
                TokenNode* node = new TokenNode(i);
                if (IsOperator(i)) {
                    TokenNode* rhs = node_stack.top(); 
                    node_stack.pop();
                    TokenNode* lhs = node_stack.top();
                    node_stack.pop();
                    node->PushLeft(lhs);
                    node->PushRight(rhs);
                    node_stack.push(node);
                } else {
                    node_stack.push(node);
                }
            }
            return node_stack.top();
        }

        TokenList token_list_;
        int token_parsed_;
};

}
