# Tradukaten

[![en](https://img.shields.io/badge/lang-en-red.svg)](README.md)
[![eo](https://img.shields.io/badge/lang-eo-green.svg)](README.eo.md)
[![es](https://img.shields.io/badge/lang-es-yellow.svg)](README.es.md)

Tradukaten (del esperanto Traduka Kateno, cadena de traducciones) es un compilador hecho para la materia de [Lenguajes y Compiladores](https://polr.luvitale.net/unlam-languages-and-compilers) de la carrera de [Ingeniería en Informática](https://polr.luvitale.net/unlam-informatics-engineering) de la [Universidad Nacional de La Matanza](https://www.unlam.edu.ar).

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

```
make
```
