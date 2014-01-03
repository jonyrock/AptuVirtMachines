/* 
 * File:   bytecodeInterpretator.h
 * Author: alex
 *
 * Created on January 2, 2014, 9:22 PM
 */

#ifndef BYTECODEINTERPRETATOR_H
#define	BYTECODEINTERPRETATOR_H

#include "mathvm.h"
#include "bytecodeCode.h"
#include <map>

namespace mathvm {

    using namespace std;

    class DataBytecode : public Bytecode {
    public:
        inline uint8_t popInsn() {
//            uint
        }

        inline uint16_t popUInt16() {
        }

        inline int64_t popInt64() {
        }
        
        inline uint64_t popUInt64() {
        }

        inline double popDouble() {
        }

        inline void dropTo(size_t to) {
        }

    };

    struct FunctionContex {
        map<uint16_t, double> dvars;
        map<uint16_t, int64_t> ivars;
        map<uint16_t, uint16_t> svars;
    };

    class BytecodeInterpretator {
        DataBytecode dstack;
        vector<const BytecodeFunction*> functions;
        vector<const string*> constants;

        void execFunction(const BytecodeFunction* fun);

        Status* execStatus;

    public:
        Status* interpretate(const BytecodeCode& code, vector<Var*>& vars);
    };
}


#endif	/* BYTECODEINTERPRETATOR_H */

