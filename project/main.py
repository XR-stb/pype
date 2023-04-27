from pype import _repl, _root, _print_tree
from pype import *

class LogoSprite(Sprite):
    def on_ready(self):
        self.texture = load("assets/logo.png")
        self.scale = 3

    def on_update(self):
        # 每秒向右下移动10个单位
        self.position += Vector2(10, 10) * Time.delta_time
        # 每秒旋转半圈
        self.angle += 180 * Time.delta_time
        # memory_usage()

LogoSprite()

for i in range(4):
    b = Button()
    b.text = f"Hello! {i}"
    b.width = 200
    b.height = 50
    b.position = Vector2(100, 100+i*60)
    b.clicked.connect(lambda _: print(_))

tex = load("assets/logo.png")
print(tex)

img = Image()
img.texture = tex
img.set_native_size()