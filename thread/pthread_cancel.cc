// pthread_cancel.cc

#include <pthread.h>
#include <iostream>
#include <unistd.h>

// 子线程回调函数
void* callback(void* arg) {
  
  printf("子线程回调，间接调用系统函数，杀死子线程。\n");
  
  for (int i = 0; i < 5; ++i) {
    std::cout << "子线程: i = " << i << std::endl; // ** 子线程: i = 0 偶尔也会被打印 **
  }
  return NULL;
}

int main(){
    pthread_t tid;
    pthread_create(&tid, NULL, callback, NULL);
    printf("我是子线程, 线程ID: %ld\n", tid);
    
    printf("我是主线程, 线程ID: %ld\n", pthread_self());
    for(int i=0; i<3; ++i)
    {
        printf("主线程 i = %d\n", i);
    }
    
    // 线程取消，必须满足两个条件
    auto flag = pthread_cancel(tid);
    if (flag == 0)
      printf("子线程取消成功\n");
    else
      printf("子线程取消失败\n");

    pthread_join(tid, NULL);
    return 0;
}