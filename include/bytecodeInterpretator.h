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

        int64_t popi() {
            return pop<int64_t>();
        }
        void pushi(int64_t v) {
            push<int64_t>(v);
        }

        double popd() {
            return pop<double>();
        }
        
        void pushd(double v) {
            push<double>(v);
        }

        uint16_t popid() {
            return pop<uint16_t>();
        }
        
        void pushid(uint16_t v){
            push<uint16_t>(v);
        }

        template<class T> T pop() {
            T value = getTyped<T>(length() - (uint32_t)sizeof (T));
            _data.resize(length() - (uint32_t)sizeof (T));
            return value;
        }

        template<class T> void push(T value) {
            addTyped(value);
        }

        inline void dropToSize(size_t to) {
            _data.resize(to);
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

