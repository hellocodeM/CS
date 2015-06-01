#ifndef SCANNER_HPP
#define SCANNER_HPP
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_map>

namespace CS {
        using std::string;

        typedef std::pair<string, int> TokenPair;
        typedef std::vector<TokenPair> TokenList;
        typedef std::unordered_map<string, int> TokenMap;

        TokenMap& kTypes() {
            static TokenMap types = {
                { "int", 1 },
                { "double", 2 },
                { "pointer", 3}
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
                { ")", 19 },
                { ";", 20 }
            };
            return operators;
        }

        TokenMap& kKeywords() {
            static TokenMap keywords = {
                { "return", 31 },
                { "if", 32 },
                { "else", 33 },
                { "while", 34 },
                { "for", 35 },
                { "int", 36 },
                { "double", 37}
            };
            return keywords;
        }

        TokenMap& kLiterals() {
            static TokenMap literals = {
                { "number_type", 51 },
                { "string_type", 52 },
                { "identifier_type", 53 },
                { "call_type", 54 }
            };
            return literals;
        }

        class Scanner {
            enum State { START, NUMBER, PUNCT, LETTER, DONE };
            public:
            Scanner() {}


            static TokenList Scan(const string& s) {
                return Scan(s.c_str());
            }

            static TokenList Scan(const char* s) {
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
                            // avoid this condition: println("hello");
                            if (ch != ';' && ispunct(ch)) {
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

            static void Restart(State& state, string& token, TokenList& tokens) {
                state = START;
                int type_id = IdentifyToken(token);
                tokens.emplace_back(token, type_id);
                token.clear();
            }

            static bool IsOperator(const string& token) {
                return kOperators().find(token) != kOperators().end();
            }

            static bool IsNumber(const string& token) {
                return !token.empty() && 
                    std::all_of(token.begin(), token.end(), isdigit);
            }

            static bool IsDouble(const string& token) {
                return token.find('.') != token.npos;
            }

            static bool IsLetter(const string& token) {
                return !token.empty() &&
                    isalpha(token.front()) &&
                    std::all_of(token.begin(), token.end(), isalnum);
            }

            static bool IsKeyword(const string& token) {
                return kKeywords().find(token) != kKeywords().end();
            }

            static bool IsType(const string& token) {
                return kTypes().find(token) != kTypes().end();
            }

            static int IdentifyToken(const string& token) {
                if (IsOperator(token))
                    return kOperators()[token];
                else if (IsNumber(token)) {
                    if (IsDouble(token)) 
                        return kTypes()["double"];
                    else
                        return kTypes()["int"];
                }
                else if (IsLetter(token)) {
                    if (IsKeyword(token))
                        return kKeywords()[token];
                    else if (IsType(token))
                        return kTypes()[token];
                    else
                        return kLiterals()["identifier_type"];
                } else {
                    std::cerr << "undefined token " << token << std::endl;
                    exit(1);
                }

            }

            private:
        };
}
#endif
