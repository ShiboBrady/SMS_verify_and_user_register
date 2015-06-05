#This project install directory,it`s used during make install
#it must be setted according to your situation
export INSTALL_PATH?=/opt/apache/fcgi-rs

#This directory to store the temporary object file and
#target produced by make.It must be writeable. 
export SOLUTION_PATH?=/var/tmp/debug

#This is default library path,It will find library in this path
#without specify -L option when link the .o file
export LIBRARY_PATH := ${LIBRARY_PATH}:${SOLUTION_PATH}

#public macro define, It`s valid to all makefile
#if you want trun off the debug info you should Add the "NDEBUG" macro.
#so, this project will be build in release mode. all assert won`t works
#if you want output debug info. you should remove the NDEBUG macro
export PUBLIC_MACRO= -DLINUX -D_DEBUG


#Generate code for a 32-bit or 64-bit environment. 
#The 32-bit environment sets int, long and pointer 
#to 32 bits and generates code that runs on any i386
#system. The 64-bit environment sets int to 32 bits 
#and long and pointer to 64 bits and generates code 
#for AMD's x86-64 architecture.
PLATFORM = -m32
#C compiler command
export CC = gcc
#C++ compiler command
export CXX = g++
#C compiler flags
export CFLAGS=-g -Wall -o2 -fPIC ${PLATFORM}
#C++ compiler flags
export CXXFLAGS=-g -Wall -o2 -fPIC ${PLATFORM}
#public c/c++ link flags.be used in all makefile
export PUBLIC_LDFLAGS= ${PLATFORM}


#utility tools define.
export RM = rm -rf
export CP = cp -rf
export MV = mv -f
export AR = ar cr
export ECHO = echo
export MKDIR = mkdir -p
export INSTALL = /usr/bin/install


export LIB_UTILITY=utility
export LIB_LOG=log4cpp
export LIB_DATABASE=database
export LIB_THREAD=thread
export APP_NAME=rs


	






