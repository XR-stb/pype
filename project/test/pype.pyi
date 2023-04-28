"""pype核心模块"""

from typing import Any, Callable, Generator, Iterable, Tuple, List

class Vector2:
    def __init__(self, x: float, y: float):
        self.x = x
        self.y = y

    def __add__(self, rhs: 'Vector2') -> 'Vector2': ...
    def __sub__(self, rhs: 'Vector2') -> 'Vector2': ...
    def __mul__(self, rhs: float) -> 'Vector2': ...
    def __truediv__(self, rhs: float) -> 'Vector2': ...
    def __eq__(self, rhs: 'Vector2') -> bool: ...
    def __ne__(self, rhs: 'Vector2') -> bool: ...

class Texture2D:
    @property
    def width(self) -> int: ...
    @property
    def height(self) -> int: ...
    @property
    def limit_left(self) -> int: ...
    @property
    def limit_right(self) -> int: ...
    @property
    def limit_top(self) -> int: ...
    @property
    def limit_bottom(self) -> int: ...

class Signal:
    def connect(self, f: Callable) -> None:
        """连接一个信号处理函数"""
    def disconnect(self, f: Callable) -> None:
        """断开一个信号处理函数"""
    def emit(self, *args) -> None:
        """发射信号"""
    def disconnect_all(self) -> None:
        """断开所有信号处理函数"""

class Time:
    delta_time: float

class KeyCode:
    Undefined: int = 0
    Alpha0: int = 1
    Alpha1: int = 2
    Alpha2: int = 3
    Alpha3: int = 4
    Alpha4: int = 5
    Alpha5: int = 6
    Alpha6: int = 7
    Alpha7: int = 8
    Alpha8: int = 9
    Alpha9: int = 10
    A: int = 11
    B: int = 12
    C: int = 13
    D: int = 14
    E: int = 15
    F: int = 16
    G: int = 17
    H: int = 18
    I: int = 19
    J: int = 20
    K: int = 21
    L: int = 22
    M: int = 23
    N: int = 24
    O: int = 25
    P: int = 26
    Q: int = 27
    R: int = 28
    S: int = 29
    T: int = 30
    U: int = 31
    V: int = 32
    W: int = 33
    X: int = 34
    Y: int = 35
    Z: int = 36
    F1: int = 37
    F2: int = 38
    F3: int = 39
    F4: int = 40
    F5: int = 41
    F6: int = 42
    F7: int = 43
    F8: int = 44
    F9: int = 45
    F10: int = 46
    F11: int = 47
    F12: int = 48
    F13: int = 49
    F14: int = 50
    F15: int = 51
    F16: int = 52
    F17: int = 53
    F18: int = 54
    F19: int = 55
    F20: int = 56
    F21: int = 57
    F22: int = 58
    F23: int = 59
    F24: int = 60
    Keypad0: int = 61
    Keypad1: int = 62
    Keypad2: int = 63
    Keypad3: int = 64
    Keypad4: int = 65
    Keypad5: int = 66
    Keypad6: int = 67
    Keypad7: int = 68
    Keypad8: int = 69
    Keypad9: int = 70
    KeypadClear: int = 71
    KeypadDecimal: int = 72
    KeypadDivide: int = 73
    KeypadEnter: int = 74
    KeypadEquals: int = 75
    KeypadMinus: int = 76
    KeypadMultiply: int = 77
    KeypadPlus: int = 78
    Backslash: int = 79
    Comma: int = 80
    Equals: int = 81
    Grave: int = 82
    LeftBracket: int = 83
    Minus: int = 84
    Period: int = 85
    Quote: int = 86
    RightBracket: int = 87
    Semicolon: int = 88
    Slash: int = 89
    Meta: int = 90
    Ctrl: int = 91
    Function: int = 92
    Alt: int = 93
    Shift: int = 94
    RightMeta: int = 95
    RightCtrl: int = 96
    RightAlt: int = 97
    RightShift: int = 98
    CapsLock: int = 99
    NumLock: int = 100
    ScrollLock: int = 101
    DownArrow: int = 102
    LeftArrow: int = 103
    RightArrow: int = 104
    UpArrow: int = 105
    End: int = 106
    Home: int = 107
    PageDown: int = 108
    PageUp: int = 109
    ContextMenu: int = 110
    Backspace: int = 111
    Escape: int = 112
    Delete: int = 113
    Help: int = 114
    Mute: int = 115
    PrintScreen: int = 116
    Enter: int = 117
    Space: int = 118
    Tab: int = 119
    VolumeDown: int = 120
    VolumeUp: int = 121
    Pause: int = 122
    Insert: int = 123
    Sleep: int = 124

    @staticmethod
    def nameof(c: int) -> str:
        """获取键码的字符串名称"""

class Input:
    @staticmethod
    def get_key(c: int) -> bool:
        """检查当前帧某个按键是否处于按下状态"""
    @staticmethod
    def get_key_down(c: int) -> bool:
        """检查某个按键是否刚刚被按下"""
    @staticmethod
    def get_key_up(c: int) -> bool:
        """检查某个按键是否刚刚被释放"""

#####################################################

class Node:
    name: str           # 节点的名称
    parent: 'Node'      # 节点的父节点
    position: Vector2   # 节点的本地坐标
    angle: float        # 节点的本地角度
    scale: float        # 节点的本地缩放
    enabled: bool       # 节点是否激活，激活的节点会触发`on_update`事件
    visible: bool       # 节点是否可见，可见的节点会触发渲染事件
    width: int          # 节点的宽度
    height: int         # 节点的高度

    @property
    def global_position(self) -> Vector2:
        """获取全局坐标"""
    @property
    def global_angle(self) -> float:
        """获取全局角度"""
    @property
    def global_scale(self) -> float:
        """获取全局缩放"""
    @property
    def global_transform(self) -> Tuple[Vector2, float, float]:
        """获取全局变换，返回`(position, angle, scale)`"""

    @property
    def child_count(self) -> int:
        """获取子节点的数量"""
    @property
    def children(self) -> Tuple['Node']:
        """返回一个包含所有子节点的元组"""

    def on_ready(self) -> None:
        """[消息] 当节点被创建时调用"""
    def on_update(self) -> None:
        """[消息] 每帧调用一次"""
    def on_destroy(self) -> None:
        """[消息] 当节点被销毁时调用"""

    def start_coroutine(self, obj) -> Generator:
        """启动一个协程"""
    def stop_coroutine(self, obj) -> None:
        """停止一个协程"""
    def stop_all_coroutines(self) -> None:
        """停止所有协程"""

class Sprite2D(Node):
    texture: Texture2D   # 精灵的纹理

class Label(Node):
    text: str

class Button(Node):
    text: str
    clicked: Signal

class Image(Node):
    texture: Texture2D

    def set_native_size(self) -> None:
        """将节点的宽高设置为纹理的宽高"""

#####################################################

def WaitForEndOfFrame() -> Generator:
    """[协程] 等待当前帧结束"""
def WaitForSeconds(seconds: float) -> Generator:
    """[协程] 等待`seconds`秒"""

def load(path: str) -> Any:
    """加载一个资源并返回一个指针，此函数带有缓存，因此多次调用同一个资源不会重复加载"""
def traverse() -> Generator:
    """返回一个可以遍历对象树所有物体的迭代器，每个项`(Node, int)`表示物体及其深度（从0开始）"""
def destroy(obj: Node) -> None:
    """销毁一个节点"""
def memory_usage() -> None:
    """打印当前的内存使用信息"""
def _print_tree() -> None:
    """用于调试，向控制台打印对象树结构"""
def _repl() -> None:
    """打开一个简易REPL，`eval`用户输入的代码，并输出结果。可以使用`exit()`退出"""

#####################################################

class FrameAnimation:
    frames: List[Texture2D] # 帧列表
    speed: int              # 每秒播放的帧数
    loop: bool              # 是否循环播放

    @staticmethod
    def from_dir(path: str, speed: int, loop: bool) -> 'FrameAnimation':
        """从文件夹加载帧动画，以文件名的字典序决定帧的顺序"""

class FrameAnimator(Node):
    """序列帧动画器，通过控制`parent`节点的`texture`属性播放序列帧动画"""

    def __setitem__(self, name: str, anim: FrameAnimation):
        """增加一个帧动画"""

    def __getitem__(self, name: str) -> FrameAnimation:
        """获取一个帧动画"""

    def play(self, name: str) -> None:
        """播放一个帧动画"""

    def stop(self) -> None:
        """停止播放"""

#####################################################