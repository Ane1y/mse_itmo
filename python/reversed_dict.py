import sys
import string
from collections import defaultdict
from solution_01 import remove_adjacent

def reverse_dict(all_data):
    strings = []
    i = 0
    dict = defaultdict(list)
    for filename in all_data:
        with open(filename) as file:
            for s in file.readlines():
                strings.append(
                    s.translate(str.maketrans("", "", string.punctuation)).split()
                )
                for word in strings[i][1:]:
                    if strings[i][0] not in dict[word]:
                        dict[word].append(strings[i][0])
                i += 1

            for k, values in sorted(dict.items()):
                print(k, "- ", end="")
                for value in values:
                    print(value, end="")
                    if value != values[-1]:
                        print(", ", end="")
                    else:
                        print()


if __name__ == "__main__":
    strings = sys.argv[1:]
    reverse_dict(strings)
