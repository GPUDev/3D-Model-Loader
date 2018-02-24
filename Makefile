#define the c++ compiler
CC = clang++
#define the compile-time flags
CFlags = -Wall -g -dynamiclib
#defining header files path
INCLUDE = -I\Headers\
#define library dependent library path
LFlags = 

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = 

#define c++ source files
SRCS = SourceCode\Model.cpp SourceCode\ModelLoader.cpp SourceCode\Utility.cpp SourceCode\Object.cpp

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.cpp=.o)
MAIN = libModelLoader.1.0.dylib
#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean
all:    $(MAIN)
	@echo  Simple compiler named ModelLoader has been compiled

$(MAIN): $(OBJS) 
	 $(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)



