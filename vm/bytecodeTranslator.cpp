#include "bytecodeTranslator.h"
#include "mathvm.h"
#include "parser.h"
#include "bytecodeCode.h"
#include "ast.h"

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
        BytecodeAstVisitor visitor(code);
        visitor.visitAst(parser.top());

        *code_ = code;
        return new Status();
    }

    void BytecodeAstVisitor::visitAst(AstFunction* rootFunction) {
        visitAstFunction(rootFunction);
        currentBytecode()->addInsn(Instruction.BC_STOP);
    }

    void BytecodeAstVisitor::visitAstFunction(AstFunction* function) {
        BytecodeFunction* fun = new BytecodeFunction(function);
        code->addFunction(fun);
        functionsStack.push(fun);
        function->node()->visit(this);
        functionsStack.pop();
    }

    void BytecodeAstVisitor::visitFunctionNode(FunctionNode* node) {
        node->body()->visit(this);
    }

    void BytecodeAstVisitor::visitBlockNode(BlockNode* node) {
        
        Scope::VarIterator varIt(node->scope());
        while (varIt.hasNext()) {
            AstVar* var = varIt.next();
            cout << typeToName(var->type()) << " " << var->name() << ";" << endl;
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

