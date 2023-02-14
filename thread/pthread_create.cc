// pthread_create.cc

#include <pthread.h>
#include <iostream>
#include <unistd.h>

// 子线程回调函数
void* callback(void* arg) {
  for (int i = 0; i < 5; ++i) {
    std::cout << "子线程: i = " << i << std::endl;
  }
  std::cout << "子线程id: " << pthread_self() << std::endl;

  return NULL;
}

int main(){
    pthread_t tid;
    /* pthread_create 4个输入参数：
     * thread: 传出参数，是无符号长整形数，线程创建成功，会将线程 ID 写入到这个指针指向的内存中
     * attr: 线程的属性，一般情况下使用默认属性即可，写 NULL
     * start_routine: 函数指针，创建出的子线程的处理动作，也就是该函数在子线程中执行。
     * arg: 作为实参传递到 start_routine 指针指向的函数内部
     */
    pthread_create(&tid, NULL, callback, NULL);

    // 主线程
    for (int i = 0; i < 5; ++i) {
      std::cout << "主线程: i = " << i << std::endl;
    }

    /* pthread_self(): 查看线程id号, 一个无符号的长整形数 */
    std::cout << "主线程id: " << pthread_self() << std::endl;

    // 此时子线程不会执行，因为主线程执行完毕就释放了地址空间，子线程也跟着退出了。若想要子线程打印，则需要下面的步骤
    // 法一： sleep
    // sleep(1);

    // 法二： 线程退出函数 pthread_exit, 只要调用该函数当前线程就马上退出了，并且不会影响到其他线程的正常运行。
    pthread_exit(NULL);
    
    return 0;
}