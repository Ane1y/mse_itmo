import numbers
from functools import total_ordering
from bigfloat import *


@total_ordering
class Poly:
    def __init__(self, coef):
        self.coef = list(map(lambda i: BigFloat(i), coef))

    def __add__(self, other):
        bigger = self.coef if len(self.coef) > len(other.coef) else other.coef
        less = other.coef if len(self.coef) > len(other.coef) else self.coef

        ans = [0] * len(bigger)

        for i in range(len(less)):
            ans[i] = bigger[i] + less[i]

        for i in range(len(less), len(bigger)):
            ans[i] = bigger[i]

        return Poly(ans)

    def __sub__(self, other):
        m_other = -1 * other
        return self + Poly(m_other)

    # умножение на скаляр,
    def __mul__(self, other):
        lgth = len(self.coef)
        if isinstance(other, numbers.Real):
            ans = [0] * lgth
            for i in range(lgth):
                ans[i] = other * self.coef[i]
        else:
            other_lgth = len(other.coef)
            ans = [0] * (lgth + other_lgth - 1)
            for i, a in enumerate(self.coef):
                for j, b in enumerate(other.coef):
                    ans[i + j] += a * b
        return Poly(ans)

    def __rmul__(self, other):
        return self.__mul__(other)

    # __str__ вызывается print() и format(), и в отличии от repr не ожидается,
    # что выведется валидный питоновский обьект
    def __str__(self):
        ans = ""
        for i in reversed(range(len(self.coef) - 2)):
            ans += f"{self.coef[i]} * x^{i} + "
        ans += f"{self.coef[0]} * x^{0}"
        return ans

    # официальное представление обьекта. по возможности выводит объект так,
    # чтобы его можно было восстановить
    def __repr__(self):
        return f"Poly({self.coef.__repr__()})"

    # вычисление длины (степень полинома),
    def __len__(self):
        return len(self.coef)

    # получение/изменение коэффициента при произвольной степени по индексу,
    def __getitem__(self, item):
        return self.coef[item]

    def __setitem__(self, key, value):
        if key >= len(self.coef):
            tmp = [0] * (key - len(self.coef) + 1)
            self.coef += tmp
        self.coef[key] = value

    # всевозможные операции сравнения.
    def __lt__(self, other):
        if len(self.coef) < len(other.coef):
            return True
        elif len(self.coef) > len(other.coef):
            return False
        return self.coef < other.coef

    def __eq__(self, other):
        return self.coef == other.coef
