import json
import random

def load_json(filename):
    with open(filename) as f:
        d = json.load(f)
    return d

def get_word_from_top(d, count):
    return random.choice(d[-count:])[0]

def get_next_word_from_top(word, d, count):
    tmp_word = [i for i in d if i[0][0] == word]
    return random.choice(tmp_word[-count:])[0]

def get_sentence(length, done, dtwo):
    sentence = ''
    last = get_word_from_top(done, 100)

    for _ in range(length):
        sentence += last
        word = get_next_word_from_top(sentence[-1], dtwo, 10)
        last = word[-1]
    return sentence

def get_poem(done, dtwo, pattern=[4, 7, 7, 5, 7, 7, 7, 5]):
    poem = []
    for i in pattern:
        poem.append(get_sentence(i, done, dtwo))
    poem = ", ".join(poem)
    poem += "."
    return poem

patterns = {
    "酒泉子": [4, 6, 3, 3, 3, 3],
    "鹧鸪天": [7, 7, 7, 7],
    "西江月": [6, 6, 7, 6],
    "苏幕遮": [3,3,4,5,7,4,5]
}

word_dict_one = load_json('dump_one.json')
word_dict_two = load_json('dump_two.json')

'''
poem = get_poem(word_dict_one, word_dict_two)
print(poem)
'''