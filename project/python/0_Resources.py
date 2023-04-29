import os

_resources = {}     # 资源缓存

def load(path: str):
    assert type(path) is str
    if path in _resources:
        return _resources[path]
    if path.endswith(".bmp") or path.endswith(".jpg") or path.endswith(".png") or path.endswith(".traw"):
        res = _PX_LoadTextureFromFile(path)
    else:
        return None
    _resources[path] = res
    return res

def load_frame_animation(path: str, speed=60, loop=True):
    anim = FrameAnimation()
    anim.speed = speed
    anim.loop = loop
    # 按文件名的字典序确定帧的顺序
    for file in sorted(os.listdir(path)):
        res = load(os.path.join(path, file))
        if type(res) is Texture2D:
            anim.frames.append(res)
    return anim
