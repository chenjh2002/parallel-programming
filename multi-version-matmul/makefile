CC=gcc
CPP=g++
CFLAGS=-g -Wall
C_OUTPUT=matmul_c.o
C_OBJFILES=matmul.c

CPP_OUTPUT=matmul_cpp.o
CPP_OBJFILES=matmul.cpp

ITER_OUTPUT=matmul_iter.o
ITER_OBJFILES=matmul_iter.c

OMP_OUTPUT=matmul_omp.o 
OMP_OBJFILES=matmul_omp.c 

SIMD_OUTPUT=simd.o
SIMD_OBJFILES=simd.c 

all:$(C_OUTPUT) $(CPP_OUTPUT) $(ITER_OUTPUT) $(OMP_OUTPUT) $(SIMD_OUTPUT) test

$(C_OUTPUT): $(C_OBJFILES)
	$(CC) $(CFLAGS) $(C_OBJFILES) -o ./exe_file/$(C_OUTPUT)

$(CPP_OUTPUT): $(CPP_OBJFILES)
	$(CPP) $(CPP_OBJFILES) -o ./exe_file/$(CPP_OUTPUT)

$(ITER_OUTPUT): $(ITER_OBJFILES)
	$(CC) $(CFLAGS) $(ITER_OBJFILES) -o ./exe_file/$(ITER_OUTPUT)

$(OMP_OUTPUT): $(OMP_OBJFILES)
	$(CC) -g -O4 -fopenmp -fopt-info-optall-optimized -ftree-vectorize -mavx $(CFLAGS) -o ./exe_file/$(OMP_OUTPUT) $(OMP_OBJFILES)

$(SIMD_OUTPUT): $(SIMD_OBJFILES)
	$(CC) -O4 -fopt-info-optall-optimized -mavx -o ./exe_file/$(SIMD_OUTPUT) $(SIMD_OBJFILES) 

test:matmul_c.o
	@echo ====C version====
	sh ./test.sh
	@echo ====C++ version====
	sh ./test_cpp.sh
	@echo ====Iteraion version====
	sh ./test_iter.sh
	@echo ====OMP version====
	sh ./test_omp.sh
	@echo ====SIMD version====
	sh ./test_simd.sh