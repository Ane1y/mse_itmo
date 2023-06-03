# Discriminated union type: variant

The `variant` library, provided in the `variant.hpp` header file, offers a type-safe container for storing a single value of one of the predefined types. It implements a discriminated union type, also known as tagged union, which provides a simple solution for manipulating heterogeneous data sets in a unified manner. In C++17, this functionality is available as `std::variant`, and in Boost as `boost::variant`.

## Usage

To use the `variant` library, include the `variant.hpp` header file in your project.

```cpp
#include "variant.hpp"
```

### Construction

Objects of the `variant` class can be created by explicitly specifying one of the predefined types or by passing an object of a compatible type to its constructor or assignment operator.

```cpp
util::variant<double, char, std::string> v; // Default constructor, creates an empty variant

v = 3.14; // Assign a double value to the variant

double d = util::get<double>(v); // Extract the stored double value
```

### Value Retrieval

The stored value can be retrieved from a `variant` object using the `util::get` function, either by specifying the desired type or by providing the index of the type in the variant's template parameter list. The `util::get` function returns a reference or pointer to the stored value if the requested type or index matches the stored type. If the types or index do not match or if the `variant` object is empty, an exception of type `util::bad_get` is thrown or `nullptr` is returned.

```cpp
util::variant<double, char, std::string> v;

v = 3.14;
double d = util::get<double>(v); // Get the stored double value

v = "Variant";
std::string s = util::get<std::string>(v); // Get the stored std::string value

v = 'A';
char c = util::get<1>(v); // Get the stored char value using the index
```

### Other Operations

The `variant` class supports the following operations:

- `swap`: Swaps the contents of two `variant` objects.
- `empty`: Checks if a `variant` object is empty.
- `clear`: Clears the contents of a `variant` object.
- `index`: Retrieves the index of the currently stored type or `util::variant_npos` if the `variant` is empty.

### Visitor Pattern

The `util::apply_visitor` function allows to implement the visitor pattern for the `variant` container. It applies the visitor's `operator()` function to the stored value in the `variant`. For an empty `variant`, the `operator()` of the visitor is called without any parameters.

```cpp
struct output
{
    void operator()(double d) const { std::cout << d << '\n'; }
    void operator()(char c) const { std::cout << c << '\n'; }
    void operator()(const std::string& s) const { std::cout << s << '\n'; }
};

int main()
{
    util::variant<double, char, std::string> v;

    v = 3.14;
    util::apply_visitor(output{}, v); // Calls the appropriate operator() of the visitor

    v = 'A';
    util::apply_visitor(output{}, v);

    v = "Variant";
    util::apply_visitor(output{}, v);
}
```

## Implementation Details

The `variant` library utilizes a strict exception safety guarantee, aligns its data based on the largest alignment requirement among the stored types, and stores its data directly within the `variant` object (without dynamic memory allocation).

Please note that this implementation may not cover all possible corner cases and may need further testing and refinement for production use.
