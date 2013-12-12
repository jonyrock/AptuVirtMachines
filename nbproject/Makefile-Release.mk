#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/libs/AsmJit/Assembler.o \
	${OBJECTDIR}/libs/AsmJit/AssemblerX86X64.o \
	${OBJECTDIR}/libs/AsmJit/CodeGenerator.o \
	${OBJECTDIR}/libs/AsmJit/Compiler.o \
	${OBJECTDIR}/libs/AsmJit/CompilerX86X64.o \
	${OBJECTDIR}/libs/AsmJit/CpuInfo.o \
	${OBJECTDIR}/libs/AsmJit/Defs.o \
	${OBJECTDIR}/libs/AsmJit/DefsX86X64.o \
	${OBJECTDIR}/libs/AsmJit/Logger.o \
	${OBJECTDIR}/libs/AsmJit/MemoryManager.o \
	${OBJECTDIR}/libs/AsmJit/Operand.o \
	${OBJECTDIR}/libs/AsmJit/OperandX86X64.o \
	${OBJECTDIR}/libs/AsmJit/Platform.o \
	${OBJECTDIR}/libs/AsmJit/Util.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/vm/ast.o \
	${OBJECTDIR}/vm/bytecodeCode.o \
	${OBJECTDIR}/vm/bytecodeTranslator.o \
	${OBJECTDIR}/vm/interpreter.o \
	${OBJECTDIR}/vm/jit.o \
	${OBJECTDIR}/vm/mathvm.o \
	${OBJECTDIR}/vm/parser.o \
	${OBJECTDIR}/vm/scanner.o \
	${OBJECTDIR}/vm/translator.o \
	${OBJECTDIR}/vm/utils.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mymathvm

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mymathvm: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mymathvm ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/libs/AsmJit/Assembler.o: libs/AsmJit/Assembler.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/Assembler.o libs/AsmJit/Assembler.cpp

${OBJECTDIR}/libs/AsmJit/AssemblerX86X64.o: libs/AsmJit/AssemblerX86X64.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/AssemblerX86X64.o libs/AsmJit/AssemblerX86X64.cpp

${OBJECTDIR}/libs/AsmJit/CodeGenerator.o: libs/AsmJit/CodeGenerator.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/CodeGenerator.o libs/AsmJit/CodeGenerator.cpp

${OBJECTDIR}/libs/AsmJit/Compiler.o: libs/AsmJit/Compiler.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/Compiler.o libs/AsmJit/Compiler.cpp

${OBJECTDIR}/libs/AsmJit/CompilerX86X64.o: libs/AsmJit/CompilerX86X64.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/CompilerX86X64.o libs/AsmJit/CompilerX86X64.cpp

${OBJECTDIR}/libs/AsmJit/CpuInfo.o: libs/AsmJit/CpuInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/CpuInfo.o libs/AsmJit/CpuInfo.cpp

${OBJECTDIR}/libs/AsmJit/Defs.o: libs/AsmJit/Defs.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/Defs.o libs/AsmJit/Defs.cpp

${OBJECTDIR}/libs/AsmJit/DefsX86X64.o: libs/AsmJit/DefsX86X64.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/DefsX86X64.o libs/AsmJit/DefsX86X64.cpp

${OBJECTDIR}/libs/AsmJit/Logger.o: libs/AsmJit/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/Logger.o libs/AsmJit/Logger.cpp

${OBJECTDIR}/libs/AsmJit/MemoryManager.o: libs/AsmJit/MemoryManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/MemoryManager.o libs/AsmJit/MemoryManager.cpp

${OBJECTDIR}/libs/AsmJit/Operand.o: libs/AsmJit/Operand.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/Operand.o libs/AsmJit/Operand.cpp

${OBJECTDIR}/libs/AsmJit/OperandX86X64.o: libs/AsmJit/OperandX86X64.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/OperandX86X64.o libs/AsmJit/OperandX86X64.cpp

${OBJECTDIR}/libs/AsmJit/Platform.o: libs/AsmJit/Platform.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/Platform.o libs/AsmJit/Platform.cpp

${OBJECTDIR}/libs/AsmJit/Util.o: libs/AsmJit/Util.cpp 
	${MKDIR} -p ${OBJECTDIR}/libs/AsmJit
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/libs/AsmJit/Util.o libs/AsmJit/Util.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/vm/ast.o: vm/ast.cpp 
	${MKDIR} -p ${OBJECTDIR}/vm
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/vm/ast.o vm/ast.cpp

${OBJECTDIR}/vm/bytecodeCode.o: vm/bytecodeCode.cpp 
	${MKDIR} -p ${OBJECTDIR}/vm
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/vm/bytecodeCode.o vm/bytecodeCode.cpp

${OBJECTDIR}/vm/bytecodeTranslator.o: vm/bytecodeTranslator.cpp 
	${MKDIR} -p ${OBJECTDIR}/vm
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/vm/bytecodeTranslator.o vm/bytecodeTranslator.cpp

${OBJECTDIR}/vm/interpreter.o: vm/interpreter.cpp 
	${MKDIR} -p ${OBJECTDIR}/vm
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/vm/interpreter.o vm/interpreter.cpp

${OBJECTDIR}/vm/jit.o: vm/jit.cpp 
	${MKDIR} -p ${OBJECTDIR}/vm
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/vm/jit.o vm/jit.cpp

${OBJECTDIR}/vm/mathvm.o: vm/mathvm.cpp 
	${MKDIR} -p ${OBJECTDIR}/vm
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/vm/mathvm.o vm/mathvm.cpp

${OBJECTDIR}/vm/parser.o: vm/parser.cpp 
	${MKDIR} -p ${OBJECTDIR}/vm
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/vm/parser.o vm/parser.cpp

${OBJECTDIR}/vm/scanner.o: vm/scanner.cpp 
	${MKDIR} -p ${OBJECTDIR}/vm
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/vm/scanner.o vm/scanner.cpp

${OBJECTDIR}/vm/translator.o: vm/translator.cpp 
	${MKDIR} -p ${OBJECTDIR}/vm
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/vm/translator.o vm/translator.cpp

${OBJECTDIR}/vm/utils.o: vm/utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/vm
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/vm/utils.o vm/utils.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mymathvm

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
