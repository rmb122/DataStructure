import pyautogui
import typing
import time
from PIL import Image, ImageDraw
import cv2
import numpy


def get_curr_position(stop_count=2):
    curr_count = 0
    curr_x = 0
    curr_y = 0

    while curr_count < stop_count:
        pos = pyautogui.position()
        if curr_x != pos.x or curr_y != pos.y:
            curr_x = pos.x
            curr_y = pos.y
            curr_count = 0
        else:
            curr_count += 1
        time.sleep(1)
    return curr_x, curr_y


def get_window_position():
    capture_succ = False
    while not capture_succ:
        print('获取窗口左上角点...')
        upper_left = get_curr_position()

        print('获取窗口右下角点...')
        lower_right = get_curr_position()

        img: Image.Image = pyautogui.screenshot()
        img = numpy.array(img)

        cv2.rectangle(img, upper_left, lower_right, (255, 255, 255))

        img = Image.fromarray(img)
        img.thumbnail((img.size[0] // 2, img.size[1] // 2))
        img.show()

        ret = input('捕获是否正确? [y]/n ')
        if ret.strip().lower() == 'y' or ret.strip() == '':
            capture_succ = True
        else:
            print('重试中...')

    return upper_left, lower_right


def match_img(image, template):
    ret = cv2.matchTemplate(image, template, cv2.TM_CCOEFF_NORMED)
    ret = cv2.minMaxLoc(ret)

    if ret[1] > 0.5:
        return ret[3]
    else:
        return None


class ArkHelperStatus():
    status_list = ['LEVEL_SELECT', 'CHAR_SELECT', 'LEVEL_RUNNING']
    status_before_click_func = [lambda:None, lambda:None, lambda:time.sleep(3)]
    curr_index = 0

    def get_curr_status(self):
        return self.status_list[self.curr_index]

    def next_status(self):
        self.curr_index += 1
        self.curr_index %= len(self.status_list)
    
    def status_before_click(self):
        self.status_before_click_func[self.curr_index]()


class ArkHelper():
    status: ArkHelperStatus
    window_position: typing.Tuple[typing.Tuple[int, int], typing.Tuple[int, int]]

    def __init__(self, window_position):
        self.window_position = window_position
        self.status = ArkHelperStatus()

    def run(self):
        while True:
            img: Image.Image = pyautogui.screenshot()
            img = img.crop(self.window_position[0] + self.window_position[1])
            img = cv2.cvtColor(numpy.array(img), cv2.COLOR_BGR2RGB)

            template = cv2.imread(self.status.get_curr_status() + '.png')
            top_left_position = match_img(img, template)
            
            if top_left_position is not None:
                self.status.status_before_click()
                height, width = template.shape[:2]
                center_position = top_left_position[0] + width // 2, top_left_position[1] + height // 2
                pyautogui.click(self.window_position[0][0] + center_position[0], self.window_position[0][1] + center_position[1])
                pyautogui.moveTo(self.window_position[0][0], self.window_position[0][1])
                self.status.next_status()
                print('找到目标')
            else:
                print('未找到匹配的图片')
  
            time.sleep(1)

a = ArkHelper(get_window_position())
a.run()
