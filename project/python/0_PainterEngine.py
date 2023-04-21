def destroy(obj):
    assert isinstance(obj, Node)
    if obj.on_destroy is not None:
        obj.on_destroy(obj)
    _PX_ObjectDelete(obj)
    obj._px_obj = None

def traverse(curr=None, depth=0):
    if curr is None:
        curr = _root
        depth = -1
    else:
        yield curr, depth
    for child in curr.children:
        yield from traverse(child, depth+1)

def _print_tree():
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
        ret = eval(s)
        if ret is not None:
            print(ret)

