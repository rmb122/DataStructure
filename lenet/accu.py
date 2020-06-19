import numpy
import torch
from PIL import Image
from glob import glob
import os

from util import get_car_mapping

model = torch.load('lenet.model')
mapping = get_car_mapping()
all_data = glob('HFUT-VL-dataset/HFUT-VL1/HFUT-VL1_1/*') + glob('HFUT-VL-dataset/HFUT-VL1/HFUT-VL1_2/*')
#all_data = glob('HFUT-VL-dataset/VL1-TEST/*')
right_cnt = 0

for i in all_data:
    img_obj = Image.open(i).resize((64, 64)).convert('L')
    img_array = numpy.asarray(img_obj).reshape((1, 1, 64, 64))
    input = torch.FloatTensor(img_array)

    with torch.no_grad():
        out = model(input)
        label = torch.argmax(out, dim=1).item()
        predict = mapping[str(label + 1)]
        truth = mapping[i.split('/')[-1].split('_')[0]]

        if predict == truth:
            right_cnt += 1
        else:
            print('这个车标是：%s' % predict, f'实际上是: %s, path = file://{os.getcwd()}/%s' % (truth, i))

print(right_cnt / len(all_data))
