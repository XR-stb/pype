def GameObject::Find(name: str):
    assert type(name) is str
    for curr, depth in traverse():
        if curr.name == name:
            return curr
    return None

def GameObject::__init__(self, name=None):
    self.name = name or "未命名物体"
    self.components = []
    self.renderer = None

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