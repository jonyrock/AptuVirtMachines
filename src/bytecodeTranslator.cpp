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
        scopeFunctions[make_pair(function->owner(), fun->name())] = functionsCount++;
        code.addFunction(fun);
        functionsStack.push(fun);
        function->node()->visit(this);
        functionsStack.pop();
    }

    void BytecodeAstVisitor::visitFunctionNode(FunctionNode* node) {
        node->body()->visit(this);
    }

    void BytecodeAstVisitor::writeIntoBytecode(const string& str) {
        //        addInsn(BC_RAWDATA_BEGIN);
        for (int i = 0; i < str.size(); i++) {
            add((uint8_t) str[i]);
        }
        //        addInsn(BC_RAWDATA_END);
    }

    void BytecodeAstVisitor::writeIntoBytecode(const uint32_t& value) {

        union {
            uint32_t myValue;
            uint8_t myBytes [4];
        };
        myValue = value;
        add(myBytes[0]);
        add(myBytes[1]);
        add(myBytes[2]);
        add(myBytes[3]);
    }

    void BytecodeAstVisitor::visitBlockNode(BlockNode* node) {

        Scope::VarIterator varIt(node->scope());
        while (varIt.hasNext()) {
            AstVar* var = varIt.next();
            uint32_t varIndex = allocateVar(*var);
            scopeVars[make_pair(node->scope(), var->name())] = varIndex;
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

    uint32_t BytecodeAstVisitor::allocateVar(const AstVar& var){
        uint32_t allocateIndex =  current() + 1;
        add((uint8_t)var.type());
        writeIntoBytecode((uint32_t)0);
        if(var.type() == VT_DOUBLE)
            writeIntoBytecode((uint32_t)0);
        return allocateIndex;
    }

    void BytecodeAstVisitor::visitBinaryOpNode(BinaryOpNode* node) {
    }

    void BytecodeAstVisitor::visitCallNode(CallNode* node) {
    }

    void BytecodeAstVisitor::visitDoubleLiteralNode(DoubleLiteralNode* node) {
    }

    void BytecodeAstVisitor::visitForNode(ForNode* node) {
    }

    void BytecodeAstVisitor::visitIfNode(IfNode* node) {
    }

    void BytecodeAstVisitor::visitIntLiteralNode(IntLiteralNode* node) {
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
    }

    void BytecodeAstVisitor::visitStringLiteralNode(StringLiteralNode* node) {
    }

    void BytecodeAstVisitor::visitUnaryOpNode(UnaryOpNode* node) {
    }

    void BytecodeAstVisitor::visitWhileNode(WhileNode* node) {
    }




}

