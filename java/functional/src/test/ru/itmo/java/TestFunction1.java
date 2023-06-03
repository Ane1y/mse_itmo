package ru.itmo.java;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class TestFunction1 {
    Function1<Integer, Integer> inc = x -> x + 1;
    Function1<Integer, Integer> multBy2 = x -> x * 2;
    Function1<Integer, Boolean> is_greater_10 = x -> x >  10;

    @Test
    void testSimpleFunc() {
        Assertions.assertEquals(6, inc.apply(5));
        Assertions.assertEquals(false, is_greater_10.apply(6));
    }

    @Test
    void testCompose() {
        Assertions.assertEquals(6, inc.compose(multBy2).apply(2));
        Assertions.assertEquals(5, multBy2.compose(inc).apply(2));
    }

    class Grandparent {
        private final Integer value = 5;

        Integer get() {
            return value;
        }
    }
    class Parent extends Grandparent {
        private final Integer value = 7;
        @Override
        Integer get() {
            return value;
        }
    }

    class Me extends Parent {
        private final Integer value;
        Me(int x) {
            value = x + 1;
        }
        @Override
        Integer get() {
            return value;
        }
    }


    @Test
    void testMostCommonType() {
        Function1<Integer, Parent> f = x -> new Me(x);
        Function1<Grandparent, Double> g = x -> x.get() * 2.0;

        Assertions.assertEquals(12, f.compose(g).apply(5));
    }
}
