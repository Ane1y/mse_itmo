# import inspect
def generate(n):
    # inspect.stack()
    def fib_n():
        if f"fib{n + 1}" not in globals():
            generate(n + 1)
        return globals()[f"fib{n - 1}"]() + globals()[f"fib{n - 2}"]()

    globals()["fib" + str(n)] = fib_n


def fib1():
    return 1


def fib2():
    return 1


def fib3():
    generate(4)
    return fib1() + fib2()


if __name__ == "__main__":
    print(fib1())
    print(fib2())
    print(fib3())
    print(fib4())
    print(fib5())
    print(fib6())
