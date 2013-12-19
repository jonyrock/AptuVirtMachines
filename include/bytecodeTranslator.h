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

        BytecodeAstVisitor(BytecodeCode& code_) : code(code_) {
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

        inline void addId(uint16_t id) {
            currentBytecode()->addInt16(id);
        }

        inline uint32_t current() {
            return currentBytecode()->current();
        }

        uint16_t allocateVar(AstVar& var);

        map<const AstVar*, uint16_t> astVarsId;
        map<const AstVar*, uint16_t> astVarsContext;

        stack<VarType> typesStack;

        void addTypeInsn(VarType type, TokenKind op);

        inline VarType topType() {
            return typesStack.top();
        }

        void ensureType(VarType ts, VarType td);
        
        uint16_t currentContext; // aka function

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

