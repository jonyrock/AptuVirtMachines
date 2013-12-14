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

        BytecodeAstVisitor(BytecodeCode& code_) : code(code_),
        functionsCount(0) {
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

        inline void addCurrentIndex() {
            writeIntoBytecode(current());
        }

        uint32_t allocateVar(const AstVar& var);

        void writeIntoBytecode(const string& str);
        void writeIntoBytecode(const uint32_t& str);

        map<pair<Scope*, string>, uint32_t> scopeVars;
        map<pair<Scope*, string>, uint32_t> scopeFunctions;
        uint32_t functionsCount;

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

