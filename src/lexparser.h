#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <cassert>
#include <algorithm>


namespace CS {
    using std::vector;
    using std::pair;
    using std::string;
    using std::unordered_map;

    typedef unordered_map<string, int> SymbolTable;
    typedef vector<pair<string, int>> TokenList;

    enum LiteralValue { NUMBER_VALUE = -3, STRING_VALUE, IDENTIFIER};

    SymbolTable kKeywords = {
        { "int", 1 },
        { "void", 2 },
        { "char", 3 },
        { "double", 4 },
        { "include", 10 },
        { "if", 11 },
        { "else", 12 },
        { "while", 13 },
        { "for", 14 },
        { "const", 15 },
        { "auto", 16 }
    };

     SymbolTable kOperators = {
        { "+", 30 },
        { "-", 31 },
        { "*", 32 },
        { "/", 33 },
        { "%", 34 },
        { "!", 35 },
        { "~", 36 },
        { ".", 37 },
        { "=", 38 },
        { "+=", 39 },
        { "-=", 40 },
        { "*=", 41 },
        { "/=", 42 },
        { "%", 43 },
        { ">", 44 },
        { "<", 45 },
        { ">=", 46 },
        { "<=", 47 },
        { "!=", 48 },
        { "==", 49 },
        { ";", 50 },
        { "[", 51 },
        { "]", 52 },
        { "(", 53 },
        { ")", 54 },
        { "{", 55 },
        { "}", 56 }
    };

    class LexParser {
        public:
            LexParser();

            TokenList Scan(const string& s);
            TokenList Scan(const char* s);
            TokenList Scan(std::istream& in);
            
        private:
            enum State { START, NUMBER, PUNCT, LETTER, DONE };

            bool IsOperator(const string& token) const;
            bool IsNumber(const string& token) const;
            bool IsLetter(const string& token) const;
            bool IsKeyword(const string& token) const;
            int IdentifyToken(const string& token) const;
            void Restart(State& state, string& token, TokenList& tokens);
            
            
    };
};
