import org.junit.jupiter.api.Assertions
import org.junit.jupiter.api.Test

class SetBasicTest {
    val basicValues = arrayListOf(5, 7, 1);

    @Test
    fun testBasic() {
        val set = SetImpl<Int>();
        for (value in basicValues) {
            set.add(value);
        }

        for (value in basicValues) {
            Assertions.assertTrue(set.contains(value));
        }
    }

    @Test
    fun testRemove() {
        val set = SetImpl<Int>();
        for (value in basicValues) {
            set.add(value);
        }

        for (value in basicValues) {
            Assertions.assertTrue(set.contains(value));
        }
        for (value in basicValues) {
            set.remove(value);
        }

        for (value in basicValues) {
            Assertions.assertFalse(set.contains(value));
        }
    }

    @Test
    fun testAddExisting() {
        val set = SetImpl<Int>();
        for (value in basicValues) {
            set.add(value);
        }

        for (value in basicValues) {
            Assertions.assertTrue(set.contains(value));
        }

        for (value in basicValues) {
            Assertions.assertFalse(set.add(value));
        }
    }

    @Test
    fun testRemoveDeleted() {
        val set = SetImpl<Int>();
        for (value in basicValues) {
            set.add(value);
        }

        for (value in basicValues) {
            Assertions.assertTrue(set.contains(value));
        }
        for (value in basicValues) {
            Assertions.assertTrue(set.remove(value));
        }
        for (value in basicValues) {
            Assertions.assertFalse(set.remove(value));
        }
    }

    @Test
    fun testIterator() {
        val set = SetImpl<Int>();
        for (value in basicValues) {
            set.add(value);
        }

        val lst  = set.iterator().asSequence().toList();

        val sortedValues = basicValues.sorted();
        Assertions.assertTrue(lst.equals(sortedValues));
    }

    @Test
    fun testIteratorOnRemovedValues() {
        val set = SetImpl<Int>();
        for (value in basicValues) {
            set.add(value);
        }

        val removedValue = basicValues[0];

        set.remove(removedValue);

        val lst  = set.iterator().asSequence().toList();

        val sortedValues = ArrayList(basicValues.sorted());
        sortedValues.remove(removedValue);
        Assertions.assertTrue(lst.equals(sortedValues));
    }

    @Test
    fun testContainsMissingElement() {
        val set = SetImpl<Int>();

        Assertions.assertFalse(set.contains(1));
        set.add(5);

        Assertions.assertFalse(set.contains(7));
    }
}
