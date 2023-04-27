"""pype核心模块"""

from typing import Any, Callable, Generator, Iterable

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

class Input:
    @staticmethod
    def get_key(c: str) -> bool:
        """检查当前帧某个按键是否被按下"""

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

    def children(self) -> Iterable['Node']:
        """返回子节点的迭代器"""

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
    def child_count(self) -> int:
        """获取子节点的数量"""
    def __getitem__(self, index: int) -> 'Node':
        """获取子节点"""

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

class Sprite(Node):
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

