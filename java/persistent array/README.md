# Persistent Array

This Java project implements a Persistent Array data structure using lists.
The Persistent Array provides a way to modify an array without altering the original array and returns a new modified array instead.

## Features

- The implementation includes a method called "path copying" that ensures persistence.
- Standard collections are not used in the implementation (custom data structures are used instead).
- Supports explicit versioning of array states with the implementation of `VersionedPersistentArray`.
- The Persistent Array also provides an `ValuesIterator` to iterate over the values of the array. This iterator does not support mutable operations.

## Modification History

You can call the `getHistory()` method on the Persistent Array to retrieve the modification history of the array. The history contains information about each sequential operation and the resulting state of the array. The format of the history display is human-readable and can vary based on the implementation.
