from PainterEngine import _repl, _root
from PainterEngine import *

class Test(PainterBehaviour):
	def Awake(self):
		print("awake")

	def Update(self):
		# print("update", Time.deltaTime)
		pass

g = GameObject("测试")

# 添加一个精灵渲染器
sp_renderer = g.AddComponent(SpriteRenderer)

# 加载纹理资源
path = "assets/ibb-and-obb.bmp"

# 设置渲染器的纹理
sp_renderer.sprite = Resources.Load(path)

_repl()



# print(g.GetComponent(Test))
# GameObject("123"), GameObject("456"), GameObject("789")
# print_tree()

# print(_root.GetChildCount(), _root.GetChild(0), _root.GetChild(1))
# _repl()