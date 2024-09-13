#include"algorithm.h"
void printALGORITHM(char *p)
{
printf("%s",p);

}
//队列操作
Queue* createQueue()
{
    Queue* queue=(Queue*)malloc(sizeof(Queue));
    queue->front = -1;
    queue->rear = -1;
    queue->size = MAX_SIZE;
    return queue;
}
int isFull(Queue* queue)
{
 return (queue->rear + 1) % queue->size == queue->front;
}
int isEmpty(Queue* queue)
{
return queue->front == -1;
}
void enqueue(Queue* queue, char* element)
{
 if (isFull(queue)) {
        printf("Queue is full.\n");
        return;
    }
    if (isEmpty(queue)) {
        queue->front = queue->rear = 0;
    } else {
        queue->rear = (queue->rear + 1) % queue->size;
    }
    queue->array[queue->rear] = element;
}
char* dequeue(Queue* queue)
{
if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        exit(1);
    }
    char* element = queue->array[queue->front];
    if (queue->front == queue->rear) {
        queue->front = queue->rear = -1;
    } else {
        queue->front = (queue->front + 1) % queue->size;
    }
    return element;
}
//链表操作
layerNode* createLayerList()
{
   layerNode* head = (layerNode*) malloc(sizeof(layerNode));
   head->layerData=NULL;
   head->next=NULL;
   head->layerType=Others;
   return head;

}
void addLayerList(layerNode*listHead, layerNode *node)
{
 //if(listHead!=NULL)
 while (listHead->next!=NULL)
 {
    /* code */
    listHead=listHead->next;
 }
 node->next=NULL;
 listHead->next=node;
}
void freeLayerList(layerNode *head)
{
  while (head!=NULL)
  {
    /* code */
    free(head->layerData);
    layerNode *temp=head;
    head=head->next;
    free(temp);
  }
  
}

//树操作
layerTreeNode *createLayerTreeNode()
{
layerTreeNode* root = NULL;
root= (layerTreeNode*) malloc(sizeof(layerTreeNode));
root->data="root";
root->nodeList=createLayerList();
return root;
    
}
void freeLayerTreeNode(layerTreeNode *root)
{
  
      free(root->data);//释放该节点data内存
      while (root->nodeList!=NULL)
      {
        /* code */
         layerNode *current=root->nodeList;
        freeLayerTreeNode((layerTreeNode *)current->layerData);//递归释放
        root->nodeList=current->next;
        free(current);//释放该节点链表当前节点内存
      }
      free(root);

}

//页面操作
pageConfig *createPage()
{
    pageConfig *page=(pageConfig*) malloc(sizeof(pageConfig));
    page->layers=createLayerList();
    return page;

}

void freePage(pageConfig *page)
{
   freeLayerList(page->layers);
   free(page);

}
