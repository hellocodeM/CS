/**
 * Save the instructions to the file, which is called opcode.
 * And deserialize it to instructions. The instructions are defined at opcode.hpp.
 */

#include <cstdio>

#define SERIALIZER_H

#ifndef OPCODE_H
#include "opcode.hpp"
#endif

namespace CS {
    namespace Serializer {
        using OpCode::InstructionTable;
        using OpCode::SymbolTable;

        static void Serialize(std::pair<InstructionTable*, SymbolTable*> is, 
                const char* file_path) {
            /*
             * size_t size_t: symboltable size, instructiontable size
             * several lines of string and int: symboltable
             * several lines of long long: instructiontable
             */
            
            FILE* output = fopen(file_path, "w");
            assert(output != nullptr);

            fprintf(output, "%zu %zu\n", is.second->size(), is.first->size());
            for (auto& i: *(is.second)) {
                fprintf(output, "%s %d\n", i.first.data(), i.second);
            }

            for (auto i: *(is.first)) {
                fprintf(output, "%lld\n", i);
            }
            fclose(output);
        }

        static std::pair<InstructionTable*, SymbolTable*>
            Deserialize(const char* file_path) {
                FILE* input = fopen(file_path, "r");
                assert(input != nullptr);
                size_t symb_size, inst_size;
                auto inst_tbl = new InstructionTable();
                auto symb_tbl = new SymbolTable();
                char buff[100];
                int size;
                long long lsize;

                fscanf(input, "%zu %zu", &symb_size, &inst_size);
                for (int i = 0; i < symb_size; i++) {
                    fscanf(input, "%s %d", buff, &size);
                    symb_tbl->emplace(buff, size);
                }
                for (int i = 0; i < inst_size; i++) {
                    fscanf(input, "%lld", &lsize);
                    inst_tbl->push_back(lsize);
                }
                
                fclose(input);
                return std::make_pair(inst_tbl, symb_tbl);
            }
    }
}
