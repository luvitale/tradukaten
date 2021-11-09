# Tradukaten

[![en](https://img.shields.io/badge/lang-en-red.svg)](README.md)
[![eo](https://img.shields.io/badge/lang-eo-green.svg)](README.eo.md)
[![es](https://img.shields.io/badge/lang-es-yellow.svg)](README.es.md)

Tradukaten (Traduka Kateno) estas kompililo farita por la [Programlingvaj kaj Kompililaj](https://polr.luvitale.net/unlam-languages-and-compilers) lernobjekto de [Komputila Inĝenierada](https://polr.luvitale.net/unlam-informatics-engineering) kariero de la [
Nacia Universitato de La Matanza](https://www.unlam.edu.ar).

> [![kompililo](https://upload.wikimedia.org/wikipedia/commons/thumb/5/50/Tradukil-fazoj.png/1280px-Tradukil-fazoj.png)](https://eo.wikipedia.org/wiki/Tradukilo)

## Instali dependecojn

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

## Kompili

### Muntodosiero

```
make
```

### Permane

```
flex -o lex.yy.c Lexico.l
bison -dyv Sintactico.y
gcc lex.yy.c y.tab.c ts.c queue.c rpn.c stack.c -o tradukaten.exe -fcommon
```

Aŭ

```
flex -o lex.yy.c Lexico.l
bison -dyv Sintactico.y
gcc lex.yy.c y.tab.c ts.c queue.c rpn.c stack.c -o tradukaten.app -fcommon
```

### Test

```
make test
```
