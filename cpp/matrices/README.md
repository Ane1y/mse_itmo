# Matrix Operations Utility
This is a command-line utility written in C++ that performs addition and multiplication operations on matrices. It accepts a list of matrix files as command-line arguments, along with the corresponding operations to be performed on the matrices.

## Usage

To run the program, use the following syntax in the Unix/Linux terminal:
```
$ ./matrices mat1.txt --add mat2.txt --mult mat3.txt --mult mat4.txt
```

For Windows command prompt:
```
C:\> matrices.exe mat1.txt --add mat2.txt --mult mat3.txt --mult mat4.txt
```

The utility supports two operations:
1. `--add` (preceded by two minus signs) performs matrix addition.
2. `--mult` performs matrix multiplication.

The operations are executed in the order they are specified. For example, in the given example above, the utility will first add the matrix from `mat1.txt` to the matrix from `mat2.txt`, then multiply the result by the matrix from `mat3.txt`, and finally multiply the result by the matrix from `mat4.txt`:
```
((mat1 + mat2) * mat3) * mat4
```

Each matrix is represented in a separate text file. The file format consists of the following:
- The first line of the file contains two natural numbers, N and M, representing the number of rows and columns in the matrix.
- The subsequent N lines contain M floating-point numbers representing the elements of the matrix.

The resulting matrix is displayed on the screen (terminal) in the same format as the input matrices. No additional or debug information is displayed.

## Error Handling

The utility handles the following error scenarios:
- If the command-line arguments do not match the specified format.
- If it is not possible to perform matrix operations (e.g., addition of matrices with different dimensions).
- If a file cannot be opened or read.

In case of an error, the program will display a meaningful error message and terminate with a non-zero exit code.

## Additional Requirements to task

This task does not require the use or knowledge of C++ classes, structures, or exceptions. It is sufficient to know how to work with two-dimensional arrays (allocate, deallocate, and iterate over them), input/output streams, strings, and functions.

Memory allocation and deallocation for matrix storage should be done manually using `new[]` and `delete[]`. Code duplication for memory allocation and deallocation should be avoided by extracting them into separate functions.

The use of STL classes for matrix storage or memory management (e.g., `std::vector` and smart pointers) is prohibited. However, STL classes can be used for other entities besides matrices (e.g., `std::string`). Writing a custom matrix class is allowed but not required.
