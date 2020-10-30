SRC=main.c util.c
OBJ=$(SRC:.c=.o)
TARGET=app

all: $(OBJ)
	gcc $(OBJ) -o $(TARGET) -L $(SGX_SDK)/lib64 -lsgx_tcrypto

main.o: main.c
	gcc -c main.c -o main.o

util.o: util.c
	gcc -c util.c -o util.o -mrdrnd

clean:
	rm -rf app $(OBJ)
