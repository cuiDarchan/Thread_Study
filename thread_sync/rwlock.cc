/*
 * 题目要求：8 个线程操作同一个全局变量，3 个线程不定时写同一全局资源，5 个线程不定时读同一全局资源。
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <iostream>

// 全局变量
int number = 0;

// 读写锁
pthread_rwlock_t rwlock;

void* read_func(void* argc) {
  while (1) {
    pthread_rwlock_rdlock(&rwlock);
    printf("读操作, number = %d, tid = %ld \n", number, pthread_self());
    pthread_rwlock_unlock(&rwlock);
    usleep(rand() % 100);  // ms
  }

  return NULL;
}

void* write_func(void* argc) {
  while (1) {
    pthread_rwlock_wrlock(&rwlock);
    int cur = number;
    number = ++cur;
    printf("写操作++, number = %d, tid = %ld \n", number, pthread_self());
    pthread_rwlock_unlock(&rwlock);
    usleep(rand() % 100);  // ms
  }

  return NULL;
}

int main() {
  pthread_t tid_list[8];
  pthread_rwlock_init(&rwlock, NULL);

  for (int i = 0; i < 3; ++i) {
    pthread_create(&tid_list[i], NULL, write_func, NULL);
  }

  for (int i = 3; i < 8; ++i) {
    pthread_create(&tid_list[i], NULL, read_func, NULL);
  }

  // ** 线程回收join， 第一次写遗忘, 故没有循环执行，主线程结束直接退出了
  for (int i = 0; i < 8; ++i) {
    pthread_join(tid_list[i], NULL);
  }

  pthread_rwlock_destroy(&rwlock);
  return 0;
}