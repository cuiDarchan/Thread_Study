// mutex.cc 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

#define MAX 100
// 全局变量
int number = 0;

// 创建一把互斥锁
// 全局变量, 多个线程共享
pthread_mutex_t mutex;

void* countNumberA(void* arg) {
  for (int i = 0; i < MAX; ++i) {
    pthread_mutex_lock(&mutex);
    number += i;
    usleep(10);
    pthread_mutex_unlock(&mutex);
    printf("Thread A, id = %lu, number = %d\n", pthread_self(), number);
  }
  return NULL;
}

void* countNumberB(void* arg) {
  for (int i = 0; i < MAX; ++i) {
    pthread_mutex_lock(&mutex);
    number += i;
    pthread_mutex_unlock(&mutex);
    printf("Thread B, id = %lu, number = %d\n", pthread_self(), number);
    usleep(5);
  }
  return NULL;
}

int main(){
    pthread_t p1,p2;

    // 初始化互斥锁
    pthread_mutex_init(&mutex, NULL);

    // 创建两个线程
    pthread_create(&p1, NULL, countNumberA, NULL);
    pthread_create(&p2, NULL, countNumberB, NULL);

    // 线程回收
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    // 线程销毁后，释放互斥锁
    pthread_mutex_destroy(&mutex);
    
    return 0;
}