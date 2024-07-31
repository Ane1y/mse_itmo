import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.atomic.AtomicMarkableReference;

public class SetImpl<T extends Comparable<T>> implements Set<T> {
    // comparable implements compareTo: -int, 0 if equals, +int
    private final Node head;

    public SetImpl() {
        head = new Node(null, null);
    }
    @Override
    public boolean add(T value) {
        while(true) {
            Pair<Node> placeToAdd = find(value);
            Node prev = placeToAdd.fst;
            Node cur = placeToAdd.snd;
            if (cur != null && cur.value.compareTo(value) == 0) {
                return false;
            } else {
                Node node = new Node(value, cur);
                if (prev.next.compareAndSet(cur, node, false, false)) {
                    return true;
                }
            }
        }
    }
    @Override
    public boolean remove(T value) {
        while (true) {
            Pair<Node> placeToDelete = find(value);
            Node prev = placeToDelete.fst;
            Node cur = placeToDelete.snd;
            if (cur == null /*list is empty*/ || cur.value.compareTo(value) != 0) {
                return false;
            }
            Node succ = cur.next.getReference();
            // logical removing
            if (cur.next.attemptMark(succ, true)) {
                // actual one
                if (prev.next.compareAndSet(cur, succ, false, false)) {
                    return true;
                }
            }
        }
    }

    @Override
    public boolean contains(T value) {
        // head is always null, so skip it
        Node prev = head;
        Node cur = prev.next.getReference();
        while (cur != null && cur.value.compareTo(value) < 0) {
            prev = cur;
            cur = prev.next.getReference();
        }
        return (cur != null && cur.value == value && !cur.next.isMarked());
    }

    @Override
    public boolean isEmpty() {
        return head.next.getReference() == null;
    }

    @Override
    public Iterator<T> iterator() {
        return getSnapshot().iterator();
    }

    List<T> getSnapshot() {
        while(true) {
            List<T> fstSnapshot = unsafeSnapshot();
            List<T> sndSnapshot = unsafeSnapshot();
            if (fstSnapshot.equals(sndSnapshot)) {
                return fstSnapshot;
            }
        }
    }

    List<T> unsafeSnapshot() {
        List<T> snapshot = new ArrayList<>();
        // dummy node
        Node prev = head;
        Node cur = prev.next.getReference();
        while(cur != null) {
            if (!prev.next.isMarked()) {
                snapshot.add(cur.value);
            }
            prev = cur;
            cur = prev.next.getReference();
        }
        return snapshot;
    }
    private Pair<Node> find(T value) {
        Node prev = head;
        Node cur = prev.next.getReference();
        while(cur != null) {
            Node succ = cur.next.getReference();
            if (cur.next.isMarked()) {
                if (!prev.next.compareAndSet(cur, succ, false, false)) {
                    prev = head;
                    cur = prev.next.getReference();
                    continue;
                }
            } else {
                if (cur.value.compareTo(value) >= 0) {
                    return new Pair<>(prev, cur);
                }
                prev = cur;
            }
            cur = succ;
        }
        return new Pair<>(prev, cur);
    }
    private class Node {
        public final T value;
        public final AtomicMarkableReference<Node> next;

        public Node(T value, Node next) {
            this.value = value;
            this.next = new AtomicMarkableReference<>(next, false);
        }
    }

    private static class Pair<T> {
        public final T fst;
        public final T snd;

        public Pair(T fst, T snd) {
            this.fst = fst;
            this.snd = snd;
        }

    }
}
