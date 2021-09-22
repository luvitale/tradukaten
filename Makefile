CC=gcc

ifeq ($(OS),Windows_NT)
	LEX=win_flex
	BISON=win_bison
	OBJECT_NAME=tradukaten.exe
else
	LEX=flex
	BISON=bison
	OBJECT_NAME=tradukaten.app
endif

tradukaten.app: lex.yy.c y.tab.c
	$(CC) lex.yy.c y.tab.c -o $(OBJECT_NAME) -fcommon

lex.yy.c: Lexico.l
	$(LEX) -o lex.yy.c Lexico.l

y.tab.c: Sintactico.y
	$(BISON) -dyv Sintactico.y

test: tradukaten.app
	./$(OBJECT_NAME) ./tests/prueba.txt

clean:
	rm -f *.yy.* *.app *.exe *.tab.* *.output
