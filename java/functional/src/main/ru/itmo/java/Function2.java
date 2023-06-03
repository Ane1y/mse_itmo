package ru.itmo.java;

public interface Function2<X, Y, F> {
    F apply(X x, Y y);

    default <G> Function2<X, Y, G> compose(Function1<? super F, ? extends G> g) {
        return (X x, Y y) ->  g.apply(apply(x, y));
    }

    default Function1 <Y, F> bind1(X x) {
        return (Y y) -> apply(x, y);
    }

    default Function1 <X, F> bind2(Y y) {
        return (X x) -> apply(x, y);
    }

    default Function1<X, Function1<Y, F>> curry() {
        return (X x) -> (Y y) -> apply(x, y);
    }
}
