gcc -c leak_detector_c.c -o leak_detctor_c.o
gcc -c allocate_mat.c -o allocate.o
gcc -o matrix leak_detctor_c.o allocate.o
./matrix
cat ./leak_info.txt

# gcc -fsanitize=leak -o allocate.o allocate_mat.c 