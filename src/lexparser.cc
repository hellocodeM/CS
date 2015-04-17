#include "lexparser.h"

namespace CS {

LexParser::LexParser() {

}

TokenList LexParser::Scan(const string& s) {
    return Scan(std::istringstream(const_cast<string&>(s)));
}

TokenList LexParser::Scan(const char* s) {
    return Scan(string(s));
}

TokenList LexParser::Scan(std::istream& in) {
    TokenList tokens;
    string token;
    State state = START;
    char ch;

    while (!in.eof()) {
        ch = in.get();
    begin:
        switch (state) {
            case START:
                token += ch;
                if (ispunct(ch)) 
                    state = PUNCT;
                else if (isdigit(ch))
                    state = NUMBER;
                else if (isalpha(ch))
                    state = LETTER;
                else 
                    token.clear();
                break;
            case NUMBER:
                if (isdigit(ch) || ch == '.') {
                    token += ch;
                } else {
                    Restart(state, token, tokens);
                    goto begin;
                }
                break;
            case PUNCT:
                if (ispunct(ch)) {
                    token += ch;
                } else {
                    Restart(state, token, tokens);
                    goto begin;
                }
                break;
            case LETTER:
                if (isalpha(ch)) {
                    token += ch;
                } else {
                    Restart(state, token, tokens);
                    goto begin;
                }
                break;
            case DONE:
                break;
            default:
                assert(false);
        }
    }
}

void LexParser::Restart(State& state, string& token, TokenList& tokens) {
    state = START;
    int type_id = IdentifyToken(token);
    tokens.emplace_back(token, type_id);
    token.clear();
}

 bool LexParser::IsOperator(const string& token) const {
    return kOperators.find(token) != kOperators.end();
}

 bool LexParser::IsNumber(const string& token) const {
    return !token.empty() && 
            std::all_of(token.begin(), token.end(), isdigit);
}

 bool LexParser::IsLetter(const string& token) const {
    return !token.empty() &&
            isalpha(token.front()) &&
            std::all_of(token.begin(), token.end(), isalnum);
}

 bool LexParser::IsKeyword(const string& token) const {
    return kKeywords.find(token) != kKeywords.end();
}

int LexParser::IdentifyToken(const string& token) const {
    if (IsOperator(token))
        return kOperators[token];
    else if (IsNumber(token))
        return NUMBER_VALUE;
    else if (IsLetter(token)) {
        if (IsKeyword(token))
            return kKeywords[token];
        return IDENTIFIER;
    } else {
        std::cerr << "unidentified token " << token << std::endl;
        exit(1);
    }
        
}
};
