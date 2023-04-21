from collections import deque

def destroy(obj):
    assert isinstance(obj, Node)
    if obj.OnDestroy is not None:
        obj.OnDestroy(obj)
    obj._px_obj = None
    _PX_ObjectDelete(obj)

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
        s = input()
        if s in ("exit()", ""):
            break
        exec(s)

