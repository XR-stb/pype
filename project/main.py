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

g = GameObject("g#001")
f = GameObject("f#002")
g.AddComponent(Test)

b = GameObject("b#003")
c = GameObject("c#004")
d = GameObject("d#005")
e = GameObject("e#006")
b.parent = g
d.parent = b
e.parent = c
c.SetParent(b)
print_tree()

c.parent = None
print('---------------------')
print_tree()

g.parent = e
print('---------------------')
print_tree()

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