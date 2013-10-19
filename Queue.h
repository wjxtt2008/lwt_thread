#ifndef Queue_H
#define Queue_H

typedef lwt_struct* Item;
//typedef int Item;
typedef struct node * PNode;
typedef struct node
{
	Item data;
	PNode next;
}Node;

typedef struct
{
	PNode front;
	PNode rear;
	int size;
}Queue;
/***************regular Queue*******************/
/*Initialize a empty Queue*/
Queue *InitQueue();

/*Destroy a Queue*/
void DestroyQueue(Queue *pqueue);

/*Clear a Queue*/
void ClearQueue(Queue *pqueue);

/*Jude is Queue empty or not*/
int IsEmpty(Queue *pqueue);

/*Get size of Queue*/
int GetSize(Queue *pqueue);

/*Check the head node*/
PNode GetFront(Queue *pqueue,Item *pitem);

/*Check the tail node*/
PNode GetRear(Queue *pqueue,Item *pitem);

/*Add new item to Queue*/
PNode EnQueue(Queue *pqueue,Item item);

/*Get item out of Queue*/
PNode DeQueue(Queue *pqueue,Item *pitem);

/*traverse Queue and execute visit*/
void QueueTraverse(Queue *pqueue,void (*visit)());

/***************Circle Queue*******************/

/*Initialize a empty CircleQueue*/
//Queue *InitCircleQueue();

/*Add new item to CircleQueue*/
PNode EnCircleQueue(Queue *pqueue,Item item);

/*Get item out of CircleQueue*/
PNode DeCircleQueue(Queue *pqueue,Item *pitem);

/*Get current node,and move CircleQueue to next node*/
PNode MoveCircleQueue(Queue *pqueue,Item *pitem);

#endif

