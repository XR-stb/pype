_resources = {}     # 资源缓存

def load(path: str):
    assert type(path) is str
    if path in _resources:
        return _resources[path]
    if path.endswith(".bmp") or path.endswith(".jpg") or path.endswith(".png") or path.endswith(".traw"):
        res = _PX_LoadTextureFromFile(path)
    else:
        raise ValueError("未知的资源类型")
    _resources[path] = res
    return res