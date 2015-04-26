#include <string>
#include <cstdio>

#define UTIL_H
#ifndef SCANNER_H
#include "scanner.hpp"
#endif

namespace CS {

    int GetId(const string& key) {
        return GetId(key.c_str());
    }

    int GetId(const char* key) {
        static TokenMap& types = kTypes();
        static TokenMap& operators = kOperators();
        static TokenMap& keywords = kKeywords();
        static TokenMap& literals = kLiterals();
        if (types.find(key) != types.end())
            return types[key];
        if (operators.find(key) != types.end())
            return operators[key];
        if (keywords.find(key) != keywords.end())
            return keywords[key];
        if (literals.find(key) != literals.end())
            return literals[key];
        assert(false);
    }

}
