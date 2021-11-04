CC=gcc

ifeq ($(OS),Windows_NT)
	LEX=win_flex
	BISON=win_bison
	EXT=exe
else
	LEX=flex
	BISON=bison
	EXT=app
endif

all: tradukaten.app

%.app: lex.yy.c y.tab.c ts.c queue.c rpn.c stack.c
	$(CC) -o $*.$(EXT) $? -fcommon

lex.yy.c: Lexico.l
	$(LEX) -o $@ $<

y.tab.c: Sintactico.y
	$(BISON) -o $@ -dyv $<

test: tradukaten.app
	./tradukaten.$(EXT) ./tests/prueba.txt

clean:
	rm -f *.yy.* *.app *.exe *.tab.* *.output
