# Tradukaten

[![en](https://img.shields.io/badge/lang-en-red.svg)](README.md)
[![eo](https://img.shields.io/badge/lang-eo-green.svg)](README.eo.md)
[![es](https://img.shields.io/badge/lang-es-yellow.svg)](README.es.md)

Tradukaten (from Esperanto: Traduka Kateno, translation chain) is a compiler done for [Languages and Compilers](https://polr.luvitale.net/unlam-languages-and-compilers) subject of [Informatics Engineering](https://polr.luvitale.net/unlam-informatics-engineering) career from [Universidad Nacional de La Matanza](https://www.unlam.edu.ar).

## Install dependencies

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
choco install --verbose mingw make winflexbison3
```

## Compile

### Makefile

```
make
```

### Manually

```
flex -o lex.yy.c Lexico.l
bison -dyv Sintactico.y
gcc lex.yy.c y.tab.c -o tradukaten.exe -fcommon
```

Or

```
flex -o lex.yy.c Lexico.l
bison -dyv Sintactico.y
gcc lex.yy.c y.tab.c -o tradukaten.app -fcommon
```
