package ru.itmo.java;


import java.util.List;

public interface Function1<X, F> { // <X - arg type, F - return type>
    F apply(X x);

    default <G> Function1 <X, G> compose(Function1<? super F, ? extends G> g) {
        return x -> g.apply(apply(x));
    }


}