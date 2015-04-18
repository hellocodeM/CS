#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <algorithm>
#include <cassert>
#include <iostream>

namespace CS {
    using std::string;

    typedef std::vector<std::pair<string, int>> TokenList;
    typedef std::map<string, int> TokenMap;

    TokenMap& kTypes() {
        static TokenMap types = {
            { "int", 1 },
            { "void", 2 },
            { "double", 3}
        };
        return types;
    }

    TokenMap& kOperators() {
        static TokenMap operators = {
            { "+", 10 },
            { "-", 11 },
            { "*", 12 },
            { "/", 13 },
            { "=", 14 },
            { "==", 15 },
            { "!=", 16 },
            { "\"", 17 },
            { "(", 18 },
            { ")", 19 }
        };
        return operators;
    }

    TokenMap& kKeywords() {
        static TokenMap keywords = {
            { "return", 31 },
            { "if", 32 },
            { "else", 33 },
            { "while", 34 },
            { "for", 35 }
        };
        return keywords;
    }

    TokenMap& kLiterals() {
        static TokenMap literals = {
            { "number_type", 51 },
            { "string_type", 52 },
            { "identifier_type", 53 }
        };
        return literals;
    }

    class Scanner {
            enum State { START, NUMBER, PUNCT, LETTER, DONE };
        public:
            Scanner() {}


            TokenList Scan(const string& s) {
                return Scan(s.c_str());
            }

            TokenList Scan(const char* s) {
                TokenList tokens;
                string token;
                State state = START;
                char ch;

                while (*s != '\0') {
                    ch = *s++;
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
                return tokens;
            }

            void Restart(State& state, string& token, TokenList& tokens) {
                state = START;
                int type_id = IdentifyToken(token);
                tokens.emplace_back(token, type_id);
                token.clear();
            }

            bool IsOperator(const string& token) const {
                return kOperators().find(token) != kOperators().end();
            }

            bool IsNumber(const string& token) const {
                return !token.empty() && 
                    std::all_of(token.begin(), token.end(), isdigit);
            }

            bool IsLetter(const string& token) const {
                return !token.empty() &&
                    isalpha(token.front()) &&
                    std::all_of(token.begin(), token.end(), isalnum);
            }

            bool IsKeyword(const string& token) const {
                return kKeywords().find(token) != kKeywords().end();
            }

            bool IsType(const string& token) const {
                return kTypes().find(token) != kTypes().end();
            }

            int IdentifyToken(const string& token) const {
                if (IsOperator(token))
                    return kOperators()[token];
                else if (IsNumber(token))
                    return kLiterals()["number_type"];
                else if (IsLetter(token)) {
                    if (IsKeyword(token))
                        return kKeywords()[token];
                    else if (IsType(token))
                        return kTypes()[token];
                    else
                        return kLiterals()["identifier_type"];
                } else {
                    std::cerr << "unidentified token " << token << std::endl;
                    exit(1);
                }

            }

        private:
    };
}
