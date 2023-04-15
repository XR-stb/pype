from PainterEngine import GameObject, Destroy, Component, PainterBehaviour, Time, _root, _print_tree

class Test(PainterBehaviour):
	def Awake(self):
		print("awake")

	def Update(self):
		# print("update", Time.deltaTime)
		pass

g = GameObject()
test = g.AddComponent(Test)
print(
	list(g.components())
)

GameObject("123"), GameObject("456"), GameObject("789")
_print_tree()

print(_root.GetChildCount(), _root.GetChild(0), _root.GetChild(1))