#include "bytecodeTranslator.h"
#include "mathvm.h"
#include "parser.h"
#include "bytecodeCode.h"
#include "ast.h"
#include "AsmJit/Build.h"

#include <iostream>
#include <stdexcept>


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
        currentContext = code.addFunction(fun);
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
            allocateVar(*var);
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
        astVarsContext[&var] = currentContext;
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
            addId(0);
            return beginIndex;
        }
        assert(false);
    }

    void BytecodeAstVisitor::addTypeInsn(VarType type, TokenKind op) {
        uint32_t codeLenBefore = currentBytecode()->length();
        if (op == tADD) {
            if (type == VT_INT)
                addInsn(BC_IADD);
            if (type == VT_DOUBLE)
                addInsn(BC_DADD);
            return;
        }

        if (op == tSUB) {
            if (type == VT_INT)
                addInsn(BC_ISUB);
            if (type == VT_DOUBLE)
                addInsn(BC_DSUB);
        }

        if (op == tMUL) {
            if (type == VT_INT)
                addInsn(BC_IMUL);
            if (type == VT_DOUBLE)
                addInsn(BC_DMUL);
        }

        if (op == tDIV) {
            if (type == VT_INT)
                addInsn(BC_IDIV);
            if (type == VT_DOUBLE)
                addInsn(BC_DDIV);
        }

        assert((currentBytecode()->length()) != codeLenBefore);
    }

    int typeAbstract(VarType t) {
        if (t == VT_INT)
            return 1;
        if (t == VT_DOUBLE)
            return 2;
        if (t == VT_STRING)
            return 3;
        throw invalid_argument("Undefined abstract order for type");
        return -1;
    }

    bool operator<(VarType a, VarType b) {
        return typeAbstract(a) < typeAbstract(b);
    }

    void BytecodeAstVisitor::ensureType(VarType ts, VarType td) {
        if (ts == td)
            return;

        if (ts == VT_INT && td == VT_DOUBLE) {
            addInsn(BC_I2D);
        }
        if (ts == VT_DOUBLE && td == VT_INT) {
            addInsn(BC_D2I);
        }
        if (ts == VT_STRING && td == VT_INT) {
            addInsn(BC_S2I);
        }

        if (ts > td)
            throw invalid_argument("Can't convert type");
    }

    void BytecodeAstVisitor::visitBinaryOpNode(BinaryOpNode* node) {

        node->left()->visit(this);
        VarType leftType = topType();

        node->right()->visit(this);
        VarType rightType = topType();

        VarType maxType = max(leftType, rightType);
        //        ensureType(leftType, maxType);
        //        ensureType(rightType, maxType);
        addTypeInsn(maxType, node->kind());


        typesStack.push(maxType);

    }

    void BytecodeAstVisitor::visitCallNode(CallNode* node) {
        // TODO: think about type
        typesStack.push(VT_INT);
    }

    void BytecodeAstVisitor::visitForNode(ForNode* node) {
    }

    void BytecodeAstVisitor::visitIfNode(IfNode* node) {
    }

    void BytecodeAstVisitor::visitLoadNode(LoadNode* node) {
        if (node->var()->type() == VT_DOUBLE) {
            addInsn(BC_LOADCTXDVAR);
        }
        if (node->var()->type() == VT_INT) {
            addInsn(BC_LOADCTXIVAR);
        }
        if (node->var()->type() == VT_STRING) {
            addInsn(BC_LOADCTXSVAR);
        }
        addId(astVarsContext[node->var()]);
        addId(astVarsId[node->var()]);
        typesStack.push(node->var()->type());
    }

    void BytecodeAstVisitor::visitNativeCallNode(NativeCallNode* node) {
        typesStack.push(node->nativeSignature()[0].first);
    }

    void BytecodeAstVisitor::visitPrintNode(PrintNode* node) {
        for (size_t i = 0; i < node->operands(); i++) {
            AstNode* opn = node->operandAt(i);
            opn->visit(this);

            if (topType() == VT_INT) {
                addInsn(BC_IPRINT);
                continue;
            }
            if (topType() == VT_DOUBLE) {
                addInsn(BC_DPRINT);
                continue;
            }
            if (topType() == VT_STRING) {
                addInsn(BC_SPRINT);
                continue;
            }

            assert(false);

        }
        typesStack.push(VT_VOID);
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
        currentBytecode()->addInt16(varId);
    }

    void BytecodeAstVisitor::visitDoubleLiteralNode(DoubleLiteralNode* node) {
        addInsn(BC_DLOAD);
        currentBytecode()->addDouble(node->literal());
        typesStack.push(VT_DOUBLE);
    }

    void BytecodeAstVisitor::visitIntLiteralNode(IntLiteralNode* node) {
        addInsn(BC_ILOAD);
        currentBytecode()->addInt64(node->literal());
        typesStack.push(VT_INT);
    }

    void BytecodeAstVisitor::visitStringLiteralNode(StringLiteralNode* node) {
        addInsn(BC_SLOAD);
        uint16_t strId = code.makeStringConstant(node->literal());
        addId(strId);
        typesStack.push(VT_STRING);
    }

    void BytecodeAstVisitor::visitUnaryOpNode(UnaryOpNode* node) {

    }

    void BytecodeAstVisitor::visitWhileNode(WhileNode* node) {
        typesStack.push(VT_VOID);
    }

}

