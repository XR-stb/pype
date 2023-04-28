from pype import _repl, _root, _print_tree
from pype import *

# 创建一个精灵对象
sprite = Sprite2D()
sprite.scale = 0.5
sprite.position = Vector2(100, 100)

# 创建一个序列帧动画器，并将其作为子节点
animator = FrameAnimator()
animator.parent = sprite

# 从文件夹加载序列帧动画，速度为12帧/秒
animator['fox_run'] = FrameAnimation.from_dir("assets/fox/run", 12)

# 播放动画
animator.play("fox_run")






# for i in range(4):
#     b = Button()
#     b.text = f"Hello! {i}"
#     b.width = 200
#     b.height = 50
#     b.position = Vector2(100, 100+i*60)
#     b.clicked.connect(lambda _: print(_))

# tex = load("assets/logo.png")
# print(tex)

# img = Image()
# img.texture = tex
# img.set_native_size()