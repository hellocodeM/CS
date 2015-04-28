#include <unordered_map>
#include <vector>
#include <string>
#include <cstring>

#define OPCODE_H

namespace CS {

    namespace OpCode {
        using std::string;
        using std::vector;
        using std::unordered_map;

        typedef unordered_map<string, int> OpCodeTable;
        typedef unordered_map<string, int> SymbolTable; // store identifer and index
        typedef vector<long long> InstructionTable;


        int GetOpCode(const string& op) {
            return GetOpCode(op.c_str());
        }

        int GetOpCode(const char* op) {
            static OpCodeTable opcodes = {

                /* stack-oriented */
                { "push", 1 },
                { "pop",  2 },
                { "mov",  3 },
                { "top",  4 },
                { "load", 5 },
                { "alloc", 6},

                /* numeric, don't need address*/
                { "add", 20 },
                { "sub", 21 },
                { "mul", 22 },
                { "div", 23 },

                /* jump */
                { "call",30 },
                { "jmp", 31 }
            };
            return opcodes[op];
        }

        static long long MakeOpCode(int op, int address) {
            return (static_cast<long long>(op) << 32) ^ 
                static_cast<long long>(address);
        }

        static long long MakeOpCode(const char* op, int address) {
            return MakeOpCode(GetOpCode(op), address);
        }

        static std::pair<int, int> SplitOpCode(long long instruction) {
            return std::make_pair(
                    static_cast<int>(instruction >> 32),
                    static_cast<int>(instruction));
        }

        class StackModel {
            public:
                StackModel(): stack_top_(0) {
                    instructions_ = new InstructionTable();
                    symbol_table_ = new SymbolTable();
                }

                ~StackModel() {
#ifndef NDEBUG
                    printf("destruct stackmodel\n");
#endif
                    delete instructions_;
                    delete symbol_table_;
                }

                std::pair<InstructionTable*, SymbolTable*>
                Export() {
                    return std::make_pair(instructions_, symbol_table_);
                }

                int Action(const string& op, int address = 0) {
                    return Action(op.c_str(), address);
                }

                int Action(const char* op, int address = 0) {
                    instructions_->push_back(MakeOpCode(op, address));
                    if (strcmp(op, "alloc")) {
                        stack_top_ += address;
                    } else if (strcmp(op, "push")) {
                        stack_top_  += 4;
                    } else if (strcmp(op, "pop")) {
                        stack_top_ -= 4;
                    } else if (strcmp(op, "load")) {
                        stack_top_ += 4;
                    }
                    return stack_top_;
                }

                /* function call*/
                int Action(const string& op, const string& id) {
                    return Action(op.data(), id.data());
                }

                int Action(const char* op, const char* id) {
                    int index = symbol_table_->size() + 1;
                    symbol_table_->emplace(id, index);
                    instructions_->push_back(MakeOpCode(op, index));
                    return 0;
                }

            private:
                InstructionTable* instructions_;
                SymbolTable* symbol_table_;
                int stack_top_;
        };
    }
}
