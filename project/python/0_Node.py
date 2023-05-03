# 所有物体的虚基类
class Node:
    on_ready = None              # 初始化函数
    on_update = None             # 更新函数
    on_destroy = None            # 销毁函数

    children = ...          # 获取子节点的元组（C实现）
    child_count = ...       # 获取子节点数量（C实现）
    parent = ...            # 获取/设置父节点（C实现）
    position = ...          # 获取/设置本地位置（C实现）
    _draw = ...             # 绘制函数
    _px_obj_init = ...      # 指示如何初始化对应的PX_Object（C实现）
    
    def __init__(self):
        # a void_p object ~ PX_Object*
        self._px_obj = self._px_obj_init()

        self.name = "未命名节点"
        self.angle = 0.0    # 本地坐标系 角度
        self.scale = 1.0    # 本地坐标系 缩放

        self._coroutines = []           # 当前的协程列表
        self._stopped_coroutines = []   # 即将在帧结束后停止的协程列表

        if self.on_ready is not None:
            self.on_ready()

    def _update(self):
        if self.on_update is not None:
            self.on_update()
        # 更新协程
        for coroutine in self._coroutines:
            obj = next(coroutine)
            if obj is StopIteration:
                self._stopped_coroutines.append(coroutine)
        if self._stopped_coroutines:
            self._coroutines = [
                c for c in self._coroutines
                if c not in self._stopped_coroutines
            ]
            self._stopped_coroutines.clear()

    @property
    def global_position(self):
        pos = self.position
        obj = self.parent
        while obj is not None:
            pos += obj.position
            obj = obj.parent
        return pos

    @property
    def global_angle(self):
        angle = self.angle
        obj = self.parent
        while obj is not None:
            angle += obj.angle
            obj = obj.parent
        return angle

    @property
    def global_scale(self):
        scale = self.scale
        obj = self.parent
        while obj is not None:
            scale *= obj.scale
            obj = obj.parent
        return scale
    
    @property
    def global_transform(self):
        pos = self.position
        angle = self.angle
        scale = self.scale
        obj = self.parent
        while obj is not None:
            pos += obj.position
            angle += obj.angle
            scale *= obj.scale
            obj = obj.parent
        return pos, angle, scale

    def start_coroutine(self, coroutine):
        self._coroutines.append(coroutine)
        return coroutine

    def stop_coroutine(self, coroutine):
        self._stopped_coroutines.append(coroutine)

    def stop_all_coroutines(self):
        self._stopped_coroutines = self._coroutines.copy()


# 全局协程创建函数
def WaitForEndOfFrame():
    yield None

def WaitForSeconds(seconds):
    total = 0
    while total < seconds:
        total += Time.delta_time
        yield None

def WaitUntil(condition):
    while not condition():
        yield None