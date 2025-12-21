CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lfl

# Executable name
TARGET = calc

# Source files
LEX_SRC = calc.l
YACC_SRC = calc.y
C_SRC = gf.c

# Generated files
YACC_C = y.tab.c
YACC_H = y.tab.h
LEX_C = lex.yy.c

# Object files
OBJ = $(YACC_C:.c=.o) $(LEX_C:.c=.o) $(C_SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(YACC_C) $(YACC_H): $(YACC_SRC)
	bison -d -o $(YACC_C) $(YACC_SRC)

$(LEX_C): $(LEX_SRC) $(YACC_H)
	flex $(LEX_SRC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ) $(YACC_C) $(YACC_H) $(LEX_C)

.PHONY: all clean