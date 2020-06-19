from flask import Flask, request
from expr2 import get_poem, word_dict_one, word_dict_two, patterns
from expr4 import bmm_cut, fmm_cut, word_dict

app = Flask(__name__)

@app.route("/expr1", methods=['POST'])
def expr1():
    brand_name = request.json['brand_name']
    return {"poem": get_poem(word_dict_one, word_dict_two, patterns[brand_name])}

@app.route("/expr2", methods=['POST'])
def expr2():
    sentence = request.json['sentence']
    return {"fmm": fmm_cut(word_dict, sentence), "bmm": bmm_cut(word_dict, sentence)}
