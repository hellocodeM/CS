#include <unordered_map>
#include <vector>
#include <string>

#define OPCODE_H

namespace CS {

    namespace OpCode {
        using std::string;
        using std::vector;
        using std::unordered_map;

        typedef unordered_map<string, int> OpCodeTable;
        typedef unordered_map<string, int> SymbolTable; // store identifer and index
        typedef vector<long long> InstructionTable;


        static int GetOpCode(const string& op) {
            return GetOpCode(op.c_str());
        }

        static int GetOpCode(const char* op) {
            static OpCodeTable opcodes = {

                /* stack-oriented */
                { "push", 1 },
                { "pop",  2 },
                { "mov",  3 },
                { "top",  4 },
                { "load", 5 },

                /* numeric */
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
                StackModel() {}

                ~StackModel() {}

                std::pair<InstructionTable*, SymbolTable*>
                Export() {
                    return std::make_pair(&instructions_, &symbol_table_);
                }
                void Action(const string& op, int address = 0) {
                    Action(op.c_str(), address);
                }

                void Action(const char* op, int address = 0) {
                    instructions_.push_back(MakeOpCode(op, address));
                }

                void Action(const string& op, const string& id) {
                    Action(op.data(), id.data());
                }

                void Action(const char* op, const char* id) {
                    int index = symbol_table_.size() + 1;
                    symbol_table_.emplace(op, index);
                    instructions_.push_back(MakeOpCode(op, index));
                }

            private:
                InstructionTable instructions_;
                SymbolTable symbol_table_;
        };
    }
}
