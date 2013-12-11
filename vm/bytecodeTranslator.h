#ifndef BYTECODETRASLATOR_H
#define	BYTECODETRASLATOR_H

#include "mathvm.h"
#include <string>

namespace mathvm {

    class BytecodeTranslator : public Translator {
        Status* translateBytecode(
                const string& program,
                Code** code);

    public:

        BytecodeTranslator() {
        }

        virtual ~BytecodeTranslator() {
        }

        virtual Status* translate(const string& program, Code** code);

    };
}

#endif

