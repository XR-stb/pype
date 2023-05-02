import easing

class Tween:
    def __init__(self, obj, name, target, duration):
        self.obj = obj                  # 目标对象
        self.name = name                # 目标属性
        self.target = target            # 最终值
        self.duration = duration        # 持续时间
        self.ease = easing.EaseOutQuad  # 缓动函数
        self.on_complete = None         # 完成时回调

    def __iter__(self):
        self._start_time = Time.time
        self._start_val = getattr(self.obj, self.name)
        return self

    def __next__(self):
        t = Time.time - self._start_time
        if t >= self.duration:
            setattr(self.obj, self.name, self.target)
            if self.on_complete:
                self.on_complete()
            return StopIteration
        progress = self.ease(t / self.duration)
        curr = self._start_val + (self.target - self._start_val) * progress
        setattr(self.obj, self.name, curr)
        return curr


class TweenList:
    def __init__(self):
        self._tweens = []

    def append(self, tween):
        self._tweens.append(tween)

    def __iter__(self):
        self.index = 0
        assert len(self._tweens) > 0
        return self

    def __next__(self):
        tween = self._tweens[self.index]
        curr = next(tween)
        if curr is not StopIteration:
            return
        self.index += 1
        if self.index >= len(self._tweens):
            return StopIteration


class TweenSet:
    def __init__(self):
        self._tweens = set()

    def add(self, tween):
        self._tweens.add(tween)

    def __iter__(self):
        assert len(self._tweens) > 0
        return self

    def __next__(self):
        ended = []
        for tween in self._tweens:
            curr = next(tween)
            if curr is StopIteration:
                ended.append(tween)
        for tween in ended:
            self._tweens.remove(tween)
        if len(self._tweens) == 0:
            return StopIteration
        
def Node@to(name, target, duration, ease=None, on_complete=None):
    tween = Tween(self, name, target, duration)
    if ease:
        tween.ease = ease
    if on_complete:
        tween.on_complete = on_complete
    return self.start_coroutine(tween)