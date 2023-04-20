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

def traverse(curr=None, depth=0):
    if curr is None:
        curr = _root
        depth = -1
    else:
        yield curr, depth
    for child in curr.children:
        yield from traverse(child, depth+1)

def print_tree():
    for curr, depth in traverse():
        indent = '    ' * depth
        prefix = '|-- ' if depth > 0 else ''
        print(indent + prefix + curr.name)

def _repl():
    while True:
        print(">>> ", end="")
        _s = input()
        if _s in ("exit()", ""):
            break
        print(eval(_s))

