package ru.itmo.java;

interface Predicate<X> extends Function1<X, Boolean> {

    static Predicate<Object> ALWAYS_TRUE = x -> true;
    static Predicate<Object> ALWAYS_FALSE = x -> false;

    default Predicate<X> or(Predicate<? super X> g) {
        return (X x) -> apply(x) || g.apply(x);
    }

    default Predicate<X> and(Predicate<? super X> g) {
        return (X x) -> apply(x) && g.apply(x);
    }

    default Predicate<X> not() {
        return (X x) -> !this.apply(x);
    }

}