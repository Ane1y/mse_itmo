package ru.itmo.java;


import java.util.ArrayList;
import java.util.List;

class Collections {
    
    static public<X, F> List<F> map(Iterable<? extends X> a, Function1<? super X, ? extends F> func) {
        List<F> f_a = new ArrayList<>();
        a.forEach(x -> f_a.add(func.apply(x)));
        return f_a;
    }

    static public <A> Iterable<? extends A> filter(Predicate<? super A> p, Iterable<? extends A> a) {
        List<A> filteredList = new ArrayList<>();
        a.forEach(x -> { if (p.apply(x)) filteredList.add(x);});
        return filteredList;
    }

    static public <A> Iterable<A> takeWhile(Predicate<? super A> p, Iterable<? extends A> a) {
        List<A> whileList = new ArrayList<>();
        for(var x : a) {
           if (p.apply(x)) {
               break;
           }
           whileList.add(x);
        }
        return whileList;
    }

    static public <A> Iterable<A> takeUnless(Predicate<? super A> p, Iterable<? extends A> a) {
        return takeWhile(p.not(), a);
    }

    static public <A, B> A foldl(Function2<? super A, ? super B, ? extends A> func,
                                 A init,
                                 Iterable<? extends B> a) {
        for(var obj : a) {
            init = func.apply(init, obj);
        }
        return init;
    }


    static public <A, B> B foldr(Function2<? super A, ? super B, ? extends B> func, B init, Iterable<? extends A> a) {
        List<A> buf = new ArrayList<>();
        a.forEach(x -> buf.add(x));
        if (buf.size() > 0) {
            for (int i = buf.size() - 1; i >= 0; i--) {
                init = func.apply(buf.get(i), init);
            }
        }
        return init;
    }
}

