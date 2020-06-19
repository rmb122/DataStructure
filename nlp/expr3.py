import json
from os.path import exists

if not exists('dump_words.json'):
    file = open('1998-01-2003版-带音.txt')
    content = file.read()
    file.close()

    content = content.split('\n')
    all_words = dict()

    for line in content:
        if line.strip() == "":
            continue

        curr_words = line.split(' ')
        for word in curr_words[1:]:
            if '/' in word:
                word = word.split('/')[0].replace('(', '').replace(')', '')
                if word in all_words:
                    all_words[word] += 1
                else:
                    all_words[word] = 1
    json.dump(all_words, open('dump_words.json', 'w'),  ensure_ascii=False)


while True:
    word = input("> ")
    if word in all_words:
        print(f"词为 [{word}], 词频为 [{all_words[word]}]")
    else:
        print('未检索到此词')

