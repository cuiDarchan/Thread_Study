/*
 * 场景描述：使用条件变量实现生产者和消费者模型，生产者有 5 个，往链表头部添加节点，消费者也有 5 个，删除链表头部的节点。
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <iostream>

// 链表的节点
struct Node{
    int number;
    Node* next;
};

// 定义条件变量
pthread_cond_t cond;
// 互斥锁
pthread_mutex_t mutex;
// 指向任务链表头结点指针
Node* head = NULL;

/// 生产者回调
void * producer(void * argc){
    
    while(1){
        pthread_mutex_lock(&mutex);
        // 采用头插法，不断插入节点
        Node *node = (Node*)malloc(sizeof(Node));
        node->number = rand()% 1000000;
        node->next = head;
        head = node;
        
        printf("+++ producer, number = %d, tid = %ld\n", node->number, pthread_self());
        pthread_mutex_unlock(&mutex);

        // 生产了任务，通知消费者消费
        pthread_cond_broadcast(&cond);

        sleep(rand() % 3); // s
    }
    return NULL;
}


/// 消费者回调
void * consumer(void*argc){
    
    while(1){
        pthread_mutex_lock(&mutex);
        // 删除节点
        while(head == NULL){
            // 线程阻塞，且mutex解锁
            pthread_cond_wait(&cond, &mutex);
        }
        // 删除头部节点
        Node* pnode = head;
        printf("--- consumer, number = %d, tid = %ld \n", pnode->number, pthread_self());
        head = pnode->next;
        free(pnode);
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3);
    }
    
    return NULL;
}

int main() {
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

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

  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&mutex);

  return 0;
}