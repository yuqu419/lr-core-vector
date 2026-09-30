# lr-core-vector

用 C 语言手写一个 `std::vector`：你要做的事只有一件：根据 `include/vector.h` 的描述**把 `src/vector.c` 里的空壳函数填成能用的实现，让 `make test` 全绿**。

[vector 原理可视化](https://lingrui-studio.github.io/vector-playground/)

本题实现的是只存储 `int` 的教学版动态数组，具体约定以 [include/vector.h](include/vector.h) 为准。

## 目录结构

```
lr-core-vector/
├── readme.md         本文件
├── Makefile          构建脚本（不用改）
├── .gitignore        列举 git 需要忽视的文件
├── .clang-format     格式化要求
├── include/
│   └── vector.h      接口声明 + 函数注释（不用改，但要读懂）
├── src/
│   └── vector.c      ★ 你要实现的地方
└── tests/
    └── test.c        单元测试（不用改）
```

## 自检

补全 [include/vector.c](include/vector.c) 中的函数实现后，项目根目录运行 `make test`，若最后输出结果如下即表示你完成了本项目（本项目只有未完成和已完成两种状态，不存在中间值）：

```bash
== 通过 3393 项，失败 0 项 ==
全部通过，可以 commit & push 了
```

测试始终开启 ASan + UBSan，检测到错误会以失败状态退出，不提供关闭开关。常见错误会被直接指出来，例如：

```
ERROR: AddressSanitizer: heap-buffer-overflow on address 0x... at pc 0x...
READ of size 4 at 0x... thread T0
    #0 0x... in get src/vector.c:52
```

行号会直接指到出问题的那一行，看不懂的把完成代码和报错信息复制给 AI 问一下。

## 提交

- 完成下面的`实现思路`一节，简要说明你的各个函数是如何实现的，尤其注意内存管理的说明
- 把所有修改 commit 并 push 到 GitHub 上自己的 vector 仓库
- 在个人仓库的 Actions 页面手动触发一次自动评分工作流

## 实现思路

### vector_init
- `vector`中的`data`,`end`还有`cap`指针指向分配在堆中内存，但整个`vector`都是在栈上的
- 使用`malloc`申请一块大小为`capacity * sizeof(int)`的连续内存，这样方便后续释放内存

![init](/src/img/initial.png)

### vector_destroy
- 释放堆内存，这里一定要传`data`指针，就能将整个申请的堆内存释放掉，因为可以通过`data`找到对应的`malloc_chunk`，其中记录着之前申请内存的大小
### size
- 因为申请的内存连续的，所以`size = end - data`
### capacity
- 与`size`同理，`capacity = cap - data`
### empty
- 就是`!size()`
### get和set
- 直接通过`data`偏移指定的长度即可找到
### front和back
- 与`get`是一回事
### push_back
- 难点在扩容，在`realloc`之后，可能内存会进行转移，所以`data`,`end`和`cap`都需要重新计算
- 扩容前后，`data`总是指向起始位置，`end`与`data`的相对位置不变，`cap`总是指向最后一个内存再偏移一个单位
![extend](/src/img/extend.png)
### pop_back
- 让`end`指针左移一个单位即可
### reserve
- 重新分配到所需大小即可（与扩容类似）
### shrink_to_fit
- 与`reserve`相似，不过指定`capacity == size`
### clear
- 直接让`data == end`即可

