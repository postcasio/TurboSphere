#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.138                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS =  -Wall -fPIC
RESINC = 
LIBDIR = 
LIB = ./libt5.so ./graphiccommon.so
LDFLAGS = -shared

INC_DEBUG =  $(INC)
CFLAGS_DEBUG =  $(CFLAGS) -g
RESINC_DEBUG =  $(RESINC)
RCFLAGS_DEBUG =  $(RCFLAGS)
LIBDIR_DEBUG =  $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG =  $(LDFLAGS) -fPIC -shared
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/configmanager.so

INC_RELEASE =  $(INC)
CFLAGS_RELEASE =  $(CFLAGS) -O2 -fpic -Os -pedantic-errors -Wall -fexceptions
RESINC_RELEASE =  $(RESINC)
RCFLAGS_RELEASE =  $(RCFLAGS)
LIBDIR_RELEASE =  $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE =  $(LDFLAGS) -s -shared
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = ../bin/Release/configmanager.so

OBJ_DEBUG = $(OBJDIR_DEBUG)/opengame.o $(OBJDIR_DEBUG)/openscript.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/opengame.o $(OBJDIR_RELEASE)/openscript.o

all: release

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) -shared $(LIBDIR_DEBUG) $(OBJ_DEBUG)  -o $(OUT_DEBUG) $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/openscript.o: openscript.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c openscript.cpp -o $(OBJDIR_DEBUG)/openscript.o

$(OBJDIR_DEBUG)/opengame.o: opengame.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c opengame.cpp -o $(OBJDIR_DEBUG)/opengame.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)

before_release: 
	test -d ../../bin/Release || mkdir -p ../../bin/Release
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)
	cp ../libt5.so ./libt5.so
	cp ../bin/Release/graphiccommon.so ./graphiccommon.so

after_release: 
	rm -rf ./libt5.so
	rm -rf ./graphiccommon.so
	cp ../bin/Release/configmanager.so ../configmanager.so
	
release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/openscript.o: openscript.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c openscript.cpp -o $(OBJDIR_RELEASE)/openscript.o

$(OBJDIR_RELEASE)/opengame.o: opengame.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c opengame.cpp -o $(OBJDIR_RELEASE)/opengame.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

