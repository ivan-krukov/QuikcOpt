# QuikcOpt

A minimal header-only `C++11` wrapper around `getopt`, with (some) support for Eigen types.

This parser is a wrapper around `getopt`

## Features

* Tabular argument specification

``` cpp
//         NAME                HAS_ARG (getopt)   REQ    DEFAULT    TYPE        HELP
parser.add('c', "count",       required_argument, true,  "",        "int",      "Number of multiplications");
parser.add('x', "vector",      required_argument, true,  "",        "int[K]",   "Vector data");
parser.add('A', "matrix",      required_argument, true,  "",        "int[KxK]", "Matrix data");
parser.add('e', "echo",        no_argument,       false, "false",   "bool",     "Echo input arguments");
parser.add('f', "file",        required_argument, false, "stdout",  "string",   "Output file");
parser.add('H', "help",        no_argument,       false, "false",   "bool",     "Print this message and exit");
```

* Help generation

```
USAGE: test [options]

NAME                TYPE                DEFAULT                 DESCRIPTION
-c --count          int                                         Number of multiplications
-x --vector         int[K]                                      Vector data
-A --matrix         int[KxK]                                    Matrix data
[ -e --echo]        bool                false                   Echo input arguments
[ -f --file]        string              stdout                  Output file
[ -H --help]        bool                false                   Print this message and exit
```

* Eigen types

Vectors are specified as CSV, matrices as semicolon separated (row major) CSV.

```bash
./test --matrix='10,20;0,10' --vector=0.8,0.1,4
```

Note that matrix arguments need to be quoted - otherwise the shell will interpret them as end of command.

## Building

Just include `include/QuickOpts.hpp` in your file.

## Requirements

Eigen

## Example

See [src/example.cpp](src/example.cpp)

## TODO

Proper templating of `as_*` function in `QuickOpt::Argument`


```
```
