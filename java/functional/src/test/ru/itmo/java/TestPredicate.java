package ru.itmo.java;


import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class TestPredicate {
    @Test
    void testAlwaysTrue() {
        Assertions.assertTrue(Predicate.ALWAYS_TRUE.apply(true));
        Assertions.assertTrue(Predicate.ALWAYS_TRUE.apply(false));
        Assertions.assertTrue(Predicate.ALWAYS_TRUE.apply("bip"));
    }
    @Test
    void testAlwaysFalse() {
        Assertions.assertFalse(Predicate.ALWAYS_FALSE.apply(true));
        Assertions.assertFalse(Predicate.ALWAYS_FALSE.apply(false));
        Assertions.assertFalse(Predicate.ALWAYS_FALSE.apply("bip"));
    }
    @Test
    void testOr() {
        Predicate<Integer> first = x -> x == 20;
        Predicate<Integer> second = x -> x < 20;

        Assertions.assertTrue(first.or(second).apply(19));
        Assertions.assertFalse(first.or(second).apply(35));
    }

    @Test
    void testAnd() {
        Predicate<Integer> first = x -> x >= 20;
        Predicate<Integer> second = x -> x <= 20;

        Assertions.assertTrue(first.and(second).apply(20));
        Assertions.assertFalse(first.and(second).apply(12));
    }

    @Test
    void testNot() {
        Predicate<Integer> lessThan20 = x -> x < 20;
        Assertions.assertTrue(lessThan20.not().apply(20));
    }

}
