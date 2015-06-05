#------------------------------------------------------------------
# if you want compile this module independent, please include option.mak
# if you want compile this module with other module, then before make this 
# module you should include option.mak too.
#------------------------------------------------------------------
include option.mak

#------------------------------------------------------------------
# This part must be Configured according to actual situations
#------------------------------------------------------------------

#module name
MODULE_NAME=rs
#The api home path. it`s only valid for this makefile
API_HOME=../../api
CODE_HOME=../../

#module output directory
TARGET_DIR = ${SOLUTION_PATH}
#module output name
TARGET_NAME = ${MODULE_NAME}
#reference include file path, each path start with -I, separate by space
INCLUDE_DIR = -I../include \
			  -I${API_HOME}/thread/include \
			  -I${API_HOME}/fcgi/include \
			  -I${API_HOME}/log/include \
			  -I${API_HOME}/utility/include \
			  -I${API_HOME}/json/include \
			  -I${CODE_HOME}/msas/sammodel/include \
			  -I${CODE_HOME}/oss_api/c/msip_api/include \
			  -I${CODE_HOME}/oss_api/c/baseobj_api/include \
			  -I${CODE_HOME}/oss_api/c/database_api/include \
			  -I${CODE_HOME}/oss_api/c/sys_res \
			  -I${API_HOME}/mysql/include
#source file path, separate by space
SOURCE_DIR= ../source

#sam module lib
LIB_SAM=-lsammodel -ldatabase_gcc2_0 -lmsip_api_gcc2_0 -llog_api_gcc2_0 -lobject_gcc2_0 -luncptrap2_0 -lutility_gcc2_0 

#reference library file path, each path start with -L, separate by space
LIBRARY_DIR =-L${API_HOME}/fcgi/lib \
			-L${API_HOME}/json/lib \
			-L${MSAS_SAMMODEL_SO_OUT_DIR} \
			-L${API_HOME}/mysql/lib
			
#reference library file list, each library start with -l
LIBRARY_REF := -lpthread -l${LIB_THREAD} -l${LIB_LOG} -l${LIB_UTILITY} -lfcgi -ljson ${LIB_SAM} -lmysqlclient
# -Wl,-Bstatic

#module version
VERSION=1.0
#define macro
MACROS = ${PUBLIC_MACRO} -DVERSION=${VERSION} -D_MODULE_NAME=${MODULE_NAME}


#C/C++ preprocessor flags
CPPFLAGS = ${MACROS} ${INCLUDE_DIR}
LDFLAGS = ${PUBLIC_LDFLAGS} ${LIBRARY_REF} ${LIBRARY_DIR}

#------------------------------------------------------------------
# export module name, other module use ${LIB_UTILITY} to get the 
# name of this module
#------------------------------------------------------------------
export APP_NAME=${MODULE_NAME}

#------------------------------------------------------------------
# define relationship of project file, you didn`t need change anything
# it`s fit in all situations
#------------------------------------------------------------------
vpath %.c ${SOURCE_DIR}
vpath %.cpp ${SOURCE_DIR}

OBJS = registerserver.o \
	transactionprocess.o \
	transaction.o \
	packet.o \
	configuration.o \
	confaccess.o \
	stringutil.o \
	sms.o \
	identifyservice.o \
	db.o \
	xmpp_register.o
	


TARGET=${TARGET_DIR}/${TARGET_NAME}
OBJ_DIR=${TARGET_DIR}/${MODULE_NAME}_objs
ABS_OBJS=$(addprefix ${OBJ_DIR}/,${OBJS})

.PHONY : all clean env cleanall install
all:submake ${TARGET}

submake:
	${MAKE} -C ../../api/utility/makefile
	${MAKE} -C ../../api/thread/makefile
#	${MAKE} -C ../../api/database/makefile
	${MAKE} -C ../../api/log/makefile
env:
#	${MAKE} database.mak -C ../database/makefile env
	@${ECHO} SOLUTION_PATH=${SOLUTION_PATH}
	@${ECHO} TARGET=${TARGET}
	@${ECHO} OBJ_DIR=${OBJ_DIR}
	@${ECHO} DEP=${DEP}
	@${ECHO} SOURCE_DIR=${SOURCE_DIR}
	@${ECHO} CFLAGS=${CFLAGS}
	@${ECHO} CXXFLAGS=${CXXFLAGS}
	@${ECHO} LDFLAGS=${LDFLAGS}
	@${ECHO} LIB_UTILITY=${LIB_UTILITY}
	@${ECHO} ABS_OBJS=${ABS_OBJS}
	@${ECHO} API_HOME=${API_HOME}
	@${ECHO} LIBRARY_PATH=${LIBRARY_PATH}

${OBJ_DIR}/%.d:%.c
	@test ! -d ${OBJ_DIR}&& ${MKDIR} ${OBJ_DIR};\
	${RM} $@;\
	${CC} -MM ${CPPFLAGS} $< | sed 's,\($*\)\.o[ :]*,${OBJ_DIR}/\1\.o $@:,g' > $@ 
	
${OBJ_DIR}/%.d:%.cpp
	@test ! -d ${OBJ_DIR} && ${MKDIR} ${OBJ_DIR};\
	${RM} $@;\
	${CXX} -MM ${CPPFLAGS} $< | sed 's,\($*\)\.o[ :]*,${OBJ_DIR}/\1\.o $@:,g' > $@

${OBJ_DIR}/%.o:%.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c $< -o $@

${OBJ_DIR}/%.o:%.cpp
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c $< -o $@


DEP := ${ABS_OBJS:.o=.d}
sinclude ${DEP}

#make dynamic library
#${TARGET}:${ABS_OBJS}
#	${CXX} -shared ${LDFLAGS} $^ -o $@

#make application
${TARGET}:${ABS_OBJS}
	${CXX} ${LDFLAGS} $^ -o $@

#make static library	
#${TARGET}:${ABS_OBJS}
#	${AR} $@ $^

cleanall:
	${MAKE} -C ../../api/utility/makefile clean
	${MAKE} -C ../../api/thread/makefile clean
#	${MAKE} -C ../../api/database/makefile clean
	${MAKE} -C ../../api/log/makefile clean
	-${RM} ${OBJ_DIR}/*.o ${OBJ_DIR}/*.d ${TARGET}

clean:
	-${RM} ${OBJ_DIR}/*.o ${OBJ_DIR}/*.d ${TARGET}
	

install:
	test ! -d ${INSTALL} && ${MKDIR} ${INSTALL_PATH}
	${MKDIR} ${INSTALL_PATH}/lib
	${MKDIR} ${INSTALL_PATH}/fcgilib
	${MKDIR} ${INSTALL_PATH}/mysqllib
	${MKDIR} ${INSTALL_PATH}/bin
	${MKDIR} ${INSTALL_PATH}/cfg
	${MKDIR} ${INSTALL_PATH}/log
	${INSTALL} -m 755 -t ${INSTALL_PATH}/bin ${SOLUTION_PATH}/${APP_NAME}
	${INSTALL} -m 755 -t ${INSTALL_PATH}/lib ${SOLUTION_PATH}/*.so
	${INSTALL} -m 755 -t ${INSTALL_PATH}/lib ${SOLUTION_PATH}/*.a
	${INSTALL} -m 755 -t ${INSTALL_PATH}/lib ${API_HOME}/json/lib/*
	${INSTALL} -m 755 -t ${INSTALL_PATH}/fcgilib ${API_HOME}/fcgi/lib/*
	${INSTALL} -m 755 -t ${INSTALL_PATH}/mysqllib ${API_HOME}/mysql/lib/*
	${INSTALL} -m 755 -t ${INSTALL_PATH}/cfg ../cfg/sms.json
	${INSTALL} -m 755 -t ${INSTALL_PATH}/cfg ../cfg/main.cfg
	${MV} ${INSTALL_PATH}/cfg/main.cfg ${INSTALL_PATH}/cfg/${MODULE_NAME}.cfg
