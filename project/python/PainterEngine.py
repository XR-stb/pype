from collections import deque

class Component:
    Awake = None
    Update = None

    def __init__(self, gameObject):
        self.gameObject = gameObject

    def GetComponent(self, tp):
        return self.gameObject.GetComponent(tp)

    def GetComponentInChildren(self, tp):
        return self.gameObject.GetComponentInChildren(tp)

class PainterBehaviour(Component):
    pass

class Time:
    deltaTime = 0

################# 内部函数 #################

def traverse(go=None):
    q = deque()
    q.append((go or _root, -1))
    while len(q) > 0:
        curr, depth = q.popleft()
        if depth > -1:
            yield curr, depth
        for child in curr.children:
            q.append((child, depth+1))

def print_tree(go=None):
    for curr, depth in traverse(go):
        print('    '*depth + curr.name)

def _repl():
    while True:
        print(">>> ", end="")
        _s = input()
        if _s == "exit()":
            break
        print(eval(_s))

################# GameObject #################

def GameObject::Find(name: str):
    assert type(name) is str
    for curr, depth in traverse():
        if curr.name == name:
            return curr
    return None

def GameObject::__init__(self, name=None):
    self.name = name or "未命名物体"
    self.components = []

def GameObject::__repr__(self):
    return "GameObject(" + repr(self.name) + ")"

def GameObject::_update(self):
    for cpnt in self.components:
        if cpnt.Update is not None:
            cpnt.Update()

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
    if cpnt.Awake is not None:
        cpnt.Awake()
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

################# Vector3 #################

class Vector3:
    def __init__(self, x, y, z):
        assert type(x) is float
        assert type(y) is float
        assert type(z) is float
        self.x = x
        self.y = y
        self.z = z

    def __repr__(self):
        return "Vector3(" + repr(self.x) + ", " + repr(self.y) + ", " + repr(self.z) + ")"

    def __add__(self, other):
        return Vector3(self.x + other.x, self.y + other.y, self.z + other.z)

    def __sub__(self, other):
        return Vector3(self.x - other.x, self.y - other.y, self.z - other.z)

    def __mul__(self, other):
        return Vector3(self.x * other, self.y * other, self.z * other)

    def __truediv__(self, other):
        return Vector3(self.x / other, self.y / other, self.z / other)

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y and self.z == other.z

    def __ne__(self, other):
        return self.x != other.x or self.y != other.y or self.z != other.z