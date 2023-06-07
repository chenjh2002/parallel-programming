#convert library code to object file
g++ -c -o library.o library.c

#create shared .SO library
gcc -shared -o libfoo.so library.object


#create the executable by linking shared library
gcc -L<path to .SO library> -Wall -o code main.c -l <library name>

#make shared library available at runtime
export LD_LIBRARY_PATH=<path to .SO file>:$LD_LIBRARY_PATH

#run executable
./a.out
