# Python

During the Python course, the following scripts were implemented:

### [Reversed dictionary](./reversed_dict.py)
The given dictionary is in the form of a file containing English words and their translations into Latin. The translations can have multiple options and are formatted as follows:

```
    apple - malum, pomum, popula
    fruit - baca, bacca, popum
    punishment - malum, multa
```

For the given example, the output will be:
```
    baca - fruit
    bacca - fruit
    malum - apple, punishment
    multa - punishment
    pomum - apple
    popula - apple
    popum - fruit
```

If different files contain different translations for the same words, the translations should be combined, leaving only unique words.

### [wc](./wc.py)
Implementation of the wc utility.

### [Haskell&Lisp](./haskellolisp.py)
Linked lists and functions to work with them in Haskell Lisp style without using loops, only recursion!

### [Fibonacci](./fibonacci.py)
Generate the function `fibN` if and only if `fibN-1` has already been generated (without using `exec` and `eval`).

```
>>> fib1()
1
>>> fib2()
1
>>> fib3()
2
>>> fib4()
3
>>> fib6()
NameError: name 'fib6' is not defined
>>> fib5()
5
>>> fib6()
8
```

### [Calm down](./calm_down.py)
A `calmer` decorator that sleeps the current thread if a function is called again before `1/max_rate` seconds have passed since the last call. This prevents the function from executing too frequently.

The `calmer` decorator can be used without any arguments, and the default value for `max_rate` is `1`.

```python
@calmer(max_rate=2)
def timer(start_time):
    passed_msc = (time.time_ns() - start_time) / (10 ** 6)
    print(f'passed msc since last call: {passed_msc:.2f}')

if __name__ == '__main__':
    start_time = time.time_ns()
    for i in range(10):
        timer(start_time)
        start_time = time.time_ns()

passed msc since last call: 0.03
passed msc since last call: 500.56
passed msc since last call: 500.60
passed msc since last call: 500.62
passed msc since last call: 500.10
passed msc since last call: 500.32
passed msc since last call: 500.77
passed msc since last call: 500.53
passed msc since last call: 500.23
passed msc since last call: 500.57
```

### [Polynomial](./polynomial.py)
Implement a class that represents polynomials over the field of real numbers. The class supports the following operations:

- Addition
- Subtraction
- Scalar multiplication
- Multiplication
- `__str__` and `__repr__`
- Degree calculation (length of the polynomial)
- Access/modification of the coefficient at an arbitrary degree by index
- Comparison operations

Additionally, "reflected" operations and assignment operations are implemented:

```python
poly1 = Poly([1, 2, 3])
poly2 = Poly([1, 2, 3])

poly1 += poly2
poly3 = 

3 * poly1
```

### [Implicit](./implicit.py)
A class decorator that, when applied to a class, allows accessing an undeclared instance attribute of that class to return the value from the object passed to `implicit` (or objects, if a tuple is passed).

```python
class E:
    e = 1

@implicit(E())
class A:
    pass

a = A()
print(a.e + 589)  # Output: 590
```

### [Transaction](./transaction.py)
A context manager that allows safe transaction handling.

Usage example:
```python
s = Storage()
with s.edit() as se:
    se['a'] = 1
    may_be_an_exception_here()
```

## Heap Sort
Heap sort implementation using generators.

A heap (in this case, a min-heap) is represented as follows:
  - An empty heap is a generator that does not yield any elements.
  - A non-empty heap is a generator that first yields a tuple `(value, size, depth)`, then the left sub-heap, and finally the right sub-heap (`value` is the value at the root of the heap, `size` is the number of elements in the heap, `depth` is the height of the heap).

In this implementation, tuples, lists, or other containers are not used.

## [What needs to be done](./classmethods.py)
The decorators `classmethod` and `staticmethods` was implemented