bison -d calc.y 
# Generuje y.tab.c i y.tab.h

flex calc.l
# Generuje lex.yy.c

gcc -o translator y.tab.c lex.yy.c gf.c -lfl 
# -lfl linkuje bibliotekę Flex

./translator
# Następnie wpisuj wyrażenia (każde zatwierdź Enterem).