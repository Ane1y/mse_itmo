package ru.itmo.mit.trie;

import java.util.Map;
import java.util.TreeMap;


public class TrieImpl implements Trie {

    private static class Vertex {
        public TreeMap<Character, Vertex> children = new TreeMap<>();
        int subtreeSize = 0;
        public Boolean isTerminal = false;
    }

    private final Vertex root;

    public TrieImpl() {
        root = new Vertex();
    }
    @Override
    public boolean add(String element) {
        if (contains(element)) {
            return false;
        }
        Vertex cur = root;
        root.subtreeSize += 1;
        for(int i = 0 ; i < element.length(); i++) {
            char c = element.charAt(i);
            if (!cur.children.containsKey(c)) {
                cur.children.put(c, new Vertex());
            }
            cur.children.get(c).subtreeSize += 1;
            cur = cur.children.get(c);
        }
        cur.isTerminal = true;
        return true;
    }

    @Override
    public boolean contains(String element) {
        Vertex lastVert = traverse(element);
        return (lastVert != null) ? lastVert.isTerminal : false;
    }

    @Override
    public boolean remove(String element) {
        if (!contains(element)) {
            return false;
        }
        Vertex cur = root;
        root.subtreeSize--;
        if (!element.equals("") && root.children.get(element.charAt(0)).subtreeSize == 0) {
            root.children.remove(element.charAt(0));
            return true;
        }

        for (int i = 0; i < element.length(); i++) {
            char c = element.charAt(i);
            if (cur.subtreeSize == 0) {
                cur.children.remove(c);
                return true;
            }
            cur = cur.children.get(c);
            cur.subtreeSize--;
        }
        cur.isTerminal = false;

        return true;
    }

    @Override
    public int size() {
        return root.subtreeSize;
    }

    @Override
    public int howManyStartsWithPrefix(String prefix) {
        Vertex lastVert = traverse(prefix);
        return (lastVert == null) ? 0 : lastVert.subtreeSize;
    }

    @Override
    public String nextString(String element, int k) {
        if (k < 0) {
            throw new IllegalArgumentException("k should be greater than or equal to zero");
        }
        if (k > root.subtreeSize) {
            return null;
        }
        Vertex lastVert = traverse(element);
        if (k == 0) {
            return (lastVert == null) ? null : element;
        }

        if (lastVert == null) {
            add(element);
        }
        Vertex cur = root;

        StringBuilder buffer = new StringBuilder();
        while (k > 0) {
            for (Map.Entry<Character, Vertex> entry : cur.children.entrySet()) {
                var key = entry.getKey();
                var child = entry.getValue();
                if ((cur.subtreeSize == child.subtreeSize) || (k <= child.subtreeSize)) {
                    buffer.append(key);
                    cur = child;
                } else {
                    k -= child.subtreeSize;
                }
            }
            if (cur.children.isEmpty()) {
                break;
            }

            if (k == 1) {
                do {
                    buffer.append(cur.children.firstKey());
                    cur = cur.children.firstEntry().getValue();
                } while(!cur.isTerminal);
            }

        }

        if(lastVert == null) {
            remove(element);
        }
        return buffer.toString();
    }


    /**
     * @return the end of element or null if there are no such element in trie
     */
    private Vertex traverse(String element) {
        Vertex cur = root;

        for(int i = 0, n = element.length(); i < n ; i++) {
            char c = element.charAt(i);
            if (cur.children.containsKey(c)) {
                cur = cur.children.get(c);
            } else {
                return null;
            }
        }
        return cur;
    }

}
