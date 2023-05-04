_resources = {}     # 资源缓存

def load(path: str):
    assert type(path) is str
    if path in _resources:
        return _resources[path]
    data = _platform_read_bytes(path)
    if path.endswith(".bmp") or path.endswith(".jpg") or path.endswith(".png") or path.endswith(".traw"):
        res = _PX_TextureCreateFromMemory(data)
    else:
        raise ValueError("未知的资源类型: " + path)
    _resources[path] = res
    return res

def load_dir(path: dir):
    assert type(path) is str
    return [
        load(os.path.join(path, file))
        for file in sorted(_platform_list_dir(path))
    ]