class FrameAnimation:
    def __init__(self) -> None:
        self.frames = []        # 帧列表
        self.speed = 60         # 每秒播放的帧数
        self.loop = True        # 是否循环播放

class FrameAnimator(Node):
    def __init__(self):
        self._animations = {}
        self._current_animation = None
        self._current_frame = 0
        super(FrameAnimator, self).__init__()

    def __setitem__(self, name: str, anim: FrameAnimation):
        assert isinstance(anim, FrameAnimation)
        self._animations[name] = anim

    def __getitem__(self, name: str) -> FrameAnimation:
        return self._animations[name]

    def play(self, name: str):
        assert hasattr(self.parent, 'texture')
        anim = self._animations[name]
        if anim is self._current_animation:
            return
        self._current_frame = 0
        self._current_animation = anim

    def stop(self):
        self._current_animation = None

    def on_update(self):
        if self._current_animation is None:
            return
        self._current_frame += self._current_animation.speed * Time.delta_time
        if self._current_frame >= len(self._current_animation.frames):
            if self._current_animation.loop:
                self._current_frame = 0
            else:
                self._current_animation = None
                return
        self.parent.texture = self._current_animation.frames[int(self._current_frame)]