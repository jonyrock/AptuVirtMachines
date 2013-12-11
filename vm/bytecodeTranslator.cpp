#include "bytecodeTranslator.h"
#include "mathvm.h"
#include "parser.h"
#include "visitors.h"

#include <iostream>

using namespace mathvm;
using namespace std;


Status* BytecodeTranslator::translate(const string& program, Code** code){
    
    cout << INVALID_ID << endl;
    Parser parser;
//    Status* status = parser.parseProgram(program);
//
//    if(status->isError()){
//        return status;
//    }
//    
//    return status;
    return 0;

}

