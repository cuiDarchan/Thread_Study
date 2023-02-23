/*
 * 场景描述：使用信号量实现生产者和消费者模型，生产者有 5 个，往链表头部添加节点，消费者也有 5 个，删除链表头部的节点。
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <semaphore.h>

// 链表的节点
struct Node{
    int number;
    Node* next;
};

// 定义生产者信号量
sem_t psem;
// 定义消费者信号量
sem_t csem;
// 互斥锁
pthread_mutex_t mutex;
// 指向任务链表头结点指针
Node* head = NULL;

/// 生产者回调
void * producer(void * argc){
    
    while(1){
        // 生产者减少一个信号灯
        /// 注意如下两行顺序不能颠倒，否则容易造成死锁，假设如果消费者线程先拿到了锁，详细参考见苏大神分析。
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);

        // 采用头插法，不断插入节点
        Node *node = (Node*)malloc(sizeof(Node));
        node->number = rand()% 1000000;
        node->next = head;
        head = node;
        
        printf("+++ producer, number = %d, tid = %ld\n", node->number, pthread_self());
        pthread_mutex_unlock(&mutex);

        // 通知消费者，增加一个信号灯
        sem_post(&csem);

        sleep(rand() % 3); // s
    }
    return NULL;
}


/// 消费者回调
void * consumer(void*argc){
    
    while(1){

        sem_wait(&csem);
        
        pthread_mutex_lock(&mutex);
        
        // 删除头部节点
        Node* pnode = head;
        printf("--- consumer, number = %d, tid = %ld \n", pnode->number, pthread_self());
        head = pnode->next;
        free(pnode);
        
        pthread_mutex_unlock(&mutex);
        
        sem_post(&psem);
        
        sleep(rand() % 3);
    }
    
    return NULL;
}

int main() {
  pthread_mutex_init(&mutex, NULL); // 注意此时不需要
  // 初始化信号灯, 第二个参数0 代表线程同步， 1代表产生一个信号灯
  sem_init(&psem, 0, 5);
  sem_init(&csem, 0, 0);

  pthread_t ptid[5];
  pthread_t ctid[5];
  for (int i = 0; i < 5; ++i) {
    pthread_create(&ptid[i], NULL, producer, NULL);
    pthread_create(&ctid[i], NULL, consumer, NULL);
  }

  // 线程回收
  for (int i = 0; i < 5; ++i) {
    pthread_join(ptid[i], NULL);
    pthread_join(ctid[i], NULL);
  }

  sem_destroy(&psem);
  sem_destroy(&csem);
  pthread_mutex_destroy(&mutex);

  return 0;
}