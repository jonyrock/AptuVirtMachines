#include "bytecodeTranslator.h"
#include "mathvm.h"
#include "parser.h"
#include "bytecodeCode.h"
#include "ast.h"

#include <iostream>


using namespace std;

namespace mathvm {

    Status* BytecodeTranslator::translate(const string& program, Code** code) {
        Parser parser;
        Status* status = parser.parseProgram(program);

        if (status != NULL && status->isError()) {
            return status;
        }

        *code = new BytecodeCode();
        BytecodeAstVisitor visitor(*code);
        visitor.visitTopFunction(parser->top());

        return new Status();

    }

    void BytecodeAstVisitor::visitTopFunction(AstFunction* rootFunction) {

        //BlockNode* node = rootFunction->node()->body();
        cout << "translator started" << endl;
        visitAstFunction(rootFunction);

    }

    void BytecodeAstVisitor::visitAstFunction(AstFunction* function) {
        TranslatedFunction fun = new TranslatedFunction(function);
        functionsStack.push(fun);
    }

    void BytecodeAstVisitor::visitFunctionNode(FunctionNode* node) {



    }


}

