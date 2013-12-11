#include "bytecodeTranslator.h"
#include "mathvm.h"
#include "parser.h"
#include "visitors.h"
#include "bytecode.h"

#include <iostream>

using namespace mathvm;
using namespace std;

void visitNode() {
    
}

Status* BytecodeTranslator::translate(const string& program, Code** code){
    Parser parser;
    Status* status = parser.parseProgram(program);

    if(status != NULL && status->isError()){
        return status;
    }
    
    *code = new ByteCode();
    

    return new Status();

}

