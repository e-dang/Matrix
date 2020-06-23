# Matrix

## Description
C++ header only matrix library that I wrote as practice for writing container classes that work with STL algorithms. This matrix implementation only functions as a container for data, with no internal mathematical operations such as dot products. I use this matrix implementation in my [K-Medoids](https://github.com/e-dang/K-Medoids) and [K-Means](https://github.com/e-dang/K-Means) projects.

## Usage
All that is required to use the matrix library is add an include statement to the __matrix.hpp__ header file.

## Tests
While the matrix library doesn't have any dependencies and doesn't need to be compiled, in order to run the test suite you must compile it and have the following dependencies:

#### Dependencies
- Boost 1.72.0

#### Compiling
Compiling the test suite has been tested on MacOS Mojave 10.14.6 with the following compiler:
- Clang 9.0.1

In the top level directory of the project, run the following commands:
```
mkdir build
cd build
cmake ..
make
```
