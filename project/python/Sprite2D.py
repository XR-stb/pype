class Sprite2D(Node):
    def __init__(self, texture):
        super(Sprite2D, self).__init__()
        self.texture = texture

    def _draw(self):
        _PX_TextureRenderEx(
            self.texture,
            self.globalPosition,
            self.globalAngle,
            self.globalScale
        )