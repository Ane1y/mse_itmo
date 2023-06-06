# Whatever

The `whatever` class is a non-template class that allows storing and retrieving values of arbitrary types in a type-safe manner. It provides a flexible and convenient way to work with heterogeneous objects within a single common type.

## Usage

To use the `whatever` class, include the `whatever.hpp` header file in the project.

```cpp
#include "whatever.hpp"
```

### Construction

Objects of the `whatever` class can be created by passing an object of any type to its constructor.

```cpp
int a = 5;
whatever wa(a);

std::string b = "6";
whatever wb(b);
```

### Value Retrieval

The stored value can be retrieved from a `whatever` object using the `whatever_cast` function. The `whatever_cast` function returns a pointer or reference to the stored value if the requested type matches the stored type. If the types do not match or if the `whatever` object is empty, a null pointer is returned or an exception of type `bad_whatever_cast` is thrown.

```cpp
whatever w(10);

int* ptr = whatever_cast<int>(&w); // Get a pointer to the stored int value
if (ptr != nullptr) {
    // Handle the int value
}

std::string& ref = whatever_cast<std::string&>(w); // Get a reference to the stored std::string value
try {
    // Handle the std::string value
} catch (const bad_whatever_cast& ex) {
    // Handle the exception
}
```

### Other Operations

The `whatever` class supports the following operations:

- `swap`: Swaps the contents of two `whatever` objects.
- `empty`: Checks if a `whatever` object is empty.
- `clear`: Clears the contents of a `whatever` object.

## Implementation Details

The `whatever` class internally stores the value using type erasure technique. It uses an interface called `HolderBase` and a template-based implementation called `Holder<T>` to store and retrieve the values. The `Holder<T>` class is derived from the `HolderBase` class and contains the actual value of type `T`. The implementation utilizes RTTI (dynamic_cast and type_info) to perform type checking and conversion.
