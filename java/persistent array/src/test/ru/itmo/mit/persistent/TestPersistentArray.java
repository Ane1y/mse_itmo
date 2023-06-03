package ru.itmo.mit.persistent;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

public class TestPersistentArray {
    static PersistentArray<Integer> lst = new PersistentArrayImpl<>();
    static Integer[] testInts = {1, 5, 7, 9, 4};
    @BeforeAll
    public static void setUp() {
        for (int i = 0; i < testInts.length; i++) {
            lst = lst.set(i, testInts[i]);
        }
    }

    @Test
    void testIterator() {
        var it = lst.iterator();
        Assertions.assertThrows(UnsupportedOperationException.class, it::remove);
        int i = 0;
        while(it.hasNext()) {
            Assertions.assertEquals(testInts[i], it.next());
            i++;
        }
    }
    @Test
    public void testGet() {
        for(int i = 0; i < testInts.length; i++) {
            Assertions.assertEquals(testInts[i], lst.get(i));
        }

    }

    @Test
    public void testSize() {
        Assertions.assertEquals(testInts.length, lst.getSize());
    }


    @Test
    public void testSet() {
        int idx = 2;
        Integer newValue = 70;
        var newLst = lst.set(idx, newValue);
        Assertions.assertEquals(testInts[idx], lst.get(idx));
        Assertions.assertEquals(newValue, newLst.get(idx));
    }

    @Test
    public void testPersistentSet() {
        PersistentArray<Integer> testLst = new PersistentArrayImpl<>();
        for (int i = 0; i < testInts.length; i++) {
            testLst = testLst.set(i, testInts[i]);
        }
        int idx = 2;
        int newValue = 30;
        PersistentArray<Integer> newList = testLst.set(idx, newValue);
        for(int i = 0; i < testInts.length; i++) {
            Assertions.assertEquals(testInts[i], testLst.get(i), "OldList");
        }

        for(int i = 0; i < testInts.length; i++) {
            if (i != idx) {
                Assertions.assertEquals(testInts[i], newList.get(i), "NewList, i:" + i);
            } else {
                Assertions.assertEquals(newValue, newList.get(i), "NewList");
            }
        }
    }
    @Test
    void testSetAsAdd() {
        Integer value = 6;
        var array = new PersistentArrayImpl<>();
        var newArray = array.set(0, value);
        Assertions.assertEquals(value, newArray.get(0));
        Assertions.assertThrows(IndexOutOfBoundsException.class, () -> array.set(10, 4));
    }


}
