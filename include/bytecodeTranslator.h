#ifndef BYTECODETRASLATOR_H
#define	BYTECODETRASLATOR_H

#include "mathvm.h"
#include "visitors.h"
#include "bytecodeCode.h"
#include <string>
#include <stack>
#include <map>

namespace mathvm {

    class BytecodeAstVisitor : public AstVisitor {
        BytecodeCode& code;
        stack<BytecodeFunction*> functionsStack;
    public:

        BytecodeAstVisitor(BytecodeCode& code_) : code(code_){
        }

        void visitAst(AstFunction*);

#define VISITOR_FUNCTION(type, name) \
        virtual void visit##type(type* node);
        FOR_NODES(VISITOR_FUNCTION)
#undef VISITOR_FUNCTION

        void visitAstFunction(AstFunction*);

    private:

        inline BytecodeFunction* currentFunction() {
            return functionsStack.top();
        }

        inline Bytecode* currentBytecode() {
            return currentFunction()->bytecode();
        }

        inline void addInsn(Instruction insn) {
            currentBytecode()->addInsn(insn);
        }

        inline void add(uint8_t b) {
            currentBytecode()->add(b);
        }

        inline uint32_t current() {
            return currentBytecode()->current();
        }

        uint16_t allocateVar(AstVar& var);

        map<const AstVar*, uint16_t> astVarsId;
        
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

