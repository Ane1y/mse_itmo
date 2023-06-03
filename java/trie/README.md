# Trie

## Overview
A Trie is an efficient data structure used for storing and searching strings. It provides fast operations for adding, searching, and removing words, as well as finding the number of words starting with a given prefix.

## How to Use
3. Add words to the Trie using the `add` method: `trie.add("word");`
4. Search for words in the Trie using the `contains` method: `boolean contains = trie.contains("word");`
5. Remove words from the Trie using the `remove` method: `boolean removed = trie.remove("word");`
6. Get the size of the Trie using the `size` method: `int size = trie.size();`
7. Find the number of words starting with a prefix using the `howManyStartsWithPrefix` method: `int count = trie.howManyStartsWithPrefix("pre");`
8. Get the next string in lexicographical order after a given string using the `nextString` method: `String next = trie.nextString("word", 5);`

## Performance
The Trie data structure provides efficient operations:
- Insertion and removal have a time complexity of O(|W|), where |W| is the length of the word.
- Searching for a word or checking if it exists has a time complexity of O(|W|).
- Finding the number of words starting with a prefix has a time complexity of O(|P|), where |P| is the length of the prefix.
- Getting the next string in lexicographical order has a time complexity of O(K), where K is the number of strings in the Trie.

