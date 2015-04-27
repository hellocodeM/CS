
#define OPCODE_H

namespace CS {

class Instruction {
    public:

    private:
};

class StackModel {
    public:
        StackModel() {}

        StackModel(const vector<Instruction>& instructions) {

        }

        OpCode Dump() const {

        }

        void Push(int i) {

        }

        void Pop() {

        }

        void Top() {

        }
        

        void Mov(int dst, int src) {
        }

        void Assign(int dst, int val) {

        }

        void Call() {

        }

        /* numeric operation */
        void Add() {

        }
        
        void Sub() {

        }

        void Mul() {

        }

        void Div() {

        }

        void Mod() {

        }
        

    private:
        vector<Instruction>
};
}
