#include "bytecodeTranslator.h"
#include "mathvm.h"
#include "parser.h"
#include "bytecodeCode.h"
#include "ast.h"
#include "AsmJit/Build.h"

#include <iostream>


using namespace std;

namespace mathvm {

    Status* BytecodeTranslator::translate(const string& program, Code** code_) {
        Parser parser;
        Status* status = parser.parseProgram(program);

        if (status != NULL && status->isError()) {
            return status;
        }

        BytecodeCode* code = new BytecodeCode();
        BytecodeAstVisitor visitor(*code);
        visitor.visitAst(parser.top());

        *code_ = code;
        return new Status();
    }

    void BytecodeAstVisitor::visitAst(AstFunction* rootFunction) {
        visitAstFunction(rootFunction);
    }

    void BytecodeAstVisitor::visitAstFunction(AstFunction* function) {
        BytecodeFunction* fun = new BytecodeFunction(function);
        //        scopeFunctions[make_pair(function->owner(), fun->name())] = functionsCount++;
        code.addFunction(fun);
        functionsStack.push(fun);
        function->node()->visit(this);
        functionsStack.pop();
    }

    void BytecodeAstVisitor::visitFunctionNode(FunctionNode* node) {
        node->body()->visit(this);
    }

    uint16_t BytecodeAstVisitor::allocate(const string& str) {
        currentBytecode()->addInsn(BC_STR_BEGIN);
        uint16_t strIndex = (uint16_t)current() + 1;
        for (int i = 0; i < str.size(); i++) {
            add((uint8_t) str[i]);
        }
        add((uint8_t)'$');
        return strIndex;
    }

    void BytecodeAstVisitor::visitBlockNode(BlockNode* node) {

        Scope::VarIterator varIt(node->scope());
        while (varIt.hasNext()) {
            AstVar* var = varIt.next();
            allocateVar(*var);
            //            cout << "allocated" << scopeVars[make_pair(node->scope(), var->name())];
        }

        Scope::FunctionIterator funIt(node->scope());
        while (funIt.hasNext()) {
            AstFunction* fun = funIt.next();
            visitAstFunction(fun);
        }

        for (uint32_t i = 0; i < node->nodes(); i++) {
            node->nodeAt(i)->visit(this);
        }

    }

    uint16_t BytecodeAstVisitor::allocateVar(AstVar& var) {
        uint32_t beginIndex = current() + 1;
        astVarsId[&var] = beginIndex;
        if (var.type() == VT_DOUBLE) {
            addInsn(BC_DLOAD);
            currentBytecode()->addDouble(0);
            return beginIndex;
        }
        if (var.type() == VT_INT) {
            addInsn(BC_ILOAD);
            currentBytecode()->addInt64(0);
            return beginIndex;
        }
        if (var.type() == VT_STRING) {
            addInsn(BC_SLOAD);
            currentBytecode()->addUInt16(0);
            return beginIndex;
        }
        assert(false);
    }

    void BytecodeAstVisitor::visitBinaryOpNode(BinaryOpNode* node) {
    }

    void BytecodeAstVisitor::visitCallNode(CallNode* node) {
    }
   
    void BytecodeAstVisitor::visitForNode(ForNode* node) {
    }

    void BytecodeAstVisitor::visitIfNode(IfNode* node) {
    }

    void BytecodeAstVisitor::visitLoadNode(LoadNode* node) {
    }

    void BytecodeAstVisitor::visitNativeCallNode(NativeCallNode* node) {
    }

    void BytecodeAstVisitor::visitPrintNode(PrintNode* node) {
    }

    void BytecodeAstVisitor::visitReturnNode(ReturnNode* node) {
    }

    void BytecodeAstVisitor::visitStoreNode(StoreNode* node) {
        uint16_t varId = astVarsId[node->var()];
        node->value()->visit(this);
        goto STORE_TO_VAR;
        
STORE_TO_VAR:
        if (node->var()->type() == VT_DOUBLE)
            addInsn(BC_STOREDVAR);
        if (node->var()->type() == VT_INT)
            addInsn(BC_STOREIVAR);
        if (node->var()->type() == VT_STRING)
            addInsn(BC_STORESVAR);
    }
    
    void BytecodeAstVisitor::visitDoubleLiteralNode(DoubleLiteralNode* node) {
        currentBytecode()->addDouble(node->literal());
    }
    
    void BytecodeAstVisitor::visitIntLiteralNode(IntLiteralNode* node) {
        currentBytecode()->addInt64(node->literal());
    }
    
    void BytecodeAstVisitor::visitStringLiteralNode(StringLiteralNode* node) {
        addInsn(BC_STR_BEGIN);
        uint16_t allocatedId = allocate(node->literal());
        currentBytecode()->addInt16(allocatedId);
    }
    
    void BytecodeAstVisitor::visitUnaryOpNode(UnaryOpNode* node) {
    }

    void BytecodeAstVisitor::visitWhileNode(WhileNode* node) {
    }




}

