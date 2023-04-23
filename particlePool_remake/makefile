#OBJS specifies which files to compile as part of the project
OBJS = main.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_DEBUG_FLAGS = -O0 -pg -ggdb
COMPILER_RELEASE_FLAGS = -w -Ofast

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS =  -lsfml-graphics -lsfml-window -lsfml-system

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = sfml-app

#This is the target that compiles our executable
debug : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_DEBUG_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	./$(OBJ_NAME)
release : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_RELEASE_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	./$(OBJ_NAME)