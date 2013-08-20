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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/database.o \
	${OBJECTDIR}/income_statement_data.o \
	${OBJECTDIR}/income_statement_history.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/route.o \
	${OBJECTDIR}/route_data.o \
	${OBJECTDIR}/route_process.o \
	${OBJECTDIR}/sales_route_history.o \
	${OBJECTDIR}/staff.o


# C Compiler Flags
CFLAGS=-m64 -lm -lpthread `mysql_config --cflags --libs` 

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/airsim_v2

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/airsim_v2: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/airsim_v2 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/database.o: database.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -s -lm -lpthread `mysql_config --cflags --libs` -MMD -MP -MF $@.d -o ${OBJECTDIR}/database.o database.c

${OBJECTDIR}/income_statement_data.o: income_statement_data.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -s -lm -lpthread `mysql_config --cflags --libs` -MMD -MP -MF $@.d -o ${OBJECTDIR}/income_statement_data.o income_statement_data.c

${OBJECTDIR}/income_statement_history.o: income_statement_history.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -s -lm -lpthread `mysql_config --cflags --libs` -MMD -MP -MF $@.d -o ${OBJECTDIR}/income_statement_history.o income_statement_history.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -s -lm -lpthread `mysql_config --cflags --libs` -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/route.o: route.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -s -lm -lpthread `mysql_config --cflags --libs` -MMD -MP -MF $@.d -o ${OBJECTDIR}/route.o route.c

${OBJECTDIR}/route_data.o: route_data.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -s -lm -lpthread `mysql_config --cflags --libs` -MMD -MP -MF $@.d -o ${OBJECTDIR}/route_data.o route_data.c

${OBJECTDIR}/route_process.o: route_process.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -s -lm -lpthread `mysql_config --cflags --libs` -MMD -MP -MF $@.d -o ${OBJECTDIR}/route_process.o route_process.c

${OBJECTDIR}/sales_route_history.o: sales_route_history.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -s -lm -lpthread `mysql_config --cflags --libs` -MMD -MP -MF $@.d -o ${OBJECTDIR}/sales_route_history.o sales_route_history.c

${OBJECTDIR}/staff.o: staff.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -s -lm -lpthread `mysql_config --cflags --libs` -MMD -MP -MF $@.d -o ${OBJECTDIR}/staff.o staff.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/airsim_v2

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
