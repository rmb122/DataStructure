from glob import glob
from PIL import Image
import numpy
import random
from torch import FloatTensor, LongTensor


def get_car_mapping():
    mapping = {}
    with open('HFUT-VL-dataset/HFUT-VL1/CarNames.txt') as f:
        c = f.read()
        lines = [i.strip() for i in c.split('\n') if i.strip() != '']
        for i in lines:
            name, class_id = i.split()
            mapping[class_id] = name

    return mapping


def train_data_generator(image_size, batch_size=16):
    all_data = glob('HFUT-VL-dataset/HFUT-VL1/HFUT-VL1_1/*') + glob('HFUT-VL-dataset/HFUT-VL1/HFUT-VL1_2/*')
    random.shuffle(all_data)

    for cnt in range(0, len(all_data), batch_size):
        img = []
        img_label = []

        for data in all_data[cnt:cnt + batch_size]:
            temp_img = Image.open(data).resize((image_size, image_size)).convert('L')
            temp_img = numpy.asarray(temp_img).reshape((1, image_size, image_size))  # one sample + one channel + 32x32 image
            img.append(temp_img)

            label_id = int(data.split('/')[-1].split('_')[0]) - 1
            img_label.append(label_id)
        yield FloatTensor(numpy.stack(img)),  LongTensor(img_label)
