# 线程相关概念及使用
**整个工程参考**：
[苏老师的线程教程](https://www.subingwen.cn/linux/thread/#2-1-%E7%BA%BF%E7%A8%8B%E5%87%BD%E6%95%B0)

## 1. 线程创建
参考 pthread_create.cc, 执行步骤编译和执行工程(`其他文件操作类似，不一一赘述`)
```
g++ pthread_create.cc -lpthread -o app
./app
```

## 2. 线程退出
参考 pthread_create.cc
只要调用该函数`当前线程就马上退出了`，并且`不会影响到其他线程`的正常运行，不管是在子线程或者主线程中都可以使用。

## 3. 线程回收
参考 pthread_join.cc

## 4. 线程分离
参考 pthread_detach.cc
pthread_detach线程分离之后在主线程中使用 pthread_join() 就`回收不到子线程资源`了。

## 5. 线程取消
需要同时满足两个条件：
- 在线程 A 中调用线程取消函数 `pthread_cancel`，指定杀死线程 B，这时候线程 B 是`死不了的`
- 在线程 B 中进程一次系统调用（从用户区切换到内核区），否则线程 B 可以一直运行。

## 6. 线程ID比较
```
int pthread_equal(pthread_t t1, pthread_t t2);
```