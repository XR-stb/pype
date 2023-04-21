from PainterEngine import _repl, _root
from PainterEngine import *

class Test(PainterBehaviour):
    def Awake(self):
        # 初始放大3倍
        self.gameObject.scale = 3

    def Update(self):
        # 每秒向右移动10个单位
        self.gameObject.position += Vector2(10, 0) * Time.deltaTime
        # 每秒旋转半圈
        self.gameObject.angle += 180 * Time.deltaTime

g = GameObject("g#001")
g.AddComponent(Test)

# 添加一个精灵渲染器
sp_renderer = g.AddComponent(SpriteRenderer)

# 加载纹理资源
path = "assets/logo.png"

# 设置渲染器的纹理
sp_renderer.sprite = Resources.Load(path)




# f = GameObject("f#002")
# b = GameObject("b#003")
# c = GameObject("c#004")
# d = GameObject("d#005")
# e = GameObject("e#006")
# b.parent = g
# d.parent = b
# e.parent = c
# c.SetParent(b)
# print_tree()

# c.parent = None
# print('---------------------')
# print_tree()

# g.parent = e
# print('---------------------')
# print_tree()