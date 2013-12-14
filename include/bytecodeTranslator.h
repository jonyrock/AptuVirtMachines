#ifndef BYTECODETRASLATOR_H
#define	BYTECODETRASLATOR_H

#include "mathvm.h"
#include "visitors.h"
#include "bytecodeCode.h"
#include <string>
#include <stack>

namespace mathvm {

    class BytecodeAstVisitor : public AstVisitor {
        BytecodeCode * const code;
        stack<BytecodeFunction*> functionsStack;
    public:

        BytecodeAstVisitor(BytecodeCode * const code_) : code(code_) {
        }

        void visitAst(AstFunction*);

#define VISITOR_FUNCTION(type, name) \
        virtual void visit##type(type* node);
        FOR_NODES(VISITOR_FUNCTION)
#undef VISITOR_FUNCTION

        void visitAstFunction(AstFunction*);

        inline BytecodeFunction* currentFunction() {
            return functionsStack.top();
        }
        
        inline Bytecode* currentBytecode() {
            return currentFunction()->bytecode();
        }

    };

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

