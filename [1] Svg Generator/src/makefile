# Variaveis

# projeto
BIN_1=progrdin
BIN_2=progrest

# .c
C_SOURCE=$(wildcard *.c)

# .h
H_SOURCE=$(wildcard *.h)

# .o
OBJ=$(C_SOURCE:.c=.o)

# compilador
CC=gcc

# flags
CC_FLAGS=-c                    \
         -Wall                 \
         -std=c99              \
         -fstack-protector-all
 
         
# Compilacao e linkagem

all: $(BIN_1) $(BIN_2) clean

$(BIN_1): $(OBJ)
	$(CC) -o $@ $^

$(BIN_2): $(OBJ)
	$(CC) -o $@ $^
	
%.o: %.c %.h
	$(CC) -o $@ $< $(CC_FLAGS)
	
main.o: main.c $(H_SOURCE)
	$(CC) -o $@ $< $(CC_FLAGS)

clean:
	rm -rf *.o *~
	