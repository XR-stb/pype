from collections import deque

def Destroy(obj):
    if type(obj) is GameObject:
        for cpnt in obj.components:
            if cpnt.OnDestroy is not None:
                cpnt.OnDestroy()
        _PX_ObjectDelete(obj)
        return
    if isinstance(obj, Component):
        if obj.OnDestroy is not None:
            obj.OnDestroy()
        obj.gameObject.components.remove(obj)
        return
    raise ValueError("Destroy()的参数必须是GameObject或Component，你传入了" + type(obj).__name__)
        

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
        if _s in ("exit()", ""):
            break
        print(eval(_s))

