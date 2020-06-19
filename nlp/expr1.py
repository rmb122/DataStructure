import re
import json

resource = open('Ci.txt', 'r')
content = resource.read()
resource.close()

content = content.replace(' ', '').replace('\\', ' ')
sentences = re.split('\n|。|，|、', content)

count_dict_two_word = dict()
count_dict_one_word = dict()

def split_as_n(sentence, n):
    nwords = []
    for j in range(n):
        for i in range(j, len(sentence) - j - ((len(sentence) - j) % n), n):
            nwords.append(sentence[i:i + n])
    return nwords

def dict_add(d, key):
    if key in d:
        d[key] += 1
    else:
        d[key] = 1

def sort_as_value(d):
    return sorted(d.items(), key=lambda x: x[1])

def dump_dict(d, fname):
    f = open(fname, 'w')
    json.dump(d, f, ensure_ascii=False)
    f.close()

for sentence in sentences:
    if sentence and len(sentence) > 5:
        words_2 = split_as_n(sentence, 2)
        for word in words_2:
            dict_add(count_dict_two_word, word)

        words_1 = split_as_n(sentence, 1)
        for word in words_1:
            dict_add(count_dict_one_word, word)

count_dict_one_word = sort_as_value(count_dict_one_word)
dump_dict(count_dict_one_word, 'dump_one.json')

count_dict_two_word = sort_as_value(count_dict_two_word)
dump_dict(count_dict_two_word, 'dump_two.json')
