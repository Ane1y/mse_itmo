w# 1
def empty():
    yield from []


# 2
def heapify(value, heap1, heap2):
    left = next(heap1, None)
    right = next(heap2, None)

    if not left and not right:
        yield (value, 1, 1)
        yield empty()
        yield empty()

    if left and right:
        if value >= left[0] and left[0] <= right[0]:
            smallest = left
            bigger = (value, 0, 0)
            biggest = right
        elif value <= left[0] and value <= right[0]:
            smallest = (value, 0, 0)
            bigger = left
            biggest = right
        else:
            smallest = right
            bigger = left
            biggest = (value, 0, 0)

        yield (smallest[0], left[1] + right[1] + 1, max(left[2], right[2]) + 1)
        yield heapify(bigger[0], next(heap1), next(heap1))
        yield heapify(biggest[0], next(heap2), next(heap2))

    if left and not right or right and not left:
        subheap, main_heap = (
            (left, heap1) if left is not None and right is None else (right, heap2)
        )
        largest, smallest = (
            (value, subheap[0]) if value >= subheap[0] else (subheap[0], value)
        )

        yield (smallest, subheap[1] + 1, subheap[2] + 1)
        yield empty()
        yield heapify(largest, next(main_heap), next(main_heap))


# 3
def print_heap(heap, prefix=""):
    root = next(heap, None)
    # (value, size, depth)
    if root:
        print(f"{prefix}[{root[1]}, {root[2]}]: {root[0]}")
        print_heap(next(heap), prefix + " " * 4)
        print_heap(next(heap), prefix + " " * 4)


# 4
def build_heap(seq):
    # Build_Heap(A):
    # A.heap_size ← A.length
    # for i ← ⌊A.length / 2⌋ downto 1
    # do
    # Heapify(A, i)

    seq = list(seq)
    length = len(seq)

    if length == 0:
        return empty()
    for i in range(length // 2, length):
        seq[i] = heapify(seq[i], empty(), empty())

    for i in reversed(range(0, length // 2)):
        l_idx = 2 * i + 1
        r_idx = 2 * i + 2
        left = seq[l_idx] if l_idx < length else empty()
        right = seq[r_idx] if r_idx < length else empty()
        seq[i] = heapify(seq[i], left, right)

    return seq[0]


# 5
def extract_last(heap):
    pass


# 6
def extract_min(heap):
    pass


# 7
def heap_sort(seq):
    pass


# 8
def bench():
    pass
