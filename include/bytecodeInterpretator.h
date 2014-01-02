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

namespace mathvm {
    class BytecodeInterpretator {
    public:
        Status* interpretate(const BytecodeCode& code, vector<Var*>& vars);
    };
}


#endif	/* BYTECODEINTERPRETATOR_H */

