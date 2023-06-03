# C++ course

This repository contains the code for the tasks completed during the course. The tasks involve the implementation of various C++ libraries and utilities.

## Libraries Implemented
### [variant](./variant)
A header-only library providing a type-safe implementation of a union, allowing the storage of a single value from a predefined set of types.
For more details, please refer to the [detailed readme](./variant/README.md) inside the library directory.

### [whatever](./whatever)
A header-only library offering a generic container capable of storing objects of any type. For more details, please refer to the [detailed readme](./whatever/README.md) inside the library directory.

### [bignum](./bignum)
A header-only library enabling the manipulation of arbitrarily large integers that exceed the capacity of built-in types. For more details, please refer to the [detailed readme](./bignum/README.md) inside the library directory.


### [matrices](./matrices)
A header-only library providing functionality for working with matrices, including operations like matrix multiplication and inversion. For more details, please refer to the [detailed readme](./matrices/README.md) inside the library directory.

## Other tasks
- [worker.hpp](./worker.hpp): A utility for executing functions with various features, including deferred execution, result storage, type retrieval, and exception handling.
- [typeset.hpp](./typeset.hpp) allows for the creation of sets of types . It provides the following operations:
    - `belongs_to<T, Typeset>`: Checks if type `T` belongs to the typeset.
    - `join<Ts1, Ts2>`: Merges two typesets into a single typeset.
    - `cross<Ts1, Ts2>`: Computes the cross product of two typesets.
    - `subtract<Ts1, Ts2>`: Computes the difference between two typesets.
    - `variant_of<Typeset>`: Generates a `std::variant` type based on the types in the typeset.

- [pretty_print.hpp](./pretty_print.hpp): A pretty printer utility using SFINAE
- [pretty_print_concepts.hpp](./pretty_print_concepts.hpp): A pretty printer utility using C++20 concepts.
- [vector.hpp](./vector.hpp): A vector implementation with methods for resizing, adding elements, and proper exception safety.
- [buffer.hpp](./buffer.hpp): associate an execution queue with a buffer, allowing the addition and execution of tasks:
    - Each Buffer object can be associated with an execution queue using the Buffer::move_to_execution_queue method.
    - The Buffer class provides a post method to add tasks (represented as std::function<void()>) to the buffer.
    - When there are no executing tasks in the associated ExecutionQueue, the buffer automatically sends a task from its queue to the ExecutionQueue for execution.
    - The ExecutionQueue class creates a dedicated thread to execute the tasks synchronously.
- [parallel_for.hpp](./parallel_for.hpp): A thread-safe implementation of a "for" loop, enabling parallel execution.
- [pythagorean.hpp](./pythagorean.hpp): A utility that generates an infinity range of Pythagorean triples (x, y, z) where x * x + y * y == z * z.
