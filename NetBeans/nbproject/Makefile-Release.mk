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
CND_PLATFORM=Cygwin-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/f3c4c907/BookTests.o \
	${OBJECTDIR}/_ext/f3c4c907/DaemonTests.o \
	${OBJECTDIR}/_ext/f3c4c907/RecordTableTests.o \
	${OBJECTDIR}/_ext/f3c4c907/TerminalAndMirrorTests.o \
	${OBJECTDIR}/_ext/f3c4c907/UtilsTests.o \
	${OBJECTDIR}/_ext/f3c4c907/main.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetbeans.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetbeans.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetbeans.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetbeans.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetbeans.a

${OBJECTDIR}/_ext/f3c4c907/BookTests.o: ../Source/KabukiTheater/_/TESTS/BookTests.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f3c4c907
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f3c4c907/BookTests.o ../Source/KabukiTheater/_/TESTS/BookTests.cpp

${OBJECTDIR}/_ext/f3c4c907/DaemonTests.o: ../Source/KabukiTheater/_/TESTS/DaemonTests.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f3c4c907
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f3c4c907/DaemonTests.o ../Source/KabukiTheater/_/TESTS/DaemonTests.cpp

${OBJECTDIR}/_ext/f3c4c907/RecordTableTests.o: ../Source/KabukiTheater/_/TESTS/RecordTableTests.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f3c4c907
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f3c4c907/RecordTableTests.o ../Source/KabukiTheater/_/TESTS/RecordTableTests.cpp

${OBJECTDIR}/_ext/f3c4c907/TerminalAndMirrorTests.o: ../Source/KabukiTheater/_/TESTS/TerminalAndMirrorTests.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f3c4c907
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f3c4c907/TerminalAndMirrorTests.o ../Source/KabukiTheater/_/TESTS/TerminalAndMirrorTests.cpp

${OBJECTDIR}/_ext/f3c4c907/UtilsTests.o: ../Source/KabukiTheater/_/TESTS/UtilsTests.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f3c4c907
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f3c4c907/UtilsTests.o ../Source/KabukiTheater/_/TESTS/UtilsTests.cpp

${OBJECTDIR}/_ext/f3c4c907/main.o: ../Source/KabukiTheater/_/TESTS/main.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f3c4c907
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f3c4c907/main.o ../Source/KabukiTheater/_/TESTS/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
