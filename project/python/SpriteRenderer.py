class SpriteRenderer(Component):
    def __init__(self, gameObject):
        super(SpriteRenderer, self).__init__(gameObject)
        self.sprite = None
        # self.color = (1.0, 1.0, 1.0, 1.0)

    def __call__(self):
        if self.sprite is None:
            return
        pos = self.gameObject.position
        angle = self.gameObject.angle
        scale = self.gameObject.scale
        _PX_TextureRenderEx(self.sprite, pos, angle, scale)

    def Awake(self):
        if self.gameObject.renderer is not None:
            raise ValueError("物体已经有渲染器了")
        self.gameObject.renderer = self

    def OnDestroy(self):
        self.gameObject.renderer = None