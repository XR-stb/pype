class Sprite2D(Node):
    def __init__(self):
        self.texture = None
        self.flip = False
        self.color = Colors.White
        super(Sprite2D, self).__init__()

    def _draw(self):
        if self.texture is None:
            return
        gl.draw_texture(
            self.texture,
            *self.global_transform,
            self.color,
            self.flip
        )