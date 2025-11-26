# Definicja nazwy finalnego programu
TARGET = translator

# Definicja kompilatora i flag
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lfl # Flaga potrzebna do linkowania biblioteki Flex

# Definicja plików źródłowych i obiektowych
BISON_SRC = calc.y
FLEX_SRC = calc.l
C_SRC = gf.c
OBJ = y.tab.o lex.yy.o gf.o

# ------------------------------------------------------------------------------
# Główny cel: Buduje translator
# ------------------------------------------------------------------------------
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# ------------------------------------------------------------------------------
# Generowanie plików obiektowych
# ------------------------------------------------------------------------------

# Zależność od pliku y.tab.c (generowany przez Bisona)
y.tab.o: y.tab.c gf.h
	$(CC) $(CFLAGS) -c y.tab.c -o $@

# Zależność od pliku lex.yy.c (generowany przez Flexa)
lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c -o $@

# Zależność od pliku gf.c
gf.o: gf.c gf.h
	$(CC) $(CFLAGS) -c gf.c -o $@

# ------------------------------------------------------------------------------
# Generowanie kodu źródłowego przez Flex/Bison
# ------------------------------------------------------------------------------

# 1. Bison generuje y.tab.c i y.tab.h (nagłówek z tokenami)
y.tab.c y.tab.h: $(BISON_SRC)
	bison -d -y $(BISON_SRC)

# 2. Flex generuje lex.yy.c
lex.yy.c: $(FLEX_SRC)
	flex $(FLEX_SRC)

# ------------------------------------------------------------------------------
# Cele pomocnicze
# ------------------------------------------------------------------------------

.PHONY: clean
clean:
	@echo "Czyszczenie plików..."
	@rm -f $(TARGET) $(OBJ) y.tab.c y.tab.h lex.yy.c
	@echo "Zakończono."

# Ustawienie celu phony, aby zapobiec konfliktom z plikami o tej samej nazwie
.PHONY: all