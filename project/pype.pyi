def destroy():
    pass
class Node(object):
    @property
    def height(self):
        pass
    @property
    def parent(self):
        pass
    on_update = ...
    def stop_coroutine():
        pass
    _draw: ellipsis = ...
    @property
    def global_angle(self):
        pass
    def __getitem__():
        pass
    @property
    def width(self):
        pass
    @property
    def children(self):
        pass
    @property
    def visible(self):
        pass
    @property
    def child_count(self):
        """获取子节点数量"""
        pass
    on_ready = ...
    def _px_obj_init():
        pass
    def _update():
        pass
    def start_coroutine():
        pass
    @property
    def position(self):
        pass
    @property
    def global_position(self):
        """获取全局坐标"""
        pass
    on_destroy = ...
    @property
    def enabled(self):
        pass
    def stop_all_coroutines():
        pass
    def __init__():
        pass
    @property
    def global_scale(self):
        pass

class Signal(object):
    def disconnect_all():
        pass
    def connect():
        pass
    def disconnect():
        pass
    def emit():
        pass
    def __init__():
        pass

def _PX_ObjectDelete():
    pass
class Image(Node):
    @property
    def texture(self):
        pass
    def _px_obj_init():
        pass
    def set_native_size():
        pass

def _print_tree():
    pass
class Input(object):
    def __new__():
        pass
    def get_key():
        pass

class Time(object):
    delta_time: float = ...

def _PX_TextureRenderEx():
    pass
class Sprite2D(Node):
    def _draw():
        pass
    def __init__():
        pass

_resources: dict = ...
class Button(Node):
    def on_click():
        pass
    @property
    def text(self):
        pass
    def _px_obj_init():
        pass
    def __init__():
        pass

def traverse():
    pass
class Texture2D(object):
    def __new__():
        pass
    def __repr__():
        pass
    @property
    def height(self):
        pass
    @property
    def limit_right(self):
        pass
    @property
    def limit_left(self):
        pass
    @property
    def limit_bottom(self):
        pass
    @property
    def width(self):
        pass
    @property
    def limit_top(self):
        pass

def WaitForSeconds():
    pass
def memory_usage():
    pass
def load():
    pass
def WaitForEndOfFrame():
    pass
class _GCProxy(object):
    def __new__():
        pass

__name__: str = ...
class Vector2(object):
    def __new__():
        pass
    def __repr__():
        pass
    def __mul__():
        pass
    def __truediv__():
        pass
    def __add__():
        pass
    def __eq__():
        pass
    def __sub__():
        pass
    def __ne__():
        pass

def _PX_LoadTextureFromFile():
    pass
def _repl():
    pass
class Label(Node):
    @property
    def text(self):
        pass
    def _px_obj_init():
        pass

_root: Node = ...
_gc_proxy: _GCProxy = ...
def _PX_WorldObjectXYtoScreenXY():
    pass