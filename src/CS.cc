#include <iostream>
#include <unordered_map>
#include <string>

#include "evaluator.hpp"

using namespace CS;
using std::string;

typedef std::unordered_map<std::string, int> CMDTable;

int CMD(const string& cmd) {
    static CMDTable cmds = {
        { "help", 1 },
        { "exit", 2 }
    };
    return cmds[cmd];
}

string Helpers(int cmd) {
    switch (cmd) {
        case 1:
            return "no help";
        case 2:
            exit(0);
        default:
            assert(false);
    }
}

int main()
{
    //freopen("in.data", "r", stdin);
    Evaluator evaluator;
    std::cout << "Welcome to CS!" << std::endl;
    for (std::string line; ;) {
        std::cout << "CS: ";
        if (!std::getline(std::cin, line)) break;
        if (CMD(line) == 0)
            std::cout << evaluator.Evaluate(line + "\n") << std::endl;
        else 
            std::cout << Helpers(CMD(line)) << std::endl;
    }

    return 0;
}
