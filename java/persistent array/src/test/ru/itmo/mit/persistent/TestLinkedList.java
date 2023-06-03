package ru.itmo.mit.persistent;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;

import java.util.Arrays;
import java.util.List;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
public class TestLinkedList {
    LinkedList<Integer> lst = new LinkedList<>();
    Integer[] testInts = {1, 5, 7, 9, 4};
    @BeforeAll
    public void setUp() {
        lst.addAll(Arrays.asList(testInts));
    }

    @Test
    public void testGet() {
        for(int i = 0; i < testInts.length; i++) {
            Assertions.assertEquals(testInts[i], lst.get(i));
        }
    }

    @Test
    public void testSize() {
        List<Integer> testLst = new LinkedList<>();
        testLst.addAll(Arrays.asList(testInts));
        Assertions.assertEquals(testInts.length, testLst.size());
    }



    @Test
    public void testAdd() {
        int idx = lst.size();
        Integer newValue = 3;
        Assertions.assertTrue(lst.add(newValue));
        Assertions.assertEquals(newValue, lst.get(idx));
    }
}

