
#include "bytecodeInterpretator.h"

namespace mathvm {

    Status* BytecodeInterpretator::interpretate(const BytecodeCode& code,
            vector<Var*>& vars) {
        Code::FunctionIterator fi(&code);
        while (fi.hasNext()) {
            functions.push_back((BytecodeFunction*) fi.next());
        }
        
        Code::ConstantIterator ci(&code);
        while (ci.hasNext()) {
            constants.push_back(&ci.next());
        }

        return NULL;

    }

    void BytecodeInterpretator::execFunction(const BytecodeFunction& fun) {



    }

}