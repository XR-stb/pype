from pype import _repl, _root, _print_tree
from pype import *

class Test(Node):
    def on_update(self) -> None:
        if Input.get_key_down(KeyCode.Alpha0):
            print("key down");
        if Input.get_key_up(KeyCode.Alpha0):
            print("key up");    
        if Input.get_mouse_button_down(0):
            print("mouse down");
        if Input.get_mouse_button_up(0):
            print("mouse up");

Test()

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

# class Item:
#     def __init__(self, k, v):
#         self.k = k
#         self.v = v
#     def __lt__(self, other):
#         return self.v < other.v
#     def __gt__(self, other):
#         return self.v > other.v
#     def __eq__(self, other):
#         return self.v == other.v
#     def __repr__(self):
#         k = self.k
#         v = self.v
#         return f'    {k}: int = {v}'
# s = ['class Input:']
# array = []
# for k,v in Input.__dict__.items():
#     if type(v) is int:
#         array.append(Item(k, v))
# array.sort()
# for item in array:
#     s.append(repr(item))
# with open('Input.pyi', 'w') as f:
#     f.write('\n'.join(s))