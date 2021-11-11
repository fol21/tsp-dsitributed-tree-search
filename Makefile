# C/C++ compiler
CC = gcc  
CXX = g++
MPICC = mpicc

# Linker
LD = ld

# Libraries to link with (include '-l')
LIBS = -pthread -lpthread -lrt -fopenmp

# Compiler and linker flags (warning levels, optimisation level, 
LINKS = -lm

# include debugging symbols, add include search path, add library search path)
CFLAGS = -Wall -Iinclude
OFLAGS = -g -O -c

# Folders
BIN = ./bin
TEST = ./test
TEST_BIN := ${TEST}/bin
OBJ = ./src/obj

#Objects
_FOBJ = vector.o benchmark.o utils.o tour.o tour-queue.o tour-stack.o
FOBJ := $(_FOBJ:%.o=./src/obj/%.o)

# Params
PARAMS = 16 1 1 1


###### Dependencies #######

queue: ./src/queue.c ./include/queue.h
	$(CC) $(LIBS) ${CFLAGS} $(OFLAGS) $< -o ./src/obj/$@.o

vector: ./src/vector.c ./include/vector.h
	$(CC) $(LIBS) ${CFLAGS} $(OFLAGS) $< -o ./src/obj/$@.o

utils: ./src/utils.c ./include/utils.h
	$(CC) $(LIBS) ${CFLAGS} $(OFLAGS) $< -o ./src/obj/$@.o

benchmark: ./src/benchmark.c ./include/benchmark.h
	$(CC) $(LIBS) ${CFLAGS} $(OFLAGS) $< -o ./src/obj/$@.o

tour: ./src/tour.c ./include/tour.h
	$(CC) $(LIBS) ${CFLAGS} $(OFLAGS) $< -o ./src/obj/$@.o

tour-queue: ./src/tour-queue.c ./include/tour-queue.h
	$(CC) $(LIBS) ${CFLAGS} $(OFLAGS) $< -o ./src/obj/$@.o

tour-stack: ./src/tour-stack.c ./include/tour-stack.h
	$(CC) $(LIBS) ${CFLAGS} $(OFLAGS) $< -o ./src/obj/$@.o


dependencies:
	make queue
	make vector
	make utils
	make benchmark
	make tour
	make tour-queue
	make tour-stack

###### Test ######

%.test : ${TEST}/%.test.c $(FOBJ)
	make dependencies
	$(CC) $(LIBS) $(CFLAGS) -o $(TEST_BIN)/$@ $^ $(LINKS)
	clear
	$(TEST_BIN)/$@

%.mpi.test : ${TEST}/%.mpi.test.c $(FOBJ)
	make dependencies
	$(MPICC) $(LIBS) $(CFLAGS) -o $(TEST_BIN)/$@ $^ $(LINKS)
	clear
	mpiexec -np 4 $(TEST_BIN)/$@


###### Applications #####

% : %.c $(FOBJ)
	make dependencies
	$(CC) $(LIBS) $(CFLAGS) -g -o $(BIN)/$@ $^ $(LINKS)

%.mpi : %.mpi.c $(FOBJ)
	make dependencies
	$(MPICC) $(LIBS) $(CFLAGS) -g -o $(BIN)/$@ $^ $(LINKS)

%.o : %.c $(FOBJ)
	make dependencies
	$(CC) $(LIBS) $(CFLAGS) ${OFLAGS} -o $@ $^ $(LINKS)
