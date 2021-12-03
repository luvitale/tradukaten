CC=gcc

# check if flex exists
LEX := $(shell which flex)
ifndef LEX
	# if not, try to find it in the path
	LEX := $(shell which win_flex)
	ifndef LEX
		# error
		echo "flex not found"
		exit 1
	endif
endif

BISON := $(shell which bison)
ifndef BISON
	# if not, try to find it in the path
	BISON := $(shell which win_bison)
	ifndef BISON
		# error
		echo "bison not found"
		exit 1
	endif
endif

ifeq ($(OS),Windows_NT)
	EXT=exe
else
	EXT=app
endif

all: tradukaten.app

%.app: lex.yy.c y.tab.c ts.c rpn.c utils/*.c
	$(CC) -o $*.$(EXT) $? -fcommon

lex.yy.c: Lexico.l
	$(LEX) -o $@ $<

y.tab.c: Sintactico.y
	$(BISON) -o $@ -dyv $<

test:
	./tradukaten.$(EXT) ./tests/prueba.txt

clean:
	rm -f *.yy.* *.app *.exe *.tab.* *.output
