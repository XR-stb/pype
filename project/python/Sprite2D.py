class Sprite2D(Node):
    def __init__(self):
        super(Sprite2D, self).__init__()
        self.texture = None

    def _draw(self):
        if self.texture is None:
            return
        _PX_TextureRenderEx(
            self.texture,
            self.globalPosition,
            self.globalAngle,
            self.globalScale
        )