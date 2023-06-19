nvcc -c mat_vec_multiply.cu
nvcc -c file_io.cu
nvcc -c main.cu
nvcc mat_vec_multiply.o file_io.o main.o -o mat_vec_multiply 