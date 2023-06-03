# Multi-Precision Arithmetic Library

This is a header-only library in C++ that provides two classes: `bignum` and `polynomial`, which handle multi-precision arithmetic operations.

## bignum class

The `bignum` class represents a non-negative integer with an unlimited number of digits. It offers the following operations:

- Default constructor initializes the number to zero.
- Implicit construction from a 32-bit unsigned integer (`uint32_t`).
- Explicit construction from a decimal string (`std::string`). Quadratic complexity is allowed.
- Copy constructor and assignment operator.
- Explicit conversion to a 32-bit unsigned integer (`uint32_t`). If the number exceeds the range of `uint32_t`, it is truncated.
- Conditional evaluation (true if the number is non-zero).
- The class does not allow subtraction.
- Function `to_string` to obtain the decimal string representation
- Input/output operations to standard streams (`std::ostream`/`std::istream`).
- Operations: addition (`+`), multiplication (`*`), compound addition (`+=`), and compound multiplication (`*=`). Multiplication has quadratic complexity.

### Additional requirements:
- The base of the number system in `bignum` should be `uint32_t` with a base of 2^32.

## polynomial Class

The `polynomial` class, also located in the `mp` namespace, represents a polynomial of the form:

$P(x) = a_n * x_n + a_{n-1} * x_{n-1} + ... + a_0$

where the coefficients `a` are non-negative 32-bit integers.

The `polynomial` class provides the following operations:

- Explicit constructor from a string in the format: `5*x^3+2*x^1+6*x^0` (no whitespace, always specifies the exponent, and the coefficient may be omitted).
- `at` functions (constant and non-constant) to access the coefficient of the polynomial by index. If the polynomial is constructed with a lower degree, it automatically expands to the required degree (for the non-constant function).
- `operator()` for evaluating the polynomial at a given point. Overloads for `int`, `uint32_t`, `double`, and `bignum` arguments are allowed. The result has the same type as the argument. The implementation should use the Horner's scheme to minimize the number of multiplications by the argument.
