PROG = semanticka_analiza
CC = g++
CFLAGS = -Wall -std=c++11

$(PROG): lex.yy.o parser.o ast.o type.o
	$(CC) -o $@ $^
lex.yy.o: lex.yy.c parser.tab.hpp ast.hpp type.hpp
	$(CC) $(CFLAGS) -Wno-sign-compare -c -o $@ $<
lex.yy.c: lexer.l
	flex $<
parser.o: parser.tab.cpp parser.tab.hpp ast.hpp type.hpp
	$(CC) $(CFLAGS) -c -o $@ $<
parser.tab.cpp parser.tab.hpp: parser.ypp
	bison -d -v $<
ast.o: ast.cpp ast.hpp type.hpp
	$(CC) $(CFLAGS) -c -o $@ $<
type.o: type.cpp type.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -rf *tab* lex.yy.c *.o $(PROG) *output