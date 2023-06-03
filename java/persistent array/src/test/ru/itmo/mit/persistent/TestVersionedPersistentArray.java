package ru.itmo.mit.persistent;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;

public class TestVersionedPersistentArray {
    @Test
    void testVersionedIterator() {
        Integer[][] versions= new Integer[][]{{}, {5}, {5, 6}, {10, 6}};
        ArrayList<PersistentArray<Integer>> v = new ArrayList<>(4);

        v.add(new VersionedPersistentArrayImpl<>());
        v.add(v.get(0).set(0, 5));
        v.add(v.get(1).set(1, 6));
        v.add(v.get(2).set(0, 10));
        for(int version = 0; version < versions.length; version++) {
            int i = 0;
            for(var el : versions[version]) {
                Assertions.assertEquals(el, v.get(version).get(i), "Compared version " + version);
                i++;
            }
        }
    }

    @Test
    void testGetPreviousVersion() {
        ArrayList<PersistentArray<Integer>> v = new ArrayList<>(3);
        VersionedPersistentArray<Integer> init = new VersionedPersistentArrayImpl<>();
        v.add(init.set(0, 5));
        v.add(v.get(0).set(1, 6));
        v.add(v.get(1).set(0, 10));
        Integer[] testArray = new Integer[]{10, 6};
        for(int i = 0; i < testArray.length; i++) {
            Assertions.assertEquals(testArray[i], v.get(2).get(i));
        }
        var lst = init.getPreviousVersion(2);
        for(int i = 0; i < lst.getSize(); i++) {
            Assertions.assertEquals(lst.get(i), v.get(2).get(i));
        }
    }
//        var v0 = new VersionedPersistentArrayImpl<Integer>();
//        VersionedPersistentArray<Integer> v1 = v0.set(0, 1);
//        var v2 = v1.set(1, 2);
//        var v3 = v2.set(2, 3);

//        Assertions.assertThrows(IllegalArgumentException.class, () -> newArray.set(0, value), "Check whether addition to not the last version is not possible");

}
