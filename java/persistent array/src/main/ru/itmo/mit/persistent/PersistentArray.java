package ru.itmo.mit.persistent;

import org.jetbrains.annotations.Nullable;

import java.util.Iterator;


interface PersistentArray<T> {
    int getSize();

    int indexOf(Object elem);
    boolean contains(Object elem);

    @Nullable T get(int index);
    PersistentArray<T> set(int index, @Nullable T x);

    public Iterator<T> iterator();

}
