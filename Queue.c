#include"lwt.h"
//#include"Node.h"
#include"Queue.h"
#include<malloc.h>
#include<stdio.h>

/*Initialize a empty Queue*/
Queue *InitQueue()
{
	Queue *pqueue = (Queue *)malloc(sizeof(Queue));
	if(pqueue!=NULL)
	{
		pqueue->front = NULL;
		pqueue->rear = NULL;
		pqueue->size = 0;
	}
	return pqueue;
}

/*Destroy a Queue*/
void DestroyQueue(Queue *pqueue)
{
	if(IsEmpty(pqueue)!=1)
		ClearQueue(pqueue);
	free(pqueue);
}

/*Clear a Queue*/
void ClearQueue(Queue *pqueue)
{
	while(IsEmpty(pqueue)!=1)
	{
		DeQueue(pqueue,NULL);
	}

}

/*Judge is Queue empty or not*/
int IsEmpty(Queue *pqueue)
{
	if(pqueue->front==NULL&&pqueue->rear==NULL&&pqueue->size==0)
		return 1;
	else
		return 0;
}

/*Get size of Queue*/
int GetSize(Queue *pqueue)
{
	return pqueue->size;
}

/*Check the head node*/
PNode GetFront(Queue *pqueue,Item *pitem)
{
	if(IsEmpty(pqueue)!=1&&pitem!=NULL)
	{
		*pitem = pqueue->front->data;
	}
	return pqueue->front;
}

/*Check the tail node*/

PNode GetRear(Queue *pqueue,Item *pitem)
{
	if(IsEmpty(pqueue)!=1&&pitem!=NULL)
	{
		*pitem = pqueue->rear->data;
	}
	return pqueue->rear;
}

/*Add new item to Queue*/
PNode EnQueue(Queue *pqueue,Item item)
{
	PNode pnode = (PNode)malloc(sizeof(Node));
	if(pnode != NULL)
	{
		pnode->data = item;
		pnode->next = NULL;
		
		if(IsEmpty(pqueue))
		{
			pqueue->front = pnode;
		}
		else
		{
			pqueue->rear->next = pnode;
		}
		pqueue->rear = pnode;
		pqueue->size++;
	}
	return pnode;
}

/*Get item out of Queue*/
PNode DeQueue(Queue *pqueue,Item *pitem)
{
	PNode pnode = pqueue->front;
	if(IsEmpty(pqueue)!=1&&pnode!=NULL)
	{
		if(pitem!=NULL)
			*pitem = pnode->data;
		pqueue->size--;
		pqueue->front = pnode->next;
		free(pnode);
		if(pqueue->size==0)
			pqueue->rear = NULL;
	}
	return pqueue->front;
}

/*traverse Queue and execute visit*/
void QueueTraverse(Queue *pqueue,void (*visit)())
{
	PNode pnode = pqueue->front;
	int i = pqueue->size;
	while(i--)
	{
		visit(pnode->data);
		pnode = pnode->next;
	}
		
}

/******************Circle Queue********************/

/*Get item out of CircleQueue*/

PNode DeCircleQueue(Queue *pqueue,Item *pitem)
{
	PNode pnode = pqueue->front;
	if(IsEmpty(pqueue)!=1&&pnode!=NULL)
	{
		if(pitem!=NULL)
			*pitem = pnode->data;
		pqueue->size--;
		pqueue->front = pnode->next;
		pqueue->rear->next =pqueue->front;
/*
* bug: when delete node in sleep_queue, free pnode
* will cause a error
*"glibc detected free(): invalid next size(fast)"			
*/
		//free(pnode);
		if(pqueue->size==0){
			pqueue->front = NULL;	
			pqueue->rear = NULL;
		}
		//free(pnode);
	}
	return pqueue->front;
}


/*move CircleQueue to next node and return new node*/
PNode MoveCircleQueue(Queue *pqueue,Item *pitem)
{
	if(IsEmpty(pqueue)!=1&&pitem!=NULL)
	{
		pqueue->front=pqueue->front->next;
		pqueue->rear = pqueue->rear->next;
		*pitem = pqueue->front->data;
	}
	return pqueue->front;
}
/*Add item to Circle Queue*/
PNode EnCircleQueue(Queue *pqueue,Item item)
{
	PNode pnode = (PNode)malloc(sizeof(Node));
	if(pnode != NULL)
	{
		pnode->data = item;
		
		if(IsEmpty(pqueue))
		{
			pnode->next=pnode;
			pqueue->rear = pnode;
		}
		else
		{
			pnode->next = pqueue->front;
			pqueue->rear->next = pnode;
		}
		pqueue->front = pnode;
		pqueue->size++;
	}
	return pnode;
}

