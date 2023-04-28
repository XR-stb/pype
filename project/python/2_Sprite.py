class Sprite(Node):
    def __init__(self):
        self.texture = None
        super(Sprite, self).__init__()

    def _draw(self):
        if self.texture is None:
            return
        _PX_TextureRenderEx(
            self.texture,
            *self.global_transform
        )