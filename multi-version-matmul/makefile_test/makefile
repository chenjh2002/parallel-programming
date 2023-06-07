# hello:
# 	echo "Hello World"
CC=gcc
CFLAGS=-g -Wall
OUTPUT=binary
OBJFILES=lib.o pro.o

all:$(OUTPUT) test

# lib.o:lib.c 
# 	$(CC) $(CFLAGS) -c lib.c -o lib.o 

# pro.o:pro.c 
# 	$(CC) $(CFLAGS) -c pro.c -o pro.o

$(OUTPUT):$(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o binary



# $< denpandency
# $@ target(%.o)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@


test:$(OUTPUT)
	sh ./testscript.sh

clean:
	rm *.o $(OUTPUT)