## PainterEngine in your pocket

### `GameObject`

- [x] `.name`，获取名字
- [x] `.parent`，获取父物体
- [x] `.position`，获取世界坐标
- [ ] `.rotation`，获取旋转量
- [ ] `.scale`，获取缩放比例
- [ ] `.localPosition`，获取相对于父物体的位置
- [ ] `.localRotation`，获取相对于父物体的旋转量
- [ ] `.localScale`，获取相对于父物体的缩放比例
- [x] `GameObject(name='')`，构造函数
- [x] `AddComponent(self, type)`，添加一个组件
- [x] `GetChildCount(self)`，获取子物体的数量
- [x] `GetChild(self, index: int)`，按索引获取子物体
- [x] `GetComponent(self, type)`，在当前物体查找一个指定类型的组件
- [x] `GetComponentInChildren(self, type)`，在当前物体及其子物体查找一个指定类型的组件

---

- [x] `.children`，返回一个`iterator`，可用于遍历此对象的子物体
- [x] `.components`，此物体包含的组件，是一个`list`。你不应该直接修改它
- [x] `.Width`，获取宽度
- [x] `.Height`，获取高度
- [x] `.Length`，获取长度

### `PainterBehaviour`

- [x] `.gameObject`，获取所属的`GameObject`
- [x] `Awake(self)`，在初始化时被调用一次
- [x] `Update(self)`，每帧被调用一次
- [x] `GetComponent(self, type)`，相当于`self.gameObject.GetComponent`
- [x] `GetComponentInChildren(self, type)`，相当于`self.gameObject.GetComponentInChildrenInChildren`

### `Time`

- [x] [静态] `.deltaTime`，获取自上一帧经历的秒数，一个`float`值

### `SpriteRenderer`

- [ ] `.sprite`，设置精灵
- [ ] `.color`，设置颜色

### `Resources`

- [ ] [静态] `Load(path: str)`，加载一个资源，根据后缀名确定资源类型

## 全局变量与函数

- [x] `_root`，一个虚拟的根`GameObject`，你可以使用`_root.children`来遍历世界中的顶层`GameObject`。你不应该删除或重置此变量，否则将会引发异常
- [x] `_print_tree(go=None)`，用于调试，向控制台打印一个`GameObject`及其子物体
- [x] `_repl()`，打开简易REPL，在控制台请求用户输入，将结果`eval`后输出。可以使用`exit()`退出，此函数不处理异常

  ```python
  def _repl():
      while True:
          print(">>> ", end="")
          _s = input()
          if _s == "exit()":
              break
          print(eval(_s))
  ```
- [x] `Destroy(go)`，销毁一个`GameObject`

## 在Linux上开发

1. 确保`clang++`和`libc++`已安装。
2. 确认CMake已安装。
3. 确保`freegult3-dev`已安装。

```bash
git clone https://github.com/blueloveTH/PainterEngine
cd platform/linux
bash build.sh

build/PainterEngine
```