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
        return visitor.status;
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

    void BytecodeAstVisitor::visitFunctionNode_(FunctionNode* node) {
        node->body()->visit(this);
    }

    void BytecodeAstVisitor::visitBlockNode_(BlockNode* node) {

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

    void BytecodeAstVisitor::addTypedOpInsn(VarType type, TokenKind op) {
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

    void BytecodeAstVisitor::ensureType(VarType ts, VarType td, uint32_t pos) {
        if (ts == td)
            return;

        if (ts == VT_INT && td == VT_DOUBLE) {
            currentBytecode()->set(pos, BC_I2D);
        }
        if (ts == VT_DOUBLE && td == VT_INT) {
            currentBytecode()->set(pos, BC_D2I);
        }
        if (ts == VT_STRING && td == VT_INT) {
            currentBytecode()->set(pos, BC_S2I);
        }

        if (ts > td)
            throw invalid_argument("Can't convert type");
    }

    void BytecodeAstVisitor::addTrueFalseJumpRegion(Instruction jumpInsn) {
        addInsn(jumpInsn);
        trueIdUnsettedPos = current();
        addId(0);
        addInsn(BC_JA);
        falseIdUnsettedPos = current();
        addId(0);
    }

    void BytecodeAstVisitor::visitBinaryLogicOpNode(BinaryOpNode* node) {

        if (node->kind() == tAND || node->kind() == tOR) {
            node->left()->visit(this);
            uint16_t leftTrueIdUnsettedPos = trueIdUnsettedPos;
            uint16_t leftFalseIdUnsettedPos = falseIdUnsettedPos;

            uint16_t rightBeginId = current();
            node->right()->visit(this);
            uint16_t rightTrueIdUnsettedPos = trueIdUnsettedPos;
            uint16_t rightFalseIdUnsettedPos = falseIdUnsettedPos;

            addTrueFalseJumpRegion(BC_JA);

            if (node->kind() == tAND) {
                setJump(leftTrueIdUnsettedPos, rightBeginId);
                // jump located before jump id
                setJump(leftFalseIdUnsettedPos, falseIdUnsettedPos - 1);
            } else {
                setJump(leftTrueIdUnsettedPos, trueIdUnsettedPos - 1);
                setJump(leftFalseIdUnsettedPos, rightBeginId);
            }

            setJump(rightTrueIdUnsettedPos, trueIdUnsettedPos - 1);
            setJump(rightFalseIdUnsettedPos, falseIdUnsettedPos - 1);
            return;
        }

        assert(logicKindToJump.find(node->kind()) != logicKindToJump.end());
        node->left()->visit(this);
        node->right()->visit(this);
        addTrueFalseJumpRegion(logicKindToJump[node->kind()]);

    }

    void BytecodeAstVisitor::visitBinaryOpNode_(BinaryOpNode* node) {
        if (logicKinds.find(node->kind()) != logicKinds.end()) {
            visitBinaryLogicOpNode(node);
            return;
        }
        if (node->kind() == tRANGE) {
            node->left()->visit(this);
            node->right()->visit(this);
            return;
        }

        node->left()->visit(this);
        VarType leftType = topType();
        uint32_t leftCastPos = current();
        addInsn(BC_INVALID); // there will be type cast

        node->right()->visit(this);
        VarType rightType = topType();

        VarType maxType = max(leftType, rightType);
        ensureType(leftType, maxType, leftCastPos);
        ensureType(rightType, maxType);
        addTypedOpInsn(maxType, node->kind());
        typesStack.push(maxType);
    }

    void BytecodeAstVisitor::visitCallNode_(CallNode* node) {
        node->visitChildren(this);
        TranslatedFunction* fun = code.functionByName(node->name());
        if (fun == NULL) {
            status = new Status("Undefined function call", node->position());
            return;
        }
        assert(fun);
        addInsn(BC_CALL);
        typesStack.push(fun->returnType());
    }

    void BytecodeAstVisitor::visitForNode_(ForNode* node) {
        uint16_t topVar = 0;
        if (node->var()->type() == VT_INT) {
            addInsn(BC_ILOAD);
            topVar = current();
            currentBytecode()->addInt64(0);
        }

        if (node->var()->type() == VT_DOUBLE) {
            addInsn(BC_DLOAD);
            topVar = current();
            currentBytecode()->addDouble(0);
        }

        assert(topVar != 0);

        // TODO: check if var type and expression different
        node->inExpr()->visit(this);

        if (node->var()->type() == VT_INT) {
            addInsn(BC_STOREIVAR);
            addId(topVar);
            addInsn(BC_STOREIVAR);
        }
        if (node->var()->type() == VT_DOUBLE) {
            addInsn(BC_STOREDVAR);
            addId(topVar);
            addInsn(BC_STOREDVAR);
        }
        //        addId(astVarsContext[node->var()]);
        addId(astVarsId[node->var()]);


        uint16_t forConditionId = current();

        if (node->var()->type() == VT_INT) {
            addInsn(BC_LOADCTXIVAR);
            addId(currentContext);
            addId(astVarsId[node->var()]);
            addInsn(BC_LOADCTXIVAR);
            addId(currentContext);
            addId(topVar);
        }

        if (node->var()->type() == VT_DOUBLE) {
            addInsn(BC_LOADCTXDVAR);
            addId(currentContext);
            addId(astVarsId[node->var()]);
            addInsn(BC_LOADCTXDVAR);
            addId(currentContext);
            addId(topVar);
        }

        addTrueFalseJumpRegion(BC_IFICMPL);

        uint16_t bodyBegin = current();
        node->body()->visit(this);

        if (node->var()->type() == VT_INT) {
            addInsn(BC_LOADCTXIVAR);
            addId(currentContext);
            addId(astVarsId[node->var()]);
            addInsn(BC_ILOAD1);
            addInsn(BC_IADD);
            addInsn(BC_STOREIVAR);
            addId(astVarsId[node->var()]);
        }
        if (node->var()->type() == VT_DOUBLE) {
            addInsn(BC_LOADCTXDVAR);
            addId(currentContext);
            addId(astVarsId[node->var()]);
            addInsn(BC_DLOAD1);
            addInsn(BC_DADD);
            addInsn(BC_STOREDVAR);
            addId(astVarsId[node->var()]);
        }
        addInsn(BC_JA);
        addId(0);
        setJump(current() - 2, forConditionId);

        setTrueJump(bodyBegin);
        setFalseJump(current());
        addInsn(BC_INVALID);

    }

    void BytecodeAstVisitor::visitWhileNode_(WhileNode* node) {

        uint16_t whileCondition = current();
        node->whileExpr()->visit(this);

        uint16_t bodyBegin = current();
        node->loopBlock()->visit(this);
        addInsn(BC_JA);
        addId(0);
        setJump(current() - 2, whileCondition);

        setTrueJump(bodyBegin);
        setFalseJump(current());
        addInsn(BC_INVALID);

        typesStack.push(VT_VOID);
    }

    void BytecodeAstVisitor::visitIfNode_(IfNode* node) {

        addInsn(BC_JA);
        uint32_t ifBeginId = current();
        addId(0);
        node->thenBlock()->visit(this);
        addInsn(BC_JA);
        uint32_t thenEndId = current();
        addId(0);
        uint32_t elseBeginId = current();
        if (node->elseBlock() != NULL)
            node->elseBlock()->visit(this);
        addInsn(BC_JA);
        uint32_t elseEndId = current();
        addId(0);

        uint32_t ifExprBeginId = current();
        setJump(ifBeginId, ifExprBeginId);

        node->ifExpr()->visit(this);
        setTrueJump(ifBeginId + 2);
        setFalseJump(elseBeginId);

        uint32_t ifExprEndId = current();
        setJump(thenEndId, (uint16_t) ifExprEndId);
        setJump(elseEndId, (uint16_t) ifExprEndId);
        addInsn(BC_INVALID); // just idle

    }

    void BytecodeAstVisitor::loadVar(const AstVar* var) {
        if (var->type() == VT_DOUBLE) {
            addInsn(BC_LOADCTXDVAR);
        }
        if (var->type() == VT_INT) {
            addInsn(BC_LOADCTXIVAR);
        }
        if (var->type() == VT_STRING) {
            addInsn(BC_LOADCTXSVAR);
        }
        addId(astVarsContext[var]);
        addId(astVarsId[var]);
        typesStack.push(var->type());
    }

    void BytecodeAstVisitor::visitLoadNode_(LoadNode* node) {
        loadVar(node->var());
    }

    void BytecodeAstVisitor::visitNativeCallNode_(NativeCallNode* node) {
        typesStack.push(node->nativeSignature()[0].first);
    }

    void BytecodeAstVisitor::visitPrintNode_(PrintNode* node) {
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

    void BytecodeAstVisitor::visitReturnNode_(ReturnNode* node) {

    }

    void BytecodeAstVisitor::visitStoreNode_(StoreNode* node) {
        uint16_t varId = astVarsId[node->var()];
        node->value()->visit(this);
        if (node->op() == tINCRSET || node->op() == tDECRSET) {
            ensureType(topType(), node->var()->type());
            loadVar(node->var());
        }
        if (node->op() == tINCRSET) {
            addTypedOpInsn(node->var()->type(), tADD);
            goto STORE_TO_VAR;
        }

        if (node->op() == tDECRSET) {
            addInsn(BC_SWAP);
            addTypedOpInsn(node->var()->type(), tSUB);
            goto STORE_TO_VAR;
        }

        if (node->op() == tEQ)
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

    void BytecodeAstVisitor::visitDoubleLiteralNode_(DoubleLiteralNode* node) {
        addInsn(BC_DLOAD);
        currentBytecode()->addDouble(node->literal());
        typesStack.push(VT_DOUBLE);
    }

    void BytecodeAstVisitor::visitIntLiteralNode_(IntLiteralNode* node) {
        addInsn(BC_ILOAD);
        currentBytecode()->addInt64(node->literal());
        typesStack.push(VT_INT);
    }

    void BytecodeAstVisitor::visitStringLiteralNode_(StringLiteralNode* node) {
        addInsn(BC_SLOAD);
        uint16_t strId = code.makeStringConstant(node->literal());
        addId(strId);
        typesStack.push(VT_STRING);
    }

    void BytecodeAstVisitor::visitUnaryOpNode_(UnaryOpNode* node) {
        if (node->kind() == tNOT) {
            node->visitChildren(this);
            swap(trueIdUnsettedPos, falseIdUnsettedPos);
            return;
        }
        if (node->kind() == tSUB) {
            node->visitChildren(this);
            if (topType() == VT_INT) {
                addInsn(BC_ILOAD);
                currentBytecode()->addInt64(-1);
                addInsn(BC_IMUL);
                return;
            }
            if (topType() == VT_DOUBLE) {
                addInsn(BC_DLOAD);
                currentBytecode()->addDouble(-1.0);
                addInsn(BC_DMUL);
                return;
            }
        }

        assert(false);
    }

    bool BytecodeAstVisitor::beforeVisit() {
        if (status == NULL)
            return false;
        return true;
    }

}

