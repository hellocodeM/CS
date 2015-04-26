#include <stack>

#define PARSER_H

#ifndef SCANNER_H
#include "scanner.hpp"
#endif

#include "util.hpp"

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

            void PushFront(TokenNode* node) {
                if (this->next_ == nullptr) {
                    this->next_ = node;
                } else {
                    node->next_ = this->next_;
                    this->next_ = node;
                }
            }

            ~TokenNode() {
                if (left_)
                    left_->~TokenNode();
                if (right_)
                    right_->~TokenNode();
                delete left_;
                delete right_;
            }

            string value_;
            int type_;
            TokenNode* left_;
            TokenNode* right_;
            TokenNode* next_;
    };

    class Parser {

        public:
            // trivial constructor
            Parser() {}

            SyntaxTree* Parse(TokenList& token_list) {
                token_list_ = &token_list;
                token_parsed_ = 0;
                SyntaxTree root;
                TokenNode* cursor = &root;

                while (HasNext()) {
                    int position = Position();
                    if (IsStatement(position)) {
                        cursor->PushNext(Statement());
                    } else if (IsAssignment(position)) {
                        cursor->PushNext(Assignment());
                    } else {
                        cursor->PushNext(Expression());
                    }
                    SkipToken(";");
                    cursor = cursor->next_;
                }
                return root.next_;
            }

        private:
            // helper functions

            // does not need position, so these functions could be called through a single token without position
            inline bool IsOperator(int index) {
                return IsOperator(GetToken(index));
            }

            inline bool IsOperator(const TokenPair& token) {
                return Scanner::IsOperator(token.first);
            }

            inline bool IsOperator(const string& value) {
                return Scanner::IsOperator(value);
            }

            inline bool IsBinaryOperator(int index) {
                return IsBinaryOperator(GetToken(index));
            }

            inline bool IsBinaryOperator(const TokenPair& token) {
                return IsOperator(token) &&
                    token.first != "\"" &&
                    token.first != ";";
            }

            inline bool IsBinaryOperator(const string& value) {
                return Scanner::IsOperator(value) &&
                    value != "\"" &&
                    value != ";";
            }

            inline bool IsNumber(int index) {
                return IsNumber(GetToken(index));
            }

            inline bool IsNumber(const TokenPair& token){
                return token.second == GetId("int") ||
                        token.second == GetId("double");
            }

            inline bool IsIdentifier(int index) {
                return IsIdentifier(GetToken(index));
            }

            inline bool IsIdentifier(const TokenPair& token) {
                return token.second == 53;
            }
            // need the help of position
            bool IsStatement(int index) {
                auto& token = GetToken(index);
                return (token.second == 36 ||
                        token.second == 37) &&
                    HasNext(index) &&
                    IsIdentifier(index+1);
            }

            bool IsAssignment(int index) {
                auto& token = GetToken(index);
                return IsIdentifier(index) &&
                    HasNext(index) &&
                    NextToken(index).first == "=";
            }

            bool IsCall(int index) {
                auto& token = GetToken(index);
                return IsIdentifier(index) &&
                    HasNext(index) &&
                    NextToken(index).first == "(";
            }

            bool IsValue(int index) {
                auto& token = GetToken(index);
                return (token.first == "(" && 
                        HasNext(index) &&
                        IsValue(index+1)) ||
                    IsNumber(index) ||
                    IsIdentifier(index) ||
                    IsCall(index);
            }

            // control the token list
            inline void Next() {
                ++token_parsed_;
            }

            inline void Prev() {
                --token_parsed_;
            }

            inline int Position() {
                return token_parsed_;
            }

            inline pair<string, int>& NextToken() {
                return (*token_list_)[token_parsed_+1];
            }

            inline pair<string, int>& NextToken(int index) {
                return (*token_list_)[index+1];
            }

            inline pair<string, int>& Consume() {
                return (*token_list_)[token_parsed_++];
            }

            inline pair<string, int>& GetToken() {
                return (*token_list_)[token_parsed_];
            }

            inline pair<string, int>& GetToken(int index) {
                return (*token_list_)[index];
            }

            inline bool HasNext() {
                return token_parsed_ < token_list_->size();
            }

            inline bool HasNext(int index) {
                return index < token_list_->size();
            }

            void SkipToken(const char* token) {
                assert(Consume().first == token);
            }

            // operator priority

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
                type->PushLeft(id);
                return type;
            }

            SyntaxTree* Assignment() {
                SyntaxTree* id = new SyntaxTree(Consume());
                TokenNode* equal = new SyntaxTree(Consume());
                TokenNode* expr = Expression();
                equal->PushLeft(id);
                equal->PushRight(expr);
                return equal;
            }

            TokenNode* Arguments() {
                TokenNode* root = nullptr;
                enum State { ZERO, ONE, TWO };
                State state = ZERO;
                while (HasNext() && state != TWO) {
                    int position = Position();
                    switch (state) {
                        case ZERO:
                            if (IsValue(position)) {
                                if (root == nullptr)
                                    root = Value();
                                else
                                    root->PushFront(Value());
                                state = TWO;
                            } else {
                                std::cerr << "not a value" << std::endl;
                                exit(3);
                            }
                            break;
                        case ONE:
                            if (GetToken().first == ",") {
                                SkipToken(",");
                                state = ZERO;
                            } else {
                                state = TWO;
                            }
                            break;
                        case TWO:
                            goto end;
                        default:
                            assert(false);
                    }
                }
                end:
                return root;
            }

            TokenNode* Value() {
                auto& token = GetToken();
                TokenNode* node;
                if (token.first == "(") {
                    SkipToken("(");
                    node = Value();
                    SkipToken(")");
                } else {
                    if (IsNumber(Position())) {
                        node = new TokenNode(Consume());
                    } else if (IsCall(Position())) {
                        node = new TokenNode;
                        node->type_ = GetId("call_type");
                        // function identifier
                        node->PushLeft(new TokenNode(Consume()));
                        // '('
                        SkipToken("(");
                        // expr
                        node->PushRight(Arguments());
                        // ')'
                        SkipToken(")");
                    } else if (IsIdentifier(Position())) {
                        node = new TokenNode(Consume());
                    } else {
                        std::cerr << "not a value" << std::endl;
                        exit(3);
                    }
                }
                return node;
            }

            SyntaxTree* Expression() {
                std::vector<TokenNode*> tokens;
                enum State { ZERO, ONE, TWO};
                State state = ZERO;
                while (HasNext() && state != TWO) {
                    int position = Position();
                    switch (state) {
                        case ZERO:
                            if (IsValue(position)) {
                                tokens.push_back(Value());
                            } else {
                                std::cerr << "not a value:\t\'" << GetToken(position).first << "\'"<< std::endl;
                                exit(2);
                            }
                            state = ONE;
                            break;
                        case ONE:
                            if (IsBinaryOperator(position)) {
                                tokens.push_back(new TokenNode(Consume()));
                                state = ZERO;
                            } else {
                                state = TWO;
                            }
                            break;
                        case TWO:
                        default:
                            assert(false);
                    }
                }
                // convert it to postfix
                std::vector<TokenNode*> postfix;
                std::stack<TokenNode*> operators;
                for (auto node : tokens) {
                    char ch = node->value_.front();
                    if (IsBinaryOperator(node->value_)) {
                        while (!operators.empty() && operators.top()->value_ != "(" &&
                                Priority(ch) <= Priority(operators.top()->value_.front())) {
                            postfix.push_back(operators.top()), operators.pop();
                        }
                        operators.push(node);
                    } else {
                        postfix.push_back(node);
                    }
                }
                while (!operators.empty()) 
                    postfix.push_back(operators.top()), operators.pop();

                // construct syntax tree from postfix expression
                std::stack<TokenNode*> node_stack;
                for (auto &node : postfix) {
                    if (IsOperator(node->value_)) {
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

            TokenList* token_list_;
            int token_parsed_;
    };

}
