from PainterEngine import _repl, _root
from PainterEngine import *

class LogoSprite(Sprite2D):
    def __init__(self):
        tex = Resources.Load("assets/logo.png")
        super(LogoSprite, self).__init__(tex)
        self.scale = 3

    def OnUpdate(self):
        # 每秒向右移动10个单位
        self.position += Vector2(10, 0) * Time.deltaTime
        # 每秒旋转半圈
        self.angle += 180 * Time.deltaTime

sprite = LogoSprite()

