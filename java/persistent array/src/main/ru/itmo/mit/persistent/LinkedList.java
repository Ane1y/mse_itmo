package ru.itmo.mit.persistent;

import java.util.AbstractList;
import java.util.Iterator;

public class LinkedList<T> extends AbstractList<T> {
    private class Node {
        Node(T t) {
            value = t;
        }
        T value;
        Node next;
    }

    private Node head;
    private Node tail;
    private int size = 0;

    @Override
    public T get(int index) {
        if (index >= size) {
            return null;
        }
        var cur = head;
        for (int i = 0; i < index; i++) {
            cur = cur.next;
        }
        return cur.value;
    }

    @Override
    public int size() {
        return size;
    }

    @Override
    public boolean add(T value) {
        var newNode = new Node(value);
        if (tail != null) {
            tail.next = newNode;
        } else {
            head = newNode;
        }
        tail = newNode;
        size++;
        return true;
    }
}
