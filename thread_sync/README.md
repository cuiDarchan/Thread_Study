# 线程同步概念及使用
**整个工程参考**：
[苏老师的线程同步教程](https://subingwen.cn/linux/thread-sync/)

综述，所谓的同步`并不是多个线程同时`对内存进行访问，而是按照`先后顺序`依次进行的。

## 1. 线程同步概念
### 1.1 为什么要线程同步？ 
交替数数的例子。
CPU寄存器 <-> 三级缓存 <-> 物理内存 <-> 硬盘

### 1.2 同步方式
4种方式：`互斥锁、读写锁、条件变量、信号量。`

## 2. 互斥锁
```
pthread_mutex_t  mutex;
/// 初始化互斥锁, restrict: 是一个关键字, 用来修饰指针, 只有这个关键字修饰的指针可以访问指向的内存地址, 其他指针是不行的;
/// attr: 互斥锁的属性，一般使用默认属性即可，这个参数指定为 NULL
int pthread_mutex_init(pthread_mutex_t *restrict mutex,
           const pthread_mutexattr_t *restrict attr);
/// 释放互斥锁资源            
int pthread_mutex_destroy(pthread_mutex_t *mutex);

/// 加锁
int pthread_mutex_lock(pthread_mutex_t *mutex);
/// 尝试加锁
int pthread_mutex_trylock(pthread_mutex_t *mutex);
// 对互斥锁解锁
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

## 3. 死锁
### 死锁几种情况：
- 加锁后忘记解锁
- 重复加锁

## 4. 读写锁
读写锁是互斥锁的`升级版`，在做读操作的时候可以提高程序的执行效率。如果所有的线程都是做读操作, 那么`读是并行的，写是串行的，写锁比读锁的优先级高。`。
```
pthread_rwlock_t rwlock;

// 初始化读写锁
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
           const pthread_rwlockattr_t *restrict attr);
// 释放读写锁占用的系统资源
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);

```