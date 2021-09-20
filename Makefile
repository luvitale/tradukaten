LEX=flex
BISON=bison
CC=gcc

analyzer.app: lex.yy.c y.tab.c
	$(CC) lex.yy.c y.tab.c -o analyzer.app

lex.yy.c: lexical.l
	$(LEX) -o lex.yy.c lexical.l

y.tab.c: syntactic.y
	$(BISON) -dyv syntactic.y

clean:
	rm -f *.yy.* *.app *.exe *.tab.* *.output
