# Functional Java

This project focuses on creating abstract classes and interfaces for functional programming in Java. It involves the following abstractions:

* `Function1` - a function with one argument (`f(x)`)
* `Function2` - a function with two arguments (`f(x, y)`)
* `Predicate` - a predicate for one argument

* `Function1.compose` - Function composition. It takes a `Function1 g` as an argument and returns `g(f(x))`.
* `Function2.compose` - Function composition. It takes a `Function1 g` as an argument and returns `g(f(x, y))`.
* `Function2.bind1` - Binds the first argument. It takes the first argument and returns `f(_, y)`.
* `Function2.bind2` - Binds the second argument. It takes the second argument and returns `f(x, _)`.
* `Function2.curry`  converting the function to a `Function1`.

#### Predicate
* `Predicate.or`/`Predicate.and` - These methods take a predicate as an argument and return a predicate that behaves as the disjunction/conjunction of the current predicate and the argument predicate. The semantics are lazy, similar to `||` and `&&` operators.
* `Predicate.not` - Takes no arguments and returns the negation of the current predicate.
* Constant predicates: `Predicate.ALWAYS_TRUE`, `Predicate.ALWAYS_FALSE`

## Collections Class
`Collections` class that operate on `Iterable`, `Function1`, `Function2`, and `Predicate`:

* `map` - Takes a function `f` and an iterable `a`, applies `f` to each element `a_i`, and returns a list `[f(a_1), ..., f(a_n)]`.
* `filter` - Takes a predicate `p` and an iterable `a`, returns a list containing elements `a_i` for which `p(a_i) == true`.
* `takeWhile` - Takes a predicate `p` and an iterable `a`, returns a list containing elements from the start of `a` until the first element `a_i` for which `p(a_i) == false`.
* `takeUnless` - Same as `takeWhile`, but for `p(a_i) == true`.
* `foldr` / `foldl` - Takes a function with two arguments, an initial value, and a collection. It performs a fold operation as described
