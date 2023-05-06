# pype

pype是一个由Python/C++语言编写的完整开源的跨平台图形应用框架，
可移植到Windows Linux Android iOS 支持WebAssembly的Web端等（嵌入式MCU除外）。

## 快速开始

+ [构建Windows版本](https://zhuanlan.zhihu.com/p/627077407)
+ [构建Linux/WSL版本](https://zhuanlan.zhihu.com/p/627079964)
+ [在Linux上构建Web版本](https://zhuanlan.zhihu.com/p/627082462)
+ [在Linux上构建Android版本](https://zhuanlan.zhihu.com/p/627084127)

### Web演示

https://blueloveth.github.io/pype/

```python
from pype import *

def load_fox_anim(name):
    anim = FrameAnimation()
    anim.frames = load_dir(f"assets/fox/{name}")
    anim.speed = 12
    anim.loop = True
    return anim

class Player(Sprite2D):
    def on_ready(self) -> None:
        self.scale = 0.5
        self.position = Vector2(400, 300)

        self.animator = FrameAnimator()
        self.animator['run'] = load_fox_anim('run')
        self.animator['walk'] = load_fox_anim('walk')
        self.animator.parent = self
        self.animator.play('walk')

    def on_update(self) -> None:
        delta_x = 0
        if Input.get_key(KeyCode.A):
            delta_x -= 1
        if Input.get_key(KeyCode.D):
            delta_x += 1
        if delta_x != 0:
            self.animator.play('run')
            self.flip = delta_x < 0
        else:
            self.animator.play('walk')
        self.x += delta_x * Time.delta_time * 200

Player()
```

## 教程

https://zhuanlan.zhihu.com/p/625784518


## 热重载（实验）

你可以按下`F5`键执行热重载，这将会重新加载你的Python脚本，并将更新后的代码应用到场景中。
而不用重新编译或重启引擎。

## 贡献代码

你可以使用Python或者C++编写新的类实现，并提交Pull Request。

+ 若要编写纯Python类，将对应的Python源文件放置在`project/python/<类名>.py`。
该目录下的代码会自动打包到二进制中，成为`pype`模块的一部分。
你可以参考
[Node](https://github.com/blueloveTH/pype/blob/master/project/python/0_Node.py) 的实现

+ 若要编写C++类，将对应的头文件放置在`project/<类名>.h`。你可以参考
[Vector2](https://github.com/blueloveTH/pype/blob/master/project/Vector2.h) 的实现