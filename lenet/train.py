import torch.nn as nn
from torch import argmax, save
import torch.optim as optim
from LeNet import LeNet
from AlexNet import AlexNet
from util import get_car_mapping, train_data_generator


n_class = len(get_car_mapping())
lenet = LeNet(n_class)

optimizer = optim.Adam(params=lenet.parameters(), lr=0.001)
loss_function = nn.CrossEntropyLoss()

cnt = 0

for _ in range(16):  # epoch = 16
    for input, label in train_data_generator(64):
        cnt += 1
        optimizer.zero_grad()

        out = lenet(input)
        t = argmax(out, dim=1)
        loss = loss_function(out, label)
        if cnt % 100 == 0:
            print('loss after 100 batches: {}, total: {}'.format(loss, cnt))
        loss.backward()
        optimizer.step()

save(lenet, 'lenet.model')
