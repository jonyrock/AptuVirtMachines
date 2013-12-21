#ifndef BYTECODETRASLATOR_H
#define	BYTECODETRASLATOR_H

#include "mathvm.h"
#include "visitors.h"
#include "bytecodeCode.h"
#include <string>
#include <stack>
#include <map>

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

    class BytecodeAstVisitor : public AstVisitor {
        friend BytecodeTranslator;
        BytecodeCode& code;
        stack<BytecodeFunction*> functionsStack;
        Status* status;
    public:

        BytecodeAstVisitor(BytecodeCode& code_) : code(code_), status(NULL) {
        }

        void visitAst(AstFunction*);
        bool beforeVisit();

#define VISITOR_FUNCTION(type, name) \
        virtual void visit##type##_(type* node); \
        inline virtual void visit##type(type* node){ \
                if(beforeVisit()) \
                        return; \
                visit##type##_(node); \
        } 
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

        void addTypedOpInsn(VarType type, TokenKind op);

        inline VarType topType() {
            return typesStack.top();
        }
        
        void loadVar(const AstVar* var);
        
        inline void ensureType(VarType ts, VarType td) {
            ensureType(ts, td, current());
            addInsn(BC_INVALID);
        }
        
        void ensureType(VarType ts, VarType td, uint32_t pos);
            
        

        uint16_t currentContext; // aka function

    };


}

#endif

