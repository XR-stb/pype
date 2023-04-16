from collections import deque

class Component:
    def __init__(self, gameObject):
        self.gameObject = gameObject
        self.Awake()

    def Awake(self):
        pass

    def Update(self):
        pass

    def GetComponent(self, tp):
        return self.gameObject.GetComponent(tp)

    def GetComponentInChildren(self, tp):
        return self.gameObject.GetComponentInChildren(tp)

class PainterBehaviour(Component):
    pass

class Time:
    deltaTime = 0

################# 内部函数 #################

def _update(deltaTime: float):
    # 先设置Time.deltaTime
    Time.deltaTime = deltaTime

    # 层序遍历对象树
    q = deque()
    q.append(_root)
    while len(q) > 0:
        curr = q.popleft()
        # 遍历GameObject的组件，调用Update
        for cpnt in curr.components:
            cpnt.Update()
        for child in curr.children:
            q.append(child)

def _print_tree(go=None):
    q = deque()
    q.append((go or _root, -1))
    while len(q) > 0:
        curr, depth = q.popleft()
        if depth > -1:
            print('    '*depth + curr.name)
        for child in curr.children:
            q.append((child, depth+1))

def _repl():
    while True:
        print(">>> ", end="")
        _s = input()
        if _s == "exit()":
            break
        print(eval(_s))

################# GameObject #################

def GameObject::__init__(self, name=None):
    self.name = name or "未命名物体"
    self.components = []

def GameObject::__repr__(self):
    return "GameObject(" + repr(self.name) + ")"

def GameObject::GetChild(self, index: int):
    i = 0
    for child in self.children:
        if i == index:
            return child
        i += 1
    raise IndexError("index out of range")

def GameObject::GetChildCount(self):
    i = 0
    for child in self.children:
        i += 1
    return i

def GameObject::AddComponent(self, tp):
    cpnt = tp(self)
    assert isinstance(cpnt, Component)
    self.components.append(cpnt)
    return cpnt

def GameObject::GetComponent(self, tp):
    for cpnt in self.components:
        if isinstance(cpnt, tp):
            return cpnt
    return None

def GameObject::GetComponentInChildren(self, tp):
    for cpnt in self.components:
        if isinstance(cpnt, tp):
            return cpnt
    for child in self.children:
        cpnt = child.GetComponentInChildren(tp)
        if cpnt is not None:
            return cpnt
    return None