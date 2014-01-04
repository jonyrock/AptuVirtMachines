
#include "bytecodeInterpretator.h"

#include <iostream>

using namespace std;

namespace mathvm {

    Status* BytecodeInterpretator::interpretate(const BytecodeCode& code,
            vector<Var*>& vars) {
        Code::FunctionIterator fi(&code);
        while (fi.hasNext()) {
            functions.push_back((BytecodeFunction*) fi.next());
        }

        Code::ConstantIterator ci(&code);
        while (ci.hasNext()) {
            constants.push_back(&ci.next());
        }

        if (functions.empty()) {
            return new Status("Nothing to execute", 0);
        }

        execStatus = NULL;

        execFunction(functions[0]);

        return execStatus;
    }

    void BytecodeInterpretator::execFunction(const BytecodeFunction* fun) {

        const Bytecode& b = *(fun->bytecode());
        DataBytecode& d = dstack;

        size_t len = b.length();

        FunctionContex context;

        double dv;
        int64_t iv;
        uint16_t idv;
        uint16_t idv2;

        for (size_t bci = 0; bci < len;) {
            size_t length;
            Instruction insn = b.getInsn(bci);
            const char* name = bytecodeName(insn, &length);
            switch (insn) {

                    // STACK LOAD (new vars)
                case BC_DLOAD:
                    dv = b.getDouble(bci + 1);
                    d.addDouble(dv);
                    context.dvars[bci + 1] = dv;
                    dstack.addDouble(dv);
                    break;
                case BC_ILOAD:
                    iv = b.getInt64(bci + 1);
                    d.addInt64(iv);
                    context.ivars[bci + 1] = iv;
                    dstack.addInt64(iv);
                    break;
                case BC_SLOAD:
                    idv = b.getUInt16(bci + 1);
                    d.addUInt16(idv);
                    context.svars[bci + 1] = idv;
                    dstack.addUInt16(idv);
                    break;

                    // VAR  LOADS
                case BC_LOADIVAR:
                    idv = b.getUInt16(bci + 1);
                    dstack.addInt64(context.ivars[idv]);
                    break;
                case BC_LOADDVAR:
                    idv = b.getUInt16(bci + 1);
                    dstack.addDouble(context.dvars[idv]);
                    break;

                case BC_LOADSVAR:
                    idv = b.getUInt16(bci + 1);
                    dstack.addUInt16(context.svars[idv]);
                    break;

                    // VAR STORES
                case BC_STOREDVAR:
                    idv = b.getUInt16(bci + 1);
                    dv = dstack.popDouble();
                    context.dvars[idv] = dv;
                    break;

                case BC_STOREIVAR:
                    idv = b.getUInt16(bci + 1);
                    iv = dstack.popInt64();
                    context.ivars[idv] = iv;
                    break;
                    
                case BC_STORESVAR:
                    idv = b.getUInt16(bci + 1);
                    idv2 = dstack.popUInt16();
                    context.svars[idv] = idv2;
                    break;
                    
                    // VAR LOAD (outer context)
                case BC_LOADCTXDVAR:
                case BC_LOADCTXIVAR:
                case BC_LOADCTXSVAR:
                    
                    // VAR STORE (outer context)
                case BC_STORECTXDVAR:
                case BC_STORECTXIVAR:
                case BC_STORECTXSVAR:
                    cout << name << " @" << b.getUInt16(bci + 1)
                            << ":" << b.getUInt16(bci + 3);
                    break;
                case BC_IFICMPNE:
                case BC_IFICMPE:
                case BC_IFICMPG:
                case BC_IFICMPGE:
                case BC_IFICMPL:
                case BC_IFICMPLE:
                case BC_JA:
                    cout << name << " " << b.getInt16(bci + 1) + bci + 1;
                    break;

                case BC_CALL:
                    idv = b.getUInt16(bci + 1);
                    execFunction(functions[idv]);
                    break;
                case BC_CALLNATIVE:
                    execStatus = new Status("Can't call a native function", 0);
                    return;
                    break;

                case BC_RETURN:
                    goto RETURN;
                    break;
                default:
                    cout << name << endl;
//                    assert(false);
            }
            cout << endl;
            bci += length;
        }

RETURN:
        return;

    }


}