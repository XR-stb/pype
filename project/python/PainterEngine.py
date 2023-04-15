from collections import deque

class Component:
    def __init__(self, gameObject):
        self.gameObject = gameObject
        self.Awake()

    def Awake(self):
        pass

    def Update(self):
        pass

    def GetComponent(self, type):
        return self.gameObject.GetComponent(type)

    def GetComponentInChildren(self, type):
        return self.gameObject.GetComponentInChildren(type)

class PainterBehaviour(Component):
    pass

class Time:
    deltaTime = 0

def _update(deltaTime: float):
    # 先设置Time.deltaTime
    Time.deltaTime = deltaTime

    # 层序遍历对象树
    q = deque()
    q.append(_root)
    while len(q) > 0:
        curr = q.popleft()
        curr._update()
        # 遍历GameObject的组件，调用Update
        for cpnt in curr.components():
            cpnt.Update()
        for child in curr.children():
            q.append(child)

def _print_tree(go=None):
    q = deque()
    q.append((go or _root, -1))
    while len(q) > 0:
        curr, depth = q.popleft()
        if depth > -1:
            print('    '*depth + curr.name)
        for child in curr.children():
            q.append((child, depth+1))


GameObject.GetChildCount = lambda self: len(self.children())
GameObject.GetChild = lambda self, index: self.children()[index]
GameObject.__repr__ = lambda self: "GameObject(" + repr(self.name) + ")"