# pype

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
```
### Linux/WSL构建方法

1. 确保`clang++`和`libc++`已安装。
2. 确认CMake已安装。
3. 确保`freeglut3-dev`已安装。

```bash
bash build_linux.sh
```

### Web演示

https://blueloveth.github.io/pype/


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