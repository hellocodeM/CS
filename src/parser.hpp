#include <stack>

#ifndef SCANNER
#define SCANNER
#include "scanner.hpp"
#endif

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
            return root;
        }

    private:
        // helper functions
        
        // does not need position, so these functions could be called through a single token without position
        bool IsOperator(int index) {
            return IsOperator(GetToken(index));
        }

        bool IsOperator(const TokenPair& token) {
            return Scanner::IsOperator(token.first);
        }

        bool IsBinaryOperator(int index) {
            return IsBinaryOperator(GetToken(index));
        }

        bool IsBinaryOperator(const TokenPair& token) {
            return IsOperator(token) &&
                token.first != "\"" &&
                token.first != ";";
        }

        bool IsNumber(int index) {
            return IsNumber(GetToken(index));
        }

        bool IsNumber(const TokenPair& token){
            return token.second == 51;
        }

        bool IsIdentifier(int index) {
            return IsIdentifier(GetToken(index));
        }

        bool IsIdentifier(const TokenPair& token) {
            return token.second == 53;
        }
        // need the help of position
        bool IsStatement(int index) {
            auto& token = GetToken(index);
            return token.second < 10 &&
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
            return token_list_[token_parsed_+1];
        }

        inline pair<string, int>& NextToken(int index) {
            return token_list_[index+1];
        }

        inline pair<string, int>& Consume() {
            return token_list_[token_parsed_++];
        }

        inline pair<string, int>& GetToken() {
            return token_list_[token_parsed_];
        }

        inline pair<string, int>& GetToken(int index) {
            return token_list_[index];
        }

        inline bool HasNext() {
            return token_parsed_ < token_list_.size();
        }

        inline bool HasNext(int index) {
            return index < token_list_.size();
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
        TokenList Arguments() {
            TokenList res;
            enum State { ZERO, ONE, TWO };
            State state = ZERO;
            while (HasNext()) {
                int position = Position();
                switch (state) {
                    case ZERO:
                        if (IsValue(position)) {
                            auto&& value = Value();
                            std::copy(value.begin(), value.end(),
                                        std::back_inserter(res));
                            state = TWO;
                        } else {
                            std::cerr << "not a value" << std::endl;
                            exit(3);
                        }
                        break;
                    case ONE:
                        if (GetToken().first == ",") {
                            res.push_back(Consume());
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
            return res;
        }

        TokenList Value() {
            auto& token = GetToken();
            TokenList res;
            if (token.first == "(") {
                SkipToken("(");
                res = Value();
                SkipToken(")");
            } else {
                if (IsNumber(Position())) {
                    res.push_back(Consume());
                } else if (IsCall(Position())) {
                    // id
                    res.push_back(Consume());
                    // '('
                    res.push_back(Consume());
                    // expr
                    TokenList&& args = Arguments();
                    std::copy(args.begin(), args.end(),
                                std::back_inserter(res));
                    // ')'
                    res.push_back(Consume());
                } else if (IsIdentifier(Position())) {
                    res.push_back(Consume());
                } else {
                    std::cerr << "not a value" << std::endl;
                    exit(3);
                }
            }
            return res;
        }

        SyntaxTree* Expression() {
            TokenList tokens;
            enum State { ZERO, ONE, TWO};
            State state = ZERO;
            while (HasNext()) {
                int position = Position();
                switch (state) {
                    case ZERO:
                        if (IsValue(position)) {
                            auto value = Value();
                            std::copy(value.begin(),
                                        value.end(),
                                        std::back_inserter(tokens));
                        } else {
                            std::cerr << "not a value:\t\'" << GetToken(position).first << "\'"<< std::endl;
                            exit(2);
                        }
                        state = ONE;
                        break;
                    case ONE:
                        if (IsBinaryOperator(position)) {
                            tokens.push_back(Consume());
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

            // construct syntax tree from postfix expression
            std::stack<TokenNode*> node_stack;
            for (auto &i : postfix) {
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
