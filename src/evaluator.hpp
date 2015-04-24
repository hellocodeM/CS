
#ifndef SCANNER
#define SCANNER
#include "scanner.hpp"
#endif

#ifndef PARSER
#define PARSER
#include "parser.hpp"
#endif

namespace CS {

class Evaluator {
    public:
        Evaluator() {}

        string Evaluate(const string& code) {
            return Evaluate(code.c_str());
        }

        string Evaluate(const char* code) {
            return "shit";
        }

    private:

};
}
