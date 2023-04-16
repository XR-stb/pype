from PainterEngine import GameObject, Destroy, Component, PainterBehaviour, Time, _root, _print_tree, _repl

class Test(PainterBehaviour):
	def Awake(self):
		print("awake")

	def Update(self):
		# print("update", Time.deltaTime)
		pass

g = GameObject("测试")
test = g.AddComponent(Test)
print(g.components)

print(g.GetComponent(Test))

GameObject("123"), GameObject("456"), GameObject("789")
_print_tree()

_repl()

print(_root.GetChildCount(), _root.GetChild(0), _root.GetChild(1))