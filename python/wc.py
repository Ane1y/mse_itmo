# write your code here
# количество строк
# количество слов
# количество байтов
import os
import sys

filenames = sys.argv[1:]

s_lines = 0
s_words = 0
s_bytes = 0
max_size = 0
wc_stats = []
for i in range(len(filenames)):
    with open(filenames[i], "r+") as f:
        data = f.read()
        lines = data.count("\n")
        words = len(data.split())
        bytes = os.stat(filenames[i]).st_size

        s_lines += lines
        s_words += words
        s_bytes += bytes

        wc_stats.append((lines, words, bytes,))
wc_stats.append((s_lines, s_words, s_bytes))
max_size = max(len(str(s_lines)), len(str(s_words)), len(str(s_bytes)))
for i in range(len(wc_stats)):
    for stat in wc_stats[i]:
        print(f'{stat:>{max_size}}', end = ' ')
    if i == len(wc_stats) - 1:
        print('total')
    else:
        print(filenames[i])
