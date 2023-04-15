## PainterEngine in your pocket

### `GameObject`

+ `.name`，获取一个名字
+ `GameObject(name='')`，构造函数
+ `AddComponent(self, type)`，添加一个组件
+ `GetChildCount(self)`，获取子物体的数量
+ `GetChild(self, index: int)`，按索引获取子物体
+ `childrens(self)`，返回一个`tuple`，表示此对象的所有子物体
+ `components(self)`，返回一个`tuple`，表示此物体包含的组件

### `PainterBehaviour`

+ `.gameObject`，获取所属的`GameObject`
+ `Awake(self)`，在初始化时被调用一次
+ `Update(self)`，每帧被调用一次

### `Time`

+ （静态变量）`deltaTime`，获取自上一帧经历的秒数，一个`float`值

## 全局变量与函数

+ `_root`，一个虚拟的根`GameObject`，你可以使用`_root.children()`来得到世界中的顶层`GameObject`。你不应该删除或重置此变量，否则将会触发SegFault
+ `_print_tree(go=None)`，用于调试，向控制台打印一个`GameObject`及其子物体
+ `Destroy(go)`，销毁一个`GameObject`或组件

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