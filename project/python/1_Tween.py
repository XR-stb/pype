import easing

class Tween:
    def __init__(self, obj):
        self.obj = obj
        self.completed = Signal()
        self._used = False

    def _setup(self):
        if self._used:
            raise ValueError("Tween实例只能播放一次")
        self._used = True

    def play(self):
        self._setup()
        self.obj.start_coroutine(self)

class Tweener(Tween):
    def __init__(self, obj, name, target, duration):
        super(Tweener, self).__init__(obj)
        self.name = name                # 目标属性
        self.target = target            # 最终值
        self.duration = duration        # 持续时间
        self.ease = easing.EaseOutQuad  # 缓动函数

        assert isinstance(obj, Node)
        assert type(name) is str
    
    def _setup(self):
        super(Tweener, self)._setup()
        self._start_time = Time.time
        self._start_val = getattr(self.obj, self.name)

    def __next__(self):
        t = Time.time - self._start_time
        if t >= self.duration:
            setattr(self.obj, self.name, self.target)
            self.completed.emit(self)
            return StopIteration
        progress = self.ease(t / self.duration)
        curr = self._start_val + (self.target - self._start_val) * progress
        setattr(self.obj, self.name, curr)
        return curr

class TweenList(Tween):
    def __init__(self, obj, items=None):
        super(TweenList, self).__init__(obj)
        self.tweens = list(items) or []

    def append(self, tween):
        self.tweens.append(tween)

    def _setup(self):
        super(TweenList, self)._setup()
        self._i = 0
        assert len(self.tweens) > 0
        self.tweens[0]._setup()

    def __next__(self):
        tween = self.tweens[self._i]
        if next(tween) is not StopIteration:
            return
        self._i += 1
        if self._i >= len(self.tweens):
            self.completed.emit(self)
            return StopIteration
        else:
            self.tweens[self._i]._setup()

class WaitForSignal:
    def __init__(self, signal):
        self.signal = signal

    def _on_signal(self, *args):
        self._flag = True
        self.signal.disconnect(self._on_signal)

    def __iter__(self):
        self.signal.connect(self._on_signal)
        self._flag = False
        return self

    def __next__(self):
        if self._flag:
            return StopIteration

def Node@to(self, name, target, duration, ease=None):
    tween = Tweener(self, name, target, duration)
    if ease:
        tween.ease = ease
    tween.play()
    return tween