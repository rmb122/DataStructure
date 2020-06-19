import json

def load_dict(filename):
    word_dict = json.load(open(filename))
    return word_dict

def fmm_cut(word_dict, sentence):
    words = []
    sentence_len = len(sentence)
    curr_word_start = 0
    curr_word_end = sentence_len

    while curr_word_start != sentence_len:
        curr_word = sentence[curr_word_start:curr_word_end]
        if curr_word in word_dict:
            words.append(curr_word)
            curr_word_start = curr_word_end
            curr_word_end = sentence_len
        else:
            curr_word_end -= 1
            if curr_word_start == curr_word_end:
                words.append(sentence[curr_word_start])
                curr_word_start += 1
                curr_word_end = sentence_len
    return words

def bmm_cut(word_dict, sentence):
    words = []
    sentence_len = len(sentence)
    curr_word_start = 0
    curr_word_end = sentence_len

    while curr_word_end != 0:
        curr_word = sentence[curr_word_start:curr_word_end]
        if curr_word in word_dict:
            words.append(curr_word)
            curr_word_end = curr_word_start
            curr_word_start = 0
        else:
            curr_word_start += 1
            if curr_word_start == curr_word_end - 1:
                words.append(sentence[curr_word_start])
                curr_word_end -= 1
                curr_word_start = 0
    words.reverse()
    return words

word_dict = load_dict('dump_words.json')
'''target = '他们是研究生物化学的'


words = fmm_cut(word_dict, target)
print(words)

words = bmm_cut(word_dict, target)
print(words)
'''