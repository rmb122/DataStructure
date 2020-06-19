import numpy
import torch
from PIL import Image
import sys

from util import get_car_mapping

model = torch.load('lenet.model')
mapping = get_car_mapping()

img_obj = Image.open(sys.argv[1]).resize((64, 64)).convert('L')
img_array = numpy.asarray(img_obj).reshape((1, 1, 64, 64))
input = torch.FloatTensor(img_array)

with torch.no_grad():
    out = model(input)
    label = torch.argmax(out, dim=1).item()
    predict = mapping[str(label + 1)]
    print('这个车标是：%s' % predict)

