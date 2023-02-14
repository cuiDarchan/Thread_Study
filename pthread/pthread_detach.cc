// pthread_detach.cc

#include <pthread.h>
#include <iostream>
#include <unistd.h>

struct Test{
  int num;
  int age;
};

// 子线程回调函数
void* callback(void* arg) {
  for (int i = 0; i < 5; ++i) {
    std::cout << "子线程: i = " << i << std::endl;
  }
  std::cout << "子线程id: " << pthread_self() << std::endl;

  Test* t = (Test *)arg;
  t->num = 100;
  t->age = 66;
  pthread_exit(t);  // 传出的是 传出参数的地址, 此写法存在问题，返回的临时变量指针, 可以将test定义为全局变量，作为法一。
}



int main(){
    // 通过创建子线程时传入主线程中变量的地址，达到参数传递作用，使得子线程可以访问主线程的栈空间
    Test t; 
    pthread_t tid;
    pthread_create(&tid, NULL, callback, &t);
    // 主线程
    std::cout << "主线程id: " << pthread_self() << std::endl;
    
    // 线程分离
    pthread_detach(tid);
    
    // 主线程退出，但子线程不受影响，因为上一步已经做了线程分离
    pthread_exit(NULL);

    return 0;
}