from PainterEngine import _repl, _root
from PainterEngine import *

class LogoSprite(Sprite2D):
    def OnReady(self):
        self.scale = 3

    def OnUpdate(self):
        # 每秒向右下移动10个单位
        self.position += Vector2(10, 10) * Time.deltaTime
        # 每秒旋转半圈
        self.angle += 180 * Time.deltaTime

        if Input.GetKey('R'):
            _repl()

s = LogoSprite()
s.texture = Resources.Load("assets/logo.png")

