from glob import glob
from os import rename

all_data = glob('HFUT-VL-dataset/HFUT-VL1/HFUT-VL1_1/*') + glob('HFUT-VL-dataset/HFUT-VL1/HFUT-VL1_2/*')
for i in all_data:
    if int(i.split('_')[-1].split('.')[0]) > 180:
        rename(i, f'/home/rmb122/temp/lenet/HFUT-VL-dataset/VL1-TEST/{i.split("/")[-1]}')
