# pype

本仓库提供PainterEngine与 [PocketPy](https://github.com/blueloveTH/pocketpy) 的绑定，
它允许用户使用高层次的Python语法和特性来编写游戏，同时具有跨平台的能力。

> 运行一个Python解释器需要消耗较多的资源，你的目标平台应具备`50MB`以上的内存和C++17支持。

## 快速开始

要进行构建，首先切换到仓库的根目录。

### Windows构建方法

1. 确保`cl.exe`已安装。
2. 确认CMake已安装。

```
python preprocess.py
mkdir build
cd build
cmake ..
cmake --build . --config Release
cd Release
PainterEngine.exe
```
### Linux/WSL构建方法

1. 确保`clang++`和`libc++`已安装。
2. 确认CMake已安装。
3. 确保`freegult3-dev`已安装。

```bash
bash build_dev.sh
bash run_dev.sh
```

构建完成后，在可执行程序的目录下建立一个`main.py`文件，并输入如下代码，
运行后，你将会在控制台看到不断递增的数字。

```python
from PainterEngine import *

class Test(PainterBehaviour):
    def Awake(self):
        self.counter = 1

    def Update(self):
        self.counter += 1
        print(self.counter)

go = GameObject()
go.AddComponent(Test)
```

## 贡献代码

你可以使用Python或者C++编写新的类实现，并提交Pull Request。

+ 若要编写纯Python类，将对应的Python源文件放置在`project/python/<类名>.py`。
该目录下的代码会自动打包到二进制中，成为`PainterEngine`模块的一部分。
你可以参考
[Component](https://github.com/blueloveTH/pype/blob/master/project/python/Component.py) 的实现

+ 若要编写C++类，将对应的头文件放置在`project/<类名>.h`。你可以参考
[Vector2](https://github.com/blueloveTH/pype/blob/master/project/Vector2.h) 的实现

+ 你可以同时使用C++和Python编写类，例如`GameObject`类就是使用这种方式实现的。

## API参考

### `GameObject`

- [x] `.name`，获取名字，返回`str`
- [x] `.parent`，获取父物体，返回`GameObject`
- [x] `.position`，获取世界坐标，返回`Vector2`
- [x] `.localPosition`，获取相对于父物体的坐标
- [x] `.activeSelf`，获取是否激活，返回`bool`
- [x] `GameObject(name=None)`，构造函数
- [x] `SetActive(self, active: bool)`，设置是否激活
- [x] `AddComponent(self, tp)`，添加一个组件
- [x] `GetChildCount(self)`，获取子物体的数量
- [x] `GetChild(self, index: int)`，按索引获取子物体
- [x] `GetComponent(self, tp)`，在当前物体查找一个指定类型的组件
- [x] `GetComponentInChildren(self, tp)`，在当前物体及其子物体查找一个指定类型的组件
- [x] [静态] `Find(name: str)`，查找一个名字为`name`的物体
---

- [x] `.children`，返回一个迭代器，可用于遍历此对象的子物体
- [x] `.components`，此物体包含的组件，是一个`list`。你不应该直接修改它
- [x] `.Width`，获取宽度
- [x] `.Height`，获取高度

### `PainterBehaviour`

- [x] `.gameObject`，获取所属的`GameObject`
- [x] [消息] `Awake(self)`，在初始化时被调用一次
- [x] [消息] `Update(self)`，每帧被调用一次
- [x] [消息] `OnDestroy(self)`，在销毁时被调用一次
- [x] `GetComponent(self, tp)`，相当于`self.gameObject.GetComponent`
- [x] `GetComponentInChildren(self, tp)`，相当于`self.gameObject.GetComponentInChildren`
- [x] `StartCoroutine(self, obj)`，启动一个协程
- [x] `StopCoroutine(self, obj)`，停止一个协程
- [x] `StopAllCoroutines(self)`，停止所有协程

协程通常配合如下的函数来使用：

- [x] `WaitForEndOfFrame()`，等待一帧
- [x] `WaitForSeconds(seconds: float)`，等待`seconds`秒

例如下面是一个简单的协程，它会在3秒后打印`Hello World!`。

```python
class Test(PainterBehaviour):
    def Awake(self):
        self.StartCoroutine(self.coroutine())

    def coroutine(self):
        yield from WaitForSeconds(3.0)
        print("Hello World!")
```

### `Input`

- [x] [静态] `GetKey(code)`，检查当前帧某个按键是否被按下，返回`bool`

    ```python
    class Test(PainterBehaviour):
        def Update(self):
            if Input.GetKey("A"):
                print("A已经被按下啦！")
    ```

### `Time`

- [x] [静态] `.deltaTime`，获取自上一帧经历的秒数，一个`float`值

### `SpriteRenderer`

- [x] `.sprite`，设置精灵
- [ ] `.color`，设置颜色

### `Resources`

- [x] [静态] `Load(path: str)`，加载一个资源并返回一个指针，此函数带有缓存，因此多次调用同一个资源不会重复加载

## 全局变量与函数
- [x] `traverse(go=None)`，返回一个可以遍历`GameObject`及其所有子物体的迭代器，每个项`(GameObject, int)`表示物体及其深度（从0开始）
  ```python
  def traverse(go=None):
      q = deque()
      q.append((go or _root, -1))
      while len(q) > 0:
          curr, depth = q.popleft()
          if depth > -1:
              yield curr, depth
          for child in curr.children:
              q.append((child, depth+1))
  ```
- [x] `print_tree(go=None)`，用于调试，向控制台打印一个`GameObject`及其所有子物体，内部调用了`traverse`
- [x] `Destroy(go)`，销毁一个`GameObject`或`Component`
- [x] `_root`，访问虚拟根`GameObject`，你可以使用`_root.children`来遍历世界中的顶层`GameObject`。你不应该删除或重置此变量，否则将会引发异常
- [x] `_repl()`，打开一个简易REPL，请求用户输入并将结果`eval`后输出。可以使用`exit()`退出

  ```python
  def _repl():
      while True:
          print(">>> ", end="")
          _s = input()
          if _s == "exit()" or _s == "":
              break
          print(eval(_s))
  ```