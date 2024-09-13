#ifndef ALGORITHM_H
#define ALGORITHM_H

#include<stdio.h>
#include<stdlib.h>
#include"hcreate.h"



void printALGORITHM(char *p);


//队列操作
#define MAX_SIZE 10000
typedef struct Queue {
    char*array[MAX_SIZE];
    int front;
    int rear;
    int size;
} Queue;
Queue* createQueue();
int isFull(Queue* queue);
int isEmpty(Queue* queue);
void enqueue(Queue* queue, char* element);
char* dequeue(Queue* queue);


//链表定义
typedef struct layerNode
{
  void *layerData;
  LayerType layerType;
  struct layerNode *next;

}layerNode;
//链表操作
layerNode*  createLayerList();
void addLayerList(layerNode*listHead, layerNode *node);
void freeLayerList();

//树定义
typedef struct  layerTreeNode
{
    /* data */
  void *data;
  struct layerNode *nodeList;
}layerTreeNode;
//树操作
layerTreeNode *createLayerTreeNode();
void freeLayerTreeNode();


//页面数据结构定义
typedef struct pageConfig
{
  int dpiX;
  int dpiY;
  int pageWidth;
  int pageHeight;
  int pagePadding;
  int pageHorizontalSpacing;
  int pageVerticalSpacing;
  layerNode *layers;
}pageConfig;
//页面操作
pageConfig *createPage();
void freePage(pageConfig *page);
#endif