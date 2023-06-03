package ru.itmo.mit.trie;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.HashSet;
import java.util.Random;
import java.util.Set;

public class TrieTest {
    private TrieImpl testTrie;
    private final String[] basicWords = {"H", "Hello", "Hept", "Hell", "", "Hekmv"};
    private final String[] randomWords;

    private TrieTest() {
        randomWords = generateWords(10000);
    }

    //some helpers function to build really big tries
    private static String[] generateWords(int numberOfWords) {
        //https://stackoverflow.com/questions/4951997/generating-random-words-in-java
        String[] randomStrings = new String[numberOfWords];
        Random random = new Random();
        for(int i = 0; i < numberOfWords; i++) {
            char[] word = new char[random.nextInt(0, 15)];
            for(int j = 0; j < word.length; j++) {
                word[j] = (char)('a' + random.nextInt(26));
            }
            randomStrings[i] = new String(word);
        }

        return randomStrings;
    }

    void fillWithPrefixes(String element, Set<String> set) {
        StringBuilder word = new StringBuilder();
        for (int i = 0; i < element.length(); i++) {
            char c = element.charAt(i);
            word.append(c);
            testTrie.add(word.toString());
            set.add(word.toString());
        }
    }

    @BeforeEach
    public void setUp() {
        testTrie = new TrieImpl();
    }

    public void buildTrie(String[] words) {
        for (var word : words) {
            testTrie.add(word);
        }
    }

    @Test
    public void testAddEmpty() {
        Assertions.assertTrue(testTrie.add(""));
        Assertions.assertTrue(testTrie.contains(""));
    }

    @Test
    public void testBasicAdd() {
        for (var word : basicWords) {
            testTrie.add(word);
        }
        String[] new_words = new String[]{"He", "Some"};
        String[] existing_words = new String[]{"Hello", "Hept"};
        for (String word : new_words) {
            Assertions.assertTrue(testTrie.add(word), word);
        }
        for (String word : existing_words) {
            Assertions.assertFalse(testTrie.add(word), word);
        }
    }
    @Test
    public void testContains() {
        buildTrie(basicWords);
        String[] wrong_words = new String[]{"He", "Hep", "Sam", "Hellp"};
        for (String word : basicWords) {
            Assertions.assertTrue(testTrie.contains(word), word);
        }
        for (String word : wrong_words) {
            Assertions.assertFalse(testTrie.contains(word), word);
        }
    }
    @Test
    public void testRemove() {
        String words[] = {"H", "Hello", "Root", "Hell", "Hekt", "Hekmv"};
        for (var word: words) {
            Assertions.assertTrue(testTrie.add(word));
        }

        for (int i = words.length - 1; i >= 0; i--) {
            Assertions.assertTrue(testTrie.remove(words[i]), "Tried to delete " + words[i]);
             Assertions.assertFalse(testTrie.contains(words[i]), words[i] + " is contained in trie but it shouldn't");
            Assertions.assertEquals(i, testTrie.size());
        }

        Assertions.assertEquals(testTrie.size(), 0);
        for (var word: words) {
            Assertions.assertFalse(testTrie.contains(word), word + " is contained in trie but it shouldn't");
        }
    }

    @Test
    public void testEmptyStringRemove() {
        String words[] = {"S", "V", ""};
        buildTrie(words);
        for (var word : words) {
            Assertions.assertTrue(testTrie.remove(word));
        }

        Assertions.assertEquals(0, testTrie.size());

    }
    @Test
    public void testSize() {
        buildTrie(basicWords);
        Assertions.assertEquals(basicWords.length, testTrie.size());
        testTrie.add("Word");
        Assertions.assertEquals(basicWords.length + 1, testTrie.size());
        testTrie.add("Another");
        Assertions.assertEquals(basicWords.length + 2, testTrie.size());
        testTrie.remove("Another");
        Assertions.assertEquals(basicWords.length + 1, testTrie.size());
    }

    @Test
    public void testHowManyStartsWithPrefix() {
        buildTrie(basicWords);

        Assertions.assertEquals( 5, testTrie.howManyStartsWithPrefix("H"), "H");
        Assertions.assertEquals( 4, testTrie.howManyStartsWithPrefix("He"), "He");
        Assertions.assertEquals( 2, testTrie.howManyStartsWithPrefix("Hel"), "Hel");
        Assertions.assertEquals( 2, testTrie.howManyStartsWithPrefix("Hell"), "Hell");
        Assertions.assertEquals( 1, testTrie.howManyStartsWithPrefix("Hello"), "Hello");
    }

    @Test
    public void testNextString() {
        buildTrie(basicWords);
        Assertions.assertEquals("", testTrie.nextString("", 0), "empty string, k = 0");
        Assertions.assertEquals("Hello", testTrie.nextString("H", 3), "H, k = 3");
        Assertions.assertEquals("H", testTrie.nextString("H", 0), "H, k = 0");
        Assertions.assertEquals("Hell", testTrie.nextString("H", 2), "H, k = 2");

        Assertions.assertEquals("Hell", testTrie.nextString("Ad", 3), "A, k = 2");
        Assertions.assertEquals("Hell", testTrie.nextString("Ad", 3), "A, k = 2, second test");
        Assertions.assertNull(testTrie.nextString("Hello", 10), "Hello, k = 10");

        Assertions.assertEquals("Hell", testTrie.nextString("Haa", 2), "A, k = 2");
        Assertions.assertEquals("Hell", testTrie.nextString("Haa", 2), "A, k = 2, second");

    }
    @Test
    public void testBigTrie() {
        HashSet<String> set = new HashSet<>();
        for (var word : randomWords) {
            Assertions.assertEquals(set.add(word), testTrie.add(word), "Tried to add word: " + word);
        }

        for (var word : set) {
            Assertions.assertTrue(testTrie.contains(word), "Check whether word exist in trie: " + word);
        }

        Assertions.assertEquals(set.size(), testTrie.size());

        for (String randomWord : randomWords) {
            Assertions.assertEquals(set.remove(randomWord), testTrie.remove(randomWord), "Tried to remove word: " + randomWord);
            Assertions.assertEquals(set.size(), testTrie.size());
        }

        for (var word : randomWords) {
            Assertions.assertEquals(set.add(word), testTrie.add(word), "Tried to add word second time: " + word);
        }

        Assertions.assertEquals(set.size(), testTrie.size());

    }
}