# Variaveis

# projeto
PROJ=progr

# .c
C_SOURCE=$(wildcard *.c)

# .h
H_SOURCE=$(wildcard *.h)

# .o
OBJ=$(C_SOURCE:.c=.o)

# compilador
CC=gcc

# flags
CC_FLAGS=-c                     \
         -std=c99               \
         -fstack-protector-all

# Compilacao e linkagem

progr: $(PROJ)

$(PROJ): $(OBJ)
	$(CC) -o $@ -lm $^
	
%.o: %.c %.h
	$(CC) -o $@ -lm $< $(CC_FLAGS)
	
main.o: main.c $(H_SOURCE)
	$(CC) -o $@ -lm $< $(CC_FLAGS)
