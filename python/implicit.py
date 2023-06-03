def implicit(cls_tuple):
    def deco(cls):
        def inner(obj, name):
            if isinstance(cls_tuple, tuple):
                cls_list = list(cls_tuple)
            else:
                cls_list = [cls_tuple]
            for c in cls_list:
                if name in dir(c):
                    return getattr(c, name)
            raise KeyError("attribute not found")

        cls.__getattr__ = inner
        return cls

    return deco


def get_attr(attr):
    print(attr)
