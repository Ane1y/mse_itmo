package ru.itmo.mit.persistent;

import org.jetbrains.annotations.Nullable;

import java.util.Iterator;

public class VersionedPersistentArrayImpl<T> implements VersionedPersistentArray<T>  {

    PersistentArray<T> curLst = new PersistentArrayImpl<>();
    private LinkedList<PersistentArray<T>> versions = new LinkedList<>();

    @Override
    public Iterator<PersistentArray<T>> versionsIterator() {
        return new Iterator<>() {
            final Iterator<PersistentArray<T>> it = versions.iterator();

            @Override
            public boolean hasNext() {
                return it.hasNext();
            }

            @Override
            public PersistentArray<T> next() {
                return it.next();
            }
        };
    }

    @Override
    public PersistentArray<T> getPreviousVersion(int k) {
        return versions.get(k);
    }

    @Override
    public int getSize() {
        return curLst.getSize();
    }

    @Override
    public int indexOf(Object elem) {
        return curLst.indexOf(elem);
    }

    @Override
    public boolean contains(Object elem) {
        return curLst.contains(elem);
    }

    @Override
    public @Nullable T get(int index) {
        return curLst.get(index);
    }

    @Override
    public VersionedPersistentArray<T> set(int idx, T value) {
        var newArray = new VersionedPersistentArrayImpl<T>();
        newArray.versions = versions;
        newArray.curLst = curLst.set(idx, value);
        newArray.versions.add(newArray.curLst);
        return newArray;
    }

    @Override
    public Iterator<T> iterator() {
        return null;
    }

}
