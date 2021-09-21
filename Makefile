LEX=flex
BISON=bison
CC=gcc

tradukaten.app: lex.yy.c y.tab.c
	$(CC) lex.yy.c y.tab.c -o tradukaten.app

lex.yy.c: Lexico.l
	$(LEX) -o lex.yy.c Lexico.l

y.tab.c: Sintactico.y
	$(BISON) -dyv Sintactico.y

test: tradukaten.app
	true

clean:
	rm -f *.yy.* *.app *.exe *.tab.* *.output
