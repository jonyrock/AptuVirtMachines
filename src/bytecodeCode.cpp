
#include "bytecodeCode.h"

#include "bytecodeInterpretator.h"

namespace mathvm{
    
    Status* BytecodeCode::execute(vector<Var*>& vars){
        BytecodeInterpretator inp;
        inp.interpretate(*this, vars);
    }
    
}