/*
 * The core virtual machine.
 * The instruction is designed by myself, inspired by Java Bytecode and Lua Opcode. You could find all the instructions in 'opcode.hpp'.
 * Of course, these instructions are not complete, instead, it could only do a little work. But in my opinion, designing some of instructions is a work from zero to one, which is more difficult than do some jobs from one to n. 
 * After all, this is just a toy, not a product. The purpose I wrote it is to learn the mechanism of compiler and virtual machine.
 */

#define VM_H

#include <cassert>
#include <algorithm>
#include <iterator>

#ifndef OPCODE_H
#include "opcode.hpp"
#endif

namespace CS {
    namespace VM {
        using namespace OpCode;

        class OpStack {
            public:
                OpStack(): top_(-1), capacity_(16) {
                    data_ = new int[capacity_];
                }

                ~OpStack() {
                    delete[] data_;
                }

                int& operator[] (int index) {
                    return data_[index];
                }

                int Size() const {
                    return top_;
                }

                int& Top() {
                    return data_[top_];
                }

                int& Top2() {
                    return data_[top_ - 1];
                }

                void Push(int x) {
                    if (top_ >= capacity_) ReAlloc();
                    data_[++top_] = x;
                }

                int Pop() {
                    return data_[top_--];
                }

            private:
                void ReAlloc() {
                    int old_capacity = capacity_;
                    int* old_data = data_;

                    capacity_ *= 2;
                    data_ = new int[capacity_];
                    memcpy(data_, old_data, sizeof(int) * old_capacity);
                    delete[] old_data;
                }

                int* data_;
                int top_;
                int capacity_;
        };
        class VM {
            public:
                VM(): sym_tbl_(), ins_tbl_(), pc_(0) {
                }
                ~VM() {}

                void LoadSymbolTable(const SymbolTable& sym_tbl) {
                    sym_tbl_.insert(sym_tbl.begin(), sym_tbl.end());
                }

                void Execute(const InstructionTable& ins_tbl) {
                    ins_tbl_.insert(ins_tbl_.end(),
                            ins_tbl.begin(), ins_tbl.end());
                    Run();
                }

            private:
                void Run() {
                    while (pc_ < ins_tbl_.size()) {
                    long long ins = ins_tbl_[pc_++];
#ifndef NDEBUG
                    printf("%016llx\n", ins);
#endif
                    std::pair<int, int> op_val = SplitOpCode(ins);
                    int op = op_val.first;
                    int val = op_val.second;
                    
                    switch (op) {
                        case 1:
                            stack_.Push(val);
                            break;
                        case 2:
                            stack_.Pop();
                            break;
                        case 3:
                            stack_[val] = stack_.Top();
                            break;
                        case 4:
                            stack_.Push(stack_[val]);
                        case 5:
                            for (int i = 0; i < val/sizeof(int); i++)
                                stack_.Push(0);
                            break;
                        case 6:
                        case 7:
                        case 8:
                            
                        case 20:
                            stack_.Top2() += stack_.Top();
                            stack_.Pop();
                            break;
                        case 21:
                            stack_.Top2() -= stack_.Top();
                            stack_.Pop();
                            break;
                        case 22:
                            stack_.Top2() *= stack_.Top();
                            stack_.Pop();
                            break;
                        case 23:
                            stack_.Top2() /= stack_.Top();
                            stack_.Pop();
                            break;
                        case 24:
                        case 25:
                        /* function call */
                        case 30:
                            /* have not finish this part */
                            puts("call");
                            break;
                        case 31:
                            pc_ = val;
                            break;
                        default:
                            puts("unknown opcode");
                            assert(false);
                    }
                    }
                }

                SymbolTable sym_tbl_;
                InstructionTable ins_tbl_;
                int pc_;
                OpStack stack_;
        };
    }
}
