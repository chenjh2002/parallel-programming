export PATH=/home/$USERNAME/openmpi-.10.7/bin:$PATH
export LD_LIBRARY_PATH==/home/$USERNAME/openmpi-.10.7/lib:$LD_LIBRARY_PATH

# mpicc  -g  -Wall  -o  ./task1_gemm/mpi_matmul.o  ./task1_gemm/mpi_matmul.c
mpicc  -g  -Wall  -o  lhw_matmul.o  lhw_matmul.c
