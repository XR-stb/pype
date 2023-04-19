from PainterEngine import _repl, _root
from PainterEngine import *

class Test(PainterBehaviour):
    def Awake(self):
        self.StartCoroutine(self.coroutine())

    def coroutine(self):
        yield from WaitForSeconds(3.0)
        _root.position += Vector2(0, 32)
        print("Hello World!")

    def Update(self):
        # print("update", Time.deltaTime)
        delta = Vector2(10, 0) * Time.deltaTime
        self.gameObject.position += delta

        if Input.GetKey("R"):
            _repl()

g = GameObject("测试")
g.AddComponent(Test)

# 添加一个精灵渲染器
sp_renderer = g.AddComponent(SpriteRenderer)

# 加载纹理资源
path = "assets/ibb-and-obb.bmp"

# 设置渲染器的纹理
sp_renderer.sprite = Resources.Load(path)


# print(g.GetComponent(Test))
# GameObject("123"), GameObject("456"), GameObject("789")
# print_tree()

# print(_root.GetChildCount(), _root.GetChild(0), _root.GetChild(1))
# _repl()