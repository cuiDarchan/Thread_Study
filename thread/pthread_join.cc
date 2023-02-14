// pthread_join.cc

#include <pthread.h>
#include <iostream>
#include <unistd.h>

struct Test{
  int num;
  int age;
};

// 法一：定义全局变量test
Test test;
// 子线程回调函数
void* callback(void* arg) {
  std::cout << "子线程id: " << pthread_self() << std::endl;

  test.num = 100;
  test.age = 66;
  pthread_exit(&test);  // 传出的是 传出参数的地址, 此写法存在问题，返回的临时变量指针, 可以将test定义为全局变量，作为法一。
}



// 法二 子线程回调函数
void* callback2(void* arg) {
  std::cout << "子线程id: " << pthread_self() << std::endl;
  
  // 法二
  Test* t = (Test *)arg;
  t->num = 100;
  t->age = 66;
  pthread_exit(t);  // 传出的是 传出参数的地址, 此写法存在问题，返回的临时变量指针, 可以将test定义为全局变量，作为法一。
}



int main(){
    // 法一
    // pthread_t tid;
    // pthread_create(&tid, NULL, callback, NULL);

    // 法二: 通过创建子线程时传入主线程中变量的地址，达到参数传递作用，使得子线程可以访问主线程的栈空间
    Test t; 
    pthread_t tid;
    pthread_create(&tid, NULL, callback2, &t);
    
    // 主线程
    std::cout << "主线程id: " << pthread_self() << std::endl;

    /* pthread_join 阻塞函数，等待子线程执行完毕，回收子线程的内核资源，再继续执行，传入参数如下：
     * thread: 要被回收的子线程的线程 ID
     * retval: 二级指针，指向一级指针的地址，是一个传出参数，这个地址中存储了 pthread_exit () 传递出的数据，如果不需要这个参数，可以指定为 NULL
     */
    void* ptr = nullptr; // 一级指针，为了指向callback中传出的数据
    pthread_join(tid, &ptr);

    // 法一： 打印传递的参数
    // Test* t = (Test*)ptr; // ptr 进行数据类型转换
    // std::cout << "t->num: " << t->num << std::endl;
    // std::cout << "t->age: " << t->age << std::endl;

    // 法二： 打印传递的参数
    std::cout << "t.num: " << t.num << std::endl;
    std::cout << "t.age: " << t.age << std::endl;
    return 0;
}