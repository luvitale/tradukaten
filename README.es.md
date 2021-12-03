# Tradukaten

[![en](https://img.shields.io/badge/lang-en-red.svg)](README.md)
[![eo](https://img.shields.io/badge/lang-eo-green.svg)](README.eo.md)
[![es](https://img.shields.io/badge/lang-es-yellow.svg)](README.es.md)

Tradukaten (del esperanto Traduka Kateno, cadena de traducciones) es un compilador hecho para la materia de [Lenguajes y Compiladores](https://polr.luvitale.net/unlam-languages-and-compilers) de la carrera de [Ingeniería en Informática](https://polr.luvitale.net/unlam-informatics-engineering) de la [Universidad Nacional de La Matanza](https://www.unlam.edu.ar).

> [![compilador](https://upload.wikimedia.org/wikipedia/commons/thumb/e/ef/CompilationScheme-Spanish.png/300px-CompilationScheme-Spanish.png)](https://es.wikipedia.org/wiki/Compilador)

## Instalar dependencias

* [GCC](https://gcc.gnu.org/)
* [Flex](https://github.com/westes/flex)
* [Bison](https://www.gnu.org/software/bison/)

### APT

```
sudo apt-get update
sudo apt-get -y install flex bison
```

### Chocolatey

```
choco upgrade chocolatey
choco install --verbose mingw make winflexbison3
```

## Compilar

### Makefile

```
make
```

### Manualmente

```
flex -o lex.yy.c Lexico.l
bison -dyv Sintactico.y
gcc lex.yy.c y.tab.c ts.c rpn.c utils/stack.c utils/queue.c utils/asm_stack.c utils/int_list.c -o tradukaten.exe -fcommon
```

O

```
flex -o lex.yy.c Lexico.l
bison -dyv Sintactico.y
gcc lex.yy.c y.tab.c ts.c rpn.c utils/stack.c utils/queue.c utils/asm_stack.c utils/int_list.c -o tradukaten.app -fcommon
```

### Test

```
make test
```

### Ensamblar

```
tasm Final.asm
tlink /3 /x /v Final.obj numbers.obj
pause
Final.exe
```
