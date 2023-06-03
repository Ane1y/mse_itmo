package ru.itmo.java;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;

public class TestCollections {
    @Test
    void testMap() {
        List<Integer> lst = List.of(1, 2, 3);
        Function1<Integer, Integer> inc = x -> x + 1;
        Assertions.assertEquals(List.of(2, 3, 4), Collections.map(lst, inc));
    }

    @Test
    void testMapId() {
        List<Integer> lst = List.of(1, 2, 3);
        Function1<Integer, Integer> id = x -> x;
        Assertions.assertEquals(lst, Collections.map(lst, id));
    }
    @Test
    void testFilter() {
        Predicate<Integer> geq20 = x -> x >= 20;
        List<Integer> lst = List.of(10, 20, 3, 35, 5);
        Assertions.assertEquals(List.of(20, 35), Collections.filter(geq20, lst));
    }

    @Test
    void testFilterWithAlwaysTrue() {
        List<Integer> lst = new ArrayList<>();
        List<Integer> expected = new ArrayList<>();
        for (int i = 0; i < 100; i++) {
            lst.add(i);
            expected.add(i);
        }
        Assertions.assertEquals(expected, Collections.filter(Predicate.ALWAYS_TRUE, lst));
    }

    @Test
    void testFilterWithAlwaysFalse() {
        List<Integer> lst = new ArrayList<>();
        for (int i = 0; i < 100; i++) {
            lst.add(i);
        }
        Assertions.assertEquals(new ArrayList<Integer>(), Collections.filter(Predicate.ALWAYS_FALSE, lst));
    }

    @Test
    void testTakeWhile() {
        List<Integer> lst = List.of(1, 2, 3, 4, 5, 6);
        Predicate<Integer> gt3 = x -> x > 3;
        Assertions.assertEquals(List.of(1, 2, 3), Collections.takeWhile(gt3, lst));
    }


    @Test
    void testTakeWhileAlwaysTrue() {
        List<Integer> lst = new ArrayList<>();
        for (int i = 0; i < 100; i++) {
            lst.add(i);
        }
        Assertions.assertEquals(new ArrayList<Integer>(), Collections.takeWhile(Predicate.ALWAYS_TRUE, lst));
    }
    @Test
    void testTakeWhileAlwaysFalse() {
        List<Integer> lst = new ArrayList<>();
        for (int i = 0; i < 100; i++) {
            lst.add(i);
        }
        Assertions.assertEquals(lst, Collections.takeWhile(Predicate.ALWAYS_FALSE, lst));
    }

    @Test
    void testTakeUnless() {
        List<Integer> lst = List.of(1, 2, 3, 4, 5, 6);
        Predicate<Integer> leq3 = x -> x <= 3;
        Assertions.assertEquals(List.of(1, 2, 3), Collections.takeUnless(leq3, lst));
    }


    @Test
    void testTakeUnlessAlwaysTrue() {
        List<Integer> lst = new ArrayList<>();
        for (int i = 0; i < 100; i++) {
            lst.add(i);
        }
        Assertions.assertEquals(lst, Collections.takeUnless(Predicate.ALWAYS_TRUE, lst));
    }
    @Test
    void testTakeUnlessAlwaysFalse() {
        List<Integer> lst = new ArrayList<>();
        for (int i = 0; i < 100; i++) {
            lst.add(i);
        }
        Assertions.assertEquals(new ArrayList<Integer>(), Collections.takeUnless(Predicate.ALWAYS_FALSE, lst));

    }

    @Test
    void testFoldlSum() {
        List<Integer> list = new ArrayList<>();
        int sum = 0;
        for (int i = 1; i < 100; i++) {
            list.add(i);
            sum += i;
        }
        Function2<Integer, Integer, Integer> f = (x, y) -> x + y;
        Assertions.assertEquals(sum, Collections.foldl(f, 0, list));
    }

    @Test
    void testFoldrSum() {
        List<Integer> list = new ArrayList<>();
        int sum = 0;
        for (int i = 1; i < 100; i++) {
            list.add(i);
            sum += i;
        }
        Function2<Integer, Integer, Integer> f = (x, y) -> x + y;
        Assertions.assertEquals(sum, Collections.foldr(f, 0, list));
    }

    @Test
    void testEqualsOfFolds() {
        List<Integer> list = new ArrayList<>();
        for (int i = 1; i < 100; i++) {
            list.add(i);
        }
        Function2<Integer, Integer, Integer> f = (x, y) -> x + y;
        Assertions.assertEquals(Collections.foldr(f, 0, list), Collections.foldl(f, 0, list));
    }
}
