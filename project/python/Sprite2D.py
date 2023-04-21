class Sprite2D(Node):
    def __init__(self):
        self.texture = None
        super(Sprite2D, self).__init__()

    def _draw(self):
        if self.texture is None:
            return
        _PX_TextureRenderEx(
            self.texture,
            self.globalPosition,
            self.globalAngle,
            self.globalScale
        )