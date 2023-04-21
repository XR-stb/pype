class Resources:
    # 缓存已经加载过的资源
    cache = {}

def Resources::Load(path: str):
    assert type(path) is str
    if path in Resources.cache:
        return Resources.cache[path]
    if path.endswith(".bmp") or path.endswith(".jpg") or path.endswith(".png") or path.endswith(".traw"):
        res = _PX_LoadTextureFromFile(path)
    else:
        raise ValueError("未知的资源类型")
    Resources.cache[path] = res
    return res