import functools
from typing import Type


def classmethod(method):


    @functools.wraps(method)
    def inner(self=None, *args, **kwargs):
        cls_name = method.__globals__[method.__qualname__.rpartition(".")[0]]
        if self is None:
            cls = cls_name
        elif isinstance(self, cls_name):
            cls = self.__class__
        else:
            args = (self, *args)
            cls = cls_name
        return method(cls, *args, **kwargs)

    return inner


def staticmethod(method):
    @functools.wraps(method)
    def inner(self=None, *args, **kwargs):
        cls_name = method.__globals__[method.__qualname__.rpartition(".")[0]]
        if self is not None and not isinstance(self, cls_name):
            args = (self, *args)
        return method(*args, **kwargs)

    return inner

