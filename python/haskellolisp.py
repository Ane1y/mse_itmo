#!/usr/bin/env python3

from collections import namedtuple

Nil = namedtuple("Nil", ())
Cons = namedtuple("Cons", ("car", "cdr"))


def null(lst):
    """
    >>> null(Nil())
    True
    >>> null(Cons(0, Nil()))
    False
    """
    return isinstance(lst, Nil)


def fromseq(seq):
    """
    >>> fromseq([])
    Nil()
    >>> fromseq(tuple())
    Nil()
    >>> fromseq([1, 2, 3])
    Cons(car=1, cdr=Cons(car=2, cdr=Cons(car=3, cdr=Nil())))
    """
    if len(seq) == 0:
        return Nil()
    return Cons(seq[0], fromseq(seq[1:]))


def head(lst):
    """
    >>> head(fromseq([1, 2, 3]))
    1
    >>> head(Nil())
    Traceback (most recent call last):
    ...
    AttributeError: 'Nil' object has no attribute 'car'
    """
    if null(lst):
        raise AttributeError("'Nil' object has no attribute 'car'")
    return lst.car


def tail(lst):
    """
    >>> tail(fromseq([1, 2, 3]))
    Cons(car=2, cdr=Cons(car=3, cdr=Nil()))
    >>> tail(fromseq([]))
    Traceback (most recent call last):
    ...
    AttributeError: 'Nil' object has no attribute 'cdr'
    """
    return lst.cdr


def foldr(func, acc, lst):
    """
    >>> foldr(lambda x, y: x + y, 0, Nil())
    0
    >>> foldr(lambda x, y: x + y, 2, fromseq([1, 2, 3]))
    8
    >>> foldr(lambda x, y: x - y, 1, fromseq([3, 2, 1]))
    1
    """
    if null(lst):
        return acc
    return func(head(lst), foldr(func, acc, tail(lst)))


def foldl(func, acc, lst):
    """
    >>> foldl(lambda x, y: x + y, 0, Nil())
    0
    >>> foldl(lambda x, y: x + y, 2, fromseq([1, 2, 3]))
    8
    >>> foldl(lambda x, y: x - y, 1, fromseq([3, 2, 1]))
    -5
    """
    if null(lst):
        return acc
    return foldl(func, func(acc, head(lst)), tail(lst))


def length(lst):
    """
    >>> length(Nil())
    0
    >>> length(fromseq((1, 2)))
    2
    """
    acc = 0
    return foldr(lambda x, count: count + 1, 0, lst)


def tolist(lst):
    """
    >>> tolist(Nil())
    []
    >>> tolist(Cons(1, Nil()))
    [1]
    >>> tolist(fromseq([1, 2, 3]))
    [1, 2, 3]
    """
    if null(lst):
        return []
    return foldr(lambda x, l: [x] + l, list(), lst)


def map_(func, lst):
    """
    >>> tolist(map_(lambda x: x, Nil()))
    []
    >>> tolist(map_(lambda x: x, fromseq([1, 2, 3])))
    [1, 2, 3]
    >>> tolist(map_(lambda x: str(x) + '0', fromseq([1, 2, 3])))
    ['10', '20', '30']
    """
    return foldr(lambda head, tail: Cons(func(head), tail), Nil(), lst)


def append(lst1, lst2):
    """
    >>> append(Nil(), fromseq([]))
    Nil()
    >>> append(Nil(), Cons(0, Cons(1, Nil())))
    Cons(car=0, cdr=Cons(car=1, cdr=Nil()))
    >>> append(fromseq([1]), Nil())
    Cons(car=1, cdr=Nil())
    >>> append(fromseq([1, 2]), fromseq([3]))
    Cons(car=1, cdr=Cons(car=2, cdr=Cons(car=3, cdr=Nil())))
    """
    return foldr(lambda cur, pred: Cons(cur, pred), lst2, lst1)


def filter_(pred, lst):
    """
    >>> filter_(lambda x: True, Nil())
    Nil()
    >>> tolist(filter_(lambda x: True, fromseq([1, 2])))
    [1, 2]
    >>> tolist(filter_(lambda x: False, fromseq([1, 2])))
    []
    >>> tolist(filter_(lambda x: x % 2 == 0, fromseq(range(5))))
    [0, 2, 4]
    """
    return foldr(lambda cur, prev: Cons(cur, prev) if pred(cur) else prev, Nil(), lst)


def reverse(lst):
    """
    >>> reverse(Nil())
    Nil()
    >>> tolist(reverse(fromseq(range(2))))
    [1, 0]
    >>> tolist(reverse(fromseq(range(3))))
    [2, 1, 0]
    """
    return foldl(lambda prev, cur: Cons(cur, prev), Nil(), lst)


def elem(value, lst):
    """
    >>> elem(10, Nil())
    False
    >>> elem(5, fromseq(range(5)))
    False
    >>> elem(5, fromseq(range(10)))
    True
    """
    return False if null(filter_(lambda x: x == value, lst)) else True


if __name__ == "__main__":
    import doctest

    doctest.testmod()
