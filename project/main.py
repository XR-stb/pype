from PainterEngine import _repl, _root, _print_tree
from PainterEngine import *

class LogoSprite(Sprite2D):
    def on_ready(self):
        self.texture = load("assets/logo.png")
        self.scale = 3

    def on_update(self):
        # 每秒向右下移动10个单位
        self.position += Vector2(10, 10) * Time.delta_time
        # 每秒旋转半圈
        self.angle += 180 * Time.delta_time

        if Input.get_key('R'):
            _repl()

LogoSprite()

