package ru.itmo.java;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;


public class TestFunction2 {
    Function2<Integer, Integer, Integer> add = (x, y) -> x + y;
    Function2<String, String, String> concat = (x, y) -> x + y;

    @Test
    void testSimpleFunc() {
        Function2<Integer, Integer, Boolean> less = (x, y) -> x < y;
        Assertions.assertEquals(6, add.apply(4, 2));
        Assertions.assertEquals(false, less.apply(8, 5));
    }

    @Test
    void testCompose() {
        Function1<Integer, Integer> square = x -> x * x;
        Assertions.assertEquals(25, add.compose(square).apply(2, 3));
    }

    @Test
    void testBind() {
        Function1<String, String> concatWithABC = x -> x  + "ABC";
        Function1<String, String> concatWithCBA = x -> "CBA" + x;

        String inputStr = "qwe";
        Assertions.assertEquals(concat.bind1("CBA").apply(inputStr), concatWithCBA.apply(inputStr));
        Assertions.assertEquals(concat.bind2("ABC").apply(inputStr), concatWithABC.apply(inputStr));
    }

    @Test
    void testCurry() {
        Assertions.assertEquals(11, add.curry().apply(5).apply(6));
        Assertions.assertEquals("ABCabc", concat.curry().apply("ABC").apply("abc"));
    }
    class Grandparent {
        private Integer value = 1;

        Integer get() {
            return value;
        }

    }

    class Parent extends Grandparent {
        private Integer value = 1;
        @Override
        Integer get() {
            return value;
        }
    }
    class Me extends Parent {
        private Integer value;
        @Override
        Integer get() {
            return value;
        }

        Me(int value) {
            this.value = value + 1;
        }
    }

    @Test
    void testTheMostCommonType() {
        Function2<Parent, Me, Parent> f = (x, y) -> new Me(x.get() + y.get());
        Function1<Grandparent, Integer> g = a -> a.value + 5;
        Assertions.assertEquals(6, f.compose(g).apply(new Me(2), new Me(3)));
    }


}
