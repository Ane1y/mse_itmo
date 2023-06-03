package ru.itmo.mit.persistent;

import java.util.Iterator;

public interface VersionedPersistentArray<T> extends PersistentArray<T> {

    Iterator<PersistentArray<T>> versionsIterator();

    PersistentArray<T> getPreviousVersion(int k);
}
