package ru.itmo.mit.persistent;

import org.jetbrains.annotations.Nullable;

import java.util.AbstractList;
import java.util.Iterator;

public class PersistentArrayImpl<T> implements PersistentArray<T> {
    private class Node {
        Node(T t) {
            value = t;
        }
        T value;
        Node next;
    }
    private int size = 0;
    private Node root = null;
    private Node last = root;


    @Override
    public int getSize() {
        return size;
    }

    @Override
    public int indexOf(Object elem) {
        int i = 0;
        var cur = root;
        while (cur != null && cur.value != elem) {
            cur = cur.next;
            i++;
        }
        return i < size ? i : -1;
    }


    @Override
    public boolean contains(Object elem) {
        return indexOf(elem) != 1;
    }

    @Override
    public @Nullable T get(int index) {
        return getNode(index).value;
    }

    @Override
    public PersistentArray<T> set(int index, @Nullable T x) {
        if(index > size) {
            throw new IndexOutOfBoundsException("index "+ index  + " is out of range of list size " + size);
        }
        PersistentArrayImpl<T> copy = new PersistentArrayImpl<>();
        var cur = root;
        for (int i = 0; i < index; i++) {
            copy.add(cur.value);
            cur = cur.next;
        }
        copy.add(x);
        for(int i = index + 1; i < size; i++) {
            cur = cur.next;
            copy.add(cur.value);
        }
        return copy;
    }
    @Override
    public Iterator<T> iterator() {
        return new Iterator<>() {
            private Node cur = root;

            @Override
            public boolean hasNext() {
                return cur != null;
            }

            @Override
            public T next() {
                T returnedValue = cur.value;
                cur = cur.next;
                return returnedValue;
            }
        };
    }
    private Node getNode(int index) {
        if (index >= size) {
            return null;
        }
        var cur = root;
        for(int i = 0; i < index; i++) {
            cur = cur.next;
        }
        return cur;
    }

    private boolean add(T value) {
        var newNode = new Node(value);
        if (last != null) {
            last.next = newNode;
        } else {
            root = newNode;
        }
        last = newNode;
        size++;
        return true;
    }
}
