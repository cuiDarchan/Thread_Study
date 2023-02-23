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

## 5.条件变量
### 5.1 函数原型
```
#include <pthread.h>
pthread_cond_t cond;

/// 初始化，cond: 条件变量的地址，attr: 条件变量属性，一般使用默认属性，指定为 NULL
int pthread_cond_init(pthread_cond_t *restrict cond,
      const pthread_condattr_t *restrict attr);
/// 销毁释放资源        
int pthread_cond_destroy(pthread_cond_t *cond);
```
### 5.2 线程阻塞
- 在阻塞线程时候，如果线程已经对互斥锁 mutex 上锁，那么会`将这把锁打开`，这样做是为了避免死锁
- 当线程`解除阻塞`的时候，函数内部会帮助这个线程再次将这个 mutex 互斥锁锁上，继续向下访问临界区
```
// 线程阻塞函数, 哪个线程调用这个函数, 哪个线程就会被阻塞
int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);

/// 表示的时间是从1971.1.1到某个时间点的时间, 总长度使用秒/纳秒表示
struct timespec {
	time_t tv_sec;      /* Seconds */
	long   tv_nsec;     /* Nanoseconds [0 .. 999999999] */
};
/// 将线程阻塞一定的时间长度, 时间到达之后, 线程就解除阻塞了
int pthread_cond_timedwait(pthread_cond_t *restrict cond,
           pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
```
### 5.3 线程唤醒
```
// 唤醒阻塞在条件变量上的线程, 至少有一个被解除阻塞
int pthread_cond_signal(pthread_cond_t *cond);
// 唤醒阻塞在条件变量上的线程, 被阻塞的线程全部解除阻塞
int pthread_cond_broadcast(pthread_cond_t *cond);
```

### 5.4 条件变量
生产者消费者模型，参考cond.cc

### 5.5 信号量（信号灯）
信号量主要`阻塞线程`，不能完全保证线程安全，如果要保证线程安全，需要`信号量和互斥锁一起使用`。
```
#include <semaphore.h>
sem_t sem;

// 初始化信号量/信号灯, 
/// 参数：pshared：0：线程同步;非 0：进程同步. value：初始化当前信号量拥有的资源数（>=0），如果资源数为 0，线程就会被阻塞了。
int sem_init(sem_t *sem, int pshared, unsigned int value);

// 资源释放, 线程销毁之后调用这个函数即可
// 参数 sem 就是 sem_init() 的第一个参数            
int sem_destroy(sem_t *sem);

// 参数 sem 就是 sem_init() 的第一个参数  
// 函数被调用sem中的资源就会被消耗1个, 资源数-1
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
```

#### 消费者生产者模型
```
// 消费一个信号灯
sem_wait(&sem);
// 添加一个信号灯
sem_post(&sem);
```
将cond.cc 改为信号量形式 sem.cc