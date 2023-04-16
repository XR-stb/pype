from PainterEngine import _repl, _root
from PainterEngine import *

class Test(PainterBehaviour):
	def Awake(self):
		print("awake")

	def Update(self):
		print("update", Time.deltaTime)
		pass

g = GameObject("测试")
test = g.AddComponent(Test)
print(g.components)

print(g.GetComponent(Test))

GameObject("123"), GameObject("456"), GameObject("789")
print_tree()

path = "/mnt/c/Users/bluel/Desktop/test.bmp"
p = Resources.Load(path)
print(p)
_repl()

print(_root.GetChildCount(), _root.GetChild(0), _root.GetChild(1))