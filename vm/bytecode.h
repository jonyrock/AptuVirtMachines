#ifndef BYTECODE_H
#define	BYTECODE_H

#include "mathvm.h"

namespace mathvm {
    class ByteCode: public Code {
    public:
        virtual Status* execute(vector<Var*>& vars) {
            return NULL;
        }
    };
}

#endif

